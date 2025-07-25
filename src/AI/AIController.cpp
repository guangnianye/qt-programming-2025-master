//
// Created by linye on 7/25/25.
//

#include "AIController.h"
#include <QtMath>
#include <QLineF>
#include <QTimer>
#include <QRandomGenerator>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <limits>
#include "../Items/Weapons/Weapon.h"
#include "../Items/Weapons/MeleeWeapon.h"
#include "../Items/Weapons/RangedWeapon.h"
#include "../Items/Medicine/Medicine.h"
#include "../Items/Pickable.h"
#include "../Physics/PhysicsConstants.h"

AIController::AIController(Character* controlledCharacter, QObject* parent)
    : QObject(parent), character(controlledCharacter), playerTarget(nullptr), map(nullptr), 
      aiEnabled(false), currentPathIndex(0), lastCommand(MoveCommand::Stop),
      lastPlayerPos(0, 0), updateCounter(0), 
      isFleeMode(false), fleeTarget(0, 0), fleeUpdateCounter(0), dodgeCounter(0),
      isPickingMode(false), pickupUpdateCounter(0) {
    if (!character) {
        qWarning() << "AIController: Controlled character is null";
    }
    
    // 初始化路径更新定时器
    pathUpdateTimer = new QTimer(this);
    pathUpdateTimer->setInterval(300); // 增加到300ms，进一步减少更新频率
    connect(pathUpdateTimer, &QTimer::timeout, this, &AIController::onPathUpdate);
    
    // 初始化拾取定时器
    pickupTimer = new QTimer(this);
    pickupTimer->setInterval(100); // 拾取检查频率较高，100ms
    connect(pickupTimer, &QTimer::timeout, this, &AIController::performOpportunisticPickup);
    pickupTimer->start(); // 拾取定时器总是运行
    
    // 初始化躲避定时器
    dodgeTimer = new QTimer(this);
    dodgeTimer->setInterval(200); // 躲避检查频率，200ms
    connect(dodgeTimer, &QTimer::timeout, this, &AIController::performEvasiveManeuvers);
}

AIController::~AIController() {
    if (pathUpdateTimer) {
        pathUpdateTimer->stop();
    }
    if (pickupTimer) {
        pickupTimer->stop();
    }
    if (dodgeTimer) {
        dodgeTimer->stop();
    }
}

void AIController::setEnabled(bool enabled) {
    aiEnabled = enabled;
}

#pragma region Foudation Action Control

void AIController::moveLeft() {
    if (character) {
        character->setLeftDown(true);
    }
}

void AIController::moveRight() {
    if (character) {
        character->setRightDown(true);
    }
}

void AIController::jump() {
    if (character) {
        character->setUpDown(true);
    }
}

void AIController::attack() {
    if (character) {
        character->setAttacking(true);
    }
}

void AIController::pickup() {
    if (character) {
        character->setPickDown(true);
    }
}

void AIController::squat() {
    if (character) {
        character->setSquatting(true);
    }
}

void AIController::stopMovement() {
    if (character) {
        character->setLeftDown(false);
        character->setRightDown(false);
        character->setUpDown(false);
        character->setSquatting(false);
        character->setPickDown(false);
    }
}

void AIController::universalAction(bool left, bool right, bool up, bool down, bool attack, bool pickup) {
    if (character) {
        character->setLeftDown(left);
        character->setRightDown(right);
        character->setUpDown(up);
        character->setSquatting(down);
        character->setAttacking(attack);
        character->setPickDown(pickup);
    } 
}

#pragma endregion

#pragma region Player Target Management

void AIController::setPlayerTarget(Character* player) {
    playerTarget = player;
}

QPointF AIController::getPlayerPosition() const {
    if (playerTarget) {
        return playerTarget->pos();
    }
    return QPointF(0, 0); // 返回默认位置
}

qreal AIController::getDistanceToPlayer() const {
    if (!character || !playerTarget) {
        return -1.0;
    }
    
    QPointF aiPos = character->pos();
    QPointF playerPos = playerTarget->pos();
    
    // 计算欧几里得距离
    qreal dx = aiPos.x() - playerPos.x();
    qreal dy = aiPos.y() - playerPos.y();
    qreal distance = qSqrt(dx * dx + dy * dy);
    
    return distance;
}

#pragma endregion

#pragma region AI Decision System

void AIController::updateAI() {
    if (!aiEnabled || !character || !playerTarget) {
        return;
    }
    
    // 检查控制的角色是否还活着，如果死了就停止AI
    if (!character->isAlive()|| !playerTarget->isAlive()) {
        setEnabled(false);
        stopPathing();
        return;
    }
    
    // 首先检查是否需要进入或退出逃跑模式
    bool shouldEnterFleeMode = shouldFlee();
    if (shouldEnterFleeMode && !isFleeMode) {
        // 进入逃跑模式
        isFleeMode = true;
        fleeUpdateCounter = 0;
        stopPathing(); // 停止当前寻路，重新规划逃跑路径
        
        // 启动躲避定时器
        if (!dodgeTimer->isActive()) {
            dodgeTimer->start();
        }
    } else if (!shouldEnterFleeMode && isFleeMode) {
        // 退出逃跑模式
        isFleeMode = false;
        
        // 停止躲避定时器
        if (dodgeTimer->isActive()) {
            dodgeTimer->stop();
        }
    }
    
    // 更新拾取行为（无论什么模式都需要拾取）
    updatePickupBehavior();
    
    // 根据当前模式执行不同的策略
    if (isFleeMode) {
        updateFleeStrategy();
    } else {
        updateCombatStrategy();
        
        // 正常的战斗和移动逻辑
        QPointF playerPos = playerTarget->pos();
        QPointF aiPos = character->pos();
        
        // 检查AI是否需要重新规划路径（距离玩家较远或没有在寻路）
        qreal distanceToPlayer = QLineF(aiPos, playerPos).length();
        qreal optimalDistance = getOptimalAttackDistance();
        
        // 减少寻路更新的频率，避免过于频繁的重新规划
        updateCounter++;
        
        // 如果AI正在寻路，就不要干扰它
        if (isPathing()) {
            // 只有在玩家位置发生重大变化时才重新规划路径
            qreal playerMoveDist = QLineF(lastPlayerPos, playerPos).length();      
            if (playerMoveDist > 150.0 && updateCounter % 60 == 0) { // 每60帧检查一次，且玩家移动距离超过150
                lastPlayerPos = playerPos;
                moveToTarget(playerPos);
            }
            return; // 正在寻路时不执行下面的逻辑
        }
        
        // 根据武器类型调整移动和攻击策略
        if (distanceToPlayer > optimalDistance && updateCounter % 60 == 0) {
            // 距离太远，需要接近目标
            lastPlayerPos = playerPos;
            moveToTarget(playerPos);
        } else if (distanceToPlayer <= optimalDistance) {
            // 在最佳攻击距离内
            stopPathing();
            attack();
            
            // 远程武器需要保持距离，如果太近则后退
            if (hasWeapon() && isWeaponRanged() && distanceToPlayer < optimalDistance * 0.7) {
                // 计算远离玩家的方向
                QPointF escapeDirection = aiPos - playerPos;
                qreal length = QLineF(QPointF(0,0), escapeDirection).length();
                if (length > 0) {
                    escapeDirection = escapeDirection / length; // 标准化
                    QPointF escapeTarget = aiPos + escapeDirection * 150.0; // 后退150像素
                    
                    // 确保逃脱目标在有效范围内
                    if (isValidPosition(escapeTarget)) {
                        moveToTarget(escapeTarget);
                    }
                }
            }
            // 近战武器策略：尝试更接近玩家
            else if ((!hasWeapon() || isWeaponMelee()) && distanceToPlayer > optimalDistance * 0.5) {
                // 如果是近战武器且距离还不够近，继续接近
                if (updateCounter % 30 == 0) { // 减少频率避免过度寻路
                    moveToTarget(playerPos);
                }
            }
        }
    }
}

#pragma endregion

#pragma region Pathfinding System

bool AIController::moveToTarget(const QPointF& target) {
    if (!character || !map) {
        return false;
    }
    
    QPointF start = character->pos();
    
    // 检查起始位置和目标位置是否有效
    bool startValid = isValidPosition(start);
    bool targetValid = isValidPosition(target);
    currentPath = findPath(start, target);
    
    if (currentPath.isEmpty()) {
        return false;
    }
    
    currentPathIndex = 0;
    pathUpdateTimer->start();
    
    return true;
}

void AIController::stopPathing() {
    currentPath.clear();
    currentPathIndex = 0;
    pathUpdateTimer->stop();
    stopMovement();
    lastCommand = MoveCommand::Stop;
}

void AIController::updatePathing() {
    onPathUpdate();
}

void AIController::onPathUpdate() {
    if (!character || currentPath.isEmpty() || currentPathIndex >= currentPath.size()) {
        stopPathing();
        return;
    }
    
    QPointF currentPos = character->pos();
    QPointF targetPos = currentPath[currentPathIndex];
    
    // 检查是否到达当前目标点
    qreal distance = QLineF(currentPos, targetPos).length();
    // 增加到达阈值，并且添加额外的稳定性检查
    if (distance < 60.0) { // 进一步增加到达阈值
        currentPathIndex++;        
        if (currentPathIndex >= currentPath.size()) {
            stopPathing();
            return;
        }
        targetPos = currentPath[currentPathIndex];
    }
    
    // 添加位置稳定性检查，如果AI在两个位置之间振荡，则跳过当前目标
    static QPointF lastPos = currentPos;
    static int oscillationCount = 0;
    
    qreal moveDistance = QLineF(lastPos, currentPos).length();
    if (moveDistance > 150.0) { // 如果移动距离过大，说明可能在振荡
        oscillationCount++;
        if (oscillationCount > 3) {
            currentPathIndex++;
            oscillationCount = 0;
            if (currentPathIndex >= currentPath.size()) {
                stopPathing();
                return;
            }
            targetPos = currentPath[currentPathIndex];
        }
    } else {
        oscillationCount = 0;
    }
    lastPos = currentPos;
    
    // 计算并执行移动指令
    MoveCommand command = calculateMoveCommand(currentPos, targetPos);
    executeMoveCommand(command);
}

QVector<QPointF> AIController::findPath(const QPointF& start, const QPointF& goal) {
    if (!map) return QVector<QPointF>();
    
    // 转换起始点和目标点为角色底部位置
    qreal characterHeight = getCharacterHeight();
    QPointF startBottom(start.x(), start.y() + characterHeight);
    QPointF goalBottom(goal.x(), goal.y() + characterHeight);
    
    // 调整起始点和目标点到最近的平台上
    Platform* startPlatform = map->getNearestPlatform(startBottom, 50.0);
    Platform* goalPlatform = map->getNearestPlatform(goalBottom, 50.0);
    
    QPointF adjustedStart = start;
    QPointF adjustedGoal = goal;
    
    if (startPlatform) {
        adjustedStart = QPointF(start.x(), startPlatform->height - characterHeight);
    }
    if (goalPlatform) {
        adjustedGoal = QPointF(goal.x(), goalPlatform->height - characterHeight);
    }

    // 优化的A*实现 - 使用优先队列
    using PriorityQueue = std::priority_queue<PathNode, std::vector<PathNode>, std::greater<PathNode>>;
    using ClosedSet = std::unordered_set<QPointF, PositionHash, PositionEqual>;
    using OpenMap = std::unordered_map<QPointF, PathNode*, PositionHash, PositionEqual>;
    
    PriorityQueue openQueue;
    ClosedSet closedSet;
    OpenMap openMap; // 用于快速查找开放列表中的节点
    std::vector<PathNode*> allNodes; // 用于内存管理
    
    PathNode* startNode = new PathNode(adjustedStart, 0, calculateHeuristic(adjustedStart, adjustedGoal));
    allNodes.push_back(startNode);
    openQueue.push(*startNode);
    openMap[adjustedStart] = startNode;
        
    while (!openQueue.empty()) {
        // 从优先队列中取出F值最小的节点
        PathNode current = openQueue.top();
        openQueue.pop();
        
        // 检查该节点是否已经在关闭集合中（可能已被更好的路径替代）
        if (closedSet.find(current.position) != closedSet.end()) {
            continue;
        }
        
        // 将当前节点加入关闭集合
        closedSet.insert(current.position);        
        // 检查是否到达目标
        if (QLineF(current.position, adjustedGoal).length() < 80.0) {
            // 重建路径
            QVector<QPointF> path;
            
            // 找到当前节点的指针（用于回溯父节点）
            PathNode* nodePtr = nullptr;
            for (PathNode* node : allNodes) {
                if (QLineF(node->position, current.position).length() < 1.0) {
                    nodePtr = node;
                    break;
                }
            }
            
            while (nodePtr) {
                path.prepend(nodePtr->position);
                nodePtr = nodePtr->parent;
            }         
            // 清理内存
            for (auto node : allNodes) delete node;
            
            return path;
        }
        
        // 检查邻居
        QVector<QPointF> neighbors = getNeighbors(current.position);
        for (const QPointF& neighbor : neighbors) {
            // 跳过已在关闭集合中的节点
            if (closedSet.find(neighbor) != closedSet.end()) {
                continue;
            }
            
            qreal tentativeGCost = current.gCost + QLineF(current.position, neighbor).length();
            qreal hCost = calculateHeuristic(neighbor, adjustedGoal);
            
            // 检查是否在开放列表中
            auto openIt = openMap.find(neighbor);
            if (openIt != openMap.end()) {
                // 如果找到了更好的路径，更新节点
                if (tentativeGCost < openIt->second->gCost) {
                    openIt->second->gCost = tentativeGCost;
                    
                    // 找到当前节点的指针作为新的父节点
                    PathNode* currentPtr = nullptr;
                    for (PathNode* node : allNodes) {
                        if (QLineF(node->position, current.position).length() < 1.0) {
                            currentPtr = node;
                            break;
                        }
                    }
                    openIt->second->parent = currentPtr;
                    
                    // 将更新后的节点重新加入优先队列
                    openQueue.push(*openIt->second);
                }
            } else {
                // 创建新节点
                PathNode* currentPtr = nullptr;
                for (PathNode* node : allNodes) {
                    if (QLineF(node->position, current.position).length() < 1.0) {
                        currentPtr = node;
                        break;
                    }
                }
                
                PathNode* newNode = new PathNode(neighbor, tentativeGCost, hCost, currentPtr);
                allNodes.push_back(newNode);
                openQueue.push(*newNode);
                openMap[neighbor] = newNode;
            }
        }
        
        // 防止无限循环
        if (closedSet.size() > 300) {
            break;
        }
    }
    
    // 清理内存
    for (auto node : allNodes) delete node;
    
    return QVector<QPointF>(); // 没有找到路径
}

qreal AIController::calculateHeuristic(const QPointF& a, const QPointF& b) {
    qreal dx = qAbs(a.x() - b.x());
    qreal dy = a.y() - b.y(); // 正值表示a在b上方，负值表示a在b下方
    
    // 基础曼哈顿距离
    qreal baseCost = dx + qAbs(dy);
    
    // 调整垂直移动的成本
    if (dy < 0) {
        // 需要向上移动（跳跃），增加额外成本
        baseCost += qAbs(dy) * 1.5; // 向上移动成本更高
    } else if (dy > 0) {
        // 可以向下移动（下落），降低成本
        baseCost += dy * 0.8; // 向下移动成本更低
    }
    
    return baseCost;
}

QVector<QPointF> AIController::getNeighbors(const QPointF& pos) {
    QVector<QPointF> neighbors;
    
    if (!map) return neighbors;
    
    const qreal stepSize = 60.0; // 步长
    const qreal jumpHeight = 80.0; // 跳跃高度
    const qreal characterHeight = getCharacterHeight();
    
    // 获取当前位置的底部
    QPointF currentBottom(pos.x(), pos.y() + characterHeight);
    
    // 1. 左右移动 - 在同一平台上（增加更多步长选项）
    QVector<QPointF> horizontalMoves = {
        QPointF(pos.x() - stepSize, pos.y()),
        QPointF(pos.x() + stepSize, pos.y()),
        QPointF(pos.x() - stepSize * 2, pos.y()), // 更大的步长
        QPointF(pos.x() + stepSize * 2, pos.y())
    };
    
    for (const QPointF& move : horizontalMoves) {
        if (isValidPosition(move)) {
            neighbors.append(move);
        }
    }
    
    // 2. 向上跳跃到附近的平台
    QVector<Platform> platforms = map->getPlatforms();
    for (const Platform& platform : platforms) {
        // 检查平台是否在跳跃范围内（向上跳跃）
        qreal distX = qAbs(platform.rect.center().x() - pos.x());
        qreal distY = platform.height - currentBottom.y(); // 正值表示平台在上方
        
        // 只考虑向上跳跃的平台
        if (distX <= stepSize * 3 && distY > 0 && distY <= jumpHeight * 1.5) { // 增加水平搜索范围
            // 计算在该平台上的位置
            QPointF platformPos(platform.rect.center().x(), platform.height - characterHeight);
            
            // 确保位置在平台范围内
            if (platform.containsX(platformPos.x())) {
                neighbors.append(platformPos);
            }
        }
    }
    
    // 3. 重力下落到下方的平台（新增）
    for (const Platform& platform : platforms) {
        qreal distX = qAbs(platform.rect.center().x() - pos.x());
        qreal distY = currentBottom.y() - platform.height; // 正值表示平台在下方
        
        // 考虑下落的平台：水平距离在合理范围内，且平台在当前位置下方
        if (distX <= stepSize * 4 && distY > 10.0) { // 增加更大的水平距离，因为可以边走边落
            // 计算在该平台上的位置
            QPointF platformPos(platform.rect.center().x(), platform.height - characterHeight);
            
            // 确保位置在平台范围内
            if (platform.containsX(platformPos.x())) {
                neighbors.append(platformPos);
            }
        }
    }
    
    // 4. 直接下落到地面（如果没有其他平台阻挡）
    QVector<QPointF> fallPositions = {
        QPointF(pos.x() - stepSize, 0), // 会在isValidPosition中调整到正确的地面高度
        QPointF(pos.x(), 0),
        QPointF(pos.x() + stepSize, 0)
    };
    
    for (const QPointF& fallPos : fallPositions) {
        // 检查是否可以直接下落到这个水平位置的地面
        qreal floorHeight = map->getFloorHeightAt(fallPos.x());
        QPointF groundPos(fallPos.x(), floorHeight - characterHeight);
        
        // 确保地面位置在当前位置下方，且没有中间平台阻挡
        if (groundPos.y() > pos.y() + 10.0) {
            bool blocked = false;
            
            // 检查是否有平台阻挡下落路径
            for (const Platform& platform : platforms) {
                if (platform.containsX(fallPos.x()) && 
                    platform.height > currentBottom.y() && 
                    platform.height < floorHeight) {
                    blocked = true;
                    break;
                }
            }
            
            if (!blocked && isValidPosition(groundPos)) {
                neighbors.append(groundPos);
            }
        }
    }
        
    return neighbors;
}

bool AIController::isValidPosition(const QPointF& pos) {
    if (!map) return false;
    
    // 检查是否在场景范围内
    QRectF sceneRect(0, 0, 960, 640); // 假设场景大小
    if (!sceneRect.contains(pos)) return false;
    
    // 计算角色底部位置 - pos是角色顶部，需要加上角色高度
    qreal characterHeight = getCharacterHeight();
    QPointF bottomPos(pos.x(), pos.y() + characterHeight);
    
    // 检查底部位置是否有平台支撑
    Platform* platform = map->getNearestPlatform(bottomPos, 30.0);
    if (platform && platform->containsPoint(bottomPos)) {
        return true;
    }
    
    // 如果没有平台，检查是否在地面高度
    qreal floorHeight = map->getFloorHeightAt(pos.x());
    return bottomPos.y() >= floorHeight - 10.0; // 允许一些误差
}

MoveCommand AIController::calculateMoveCommand(const QPointF& currentPos, const QPointF& targetPos) {
    qreal dx = targetPos.x() - currentPos.x();
    qreal dy = targetPos.y() - currentPos.y();
    
    const qreal horizontalThreshold = 50.0;  // 进一步增加水平阈值
    const qreal verticalThreshold = 20.0;    // 增加垂直阈值
    const qreal fallThreshold = 30.0;        // 下落阈值
        
    // 需要跳跃（目标在上方）
    if (dy < -verticalThreshold) {
        if (qAbs(dx) < horizontalThreshold) {
            return MoveCommand::Jump;
        } else if (dx < 0) {
            return MoveCommand::JumpLeft;
        } else {
            return MoveCommand::JumpRight;
        }
    }
    // 需要下落（目标在下方）
    else if (dy > fallThreshold) {
        // 如果目标在下方，主要通过水平移动到目标上方，然后利用重力下落
        if (qAbs(dx) > horizontalThreshold) {
            if (dx < 0) {
                return MoveCommand::MoveLeft;
            } else {
                return MoveCommand::MoveRight;
            }
        } else {
            // 如果水平位置接近，只需要继续向前移动让重力发挥作用
            // 或者停止让角色自然下落
            return MoveCommand::Stop;
        }
    }
    // 水平移动（同一高度或接近高度）
    else if (qAbs(dx) > horizontalThreshold) {
        if (dx < 0) {
            return MoveCommand::MoveLeft;
        } else {
            return MoveCommand::MoveRight;
        }
    }
    return MoveCommand::Stop;
}

void AIController::executeMoveCommand(MoveCommand command) {
    if (!character) return;
    
    // 如果命令没有改变，就不需要重新设置
    if (command == lastCommand) {
        return;
    }
        
    // 先停止所有动作
    character->setLeftDown(false);
    character->setRightDown(false);
    character->setUpDown(false);
    
    switch (command) {
        case MoveCommand::MoveLeft:
            character->setLeftDown(true);
            break;
        case MoveCommand::MoveRight:
            character->setRightDown(true);
            break;
        case MoveCommand::Jump:
            character->setUpDown(true);
            break;
        case MoveCommand::JumpLeft:
            character->setLeftDown(true);
            character->setUpDown(true);
            break;
        case MoveCommand::JumpRight:
            character->setRightDown(true);
            character->setUpDown(true);
            break;
        case MoveCommand::Stop:
        default:
            // 已经停止所有动作
            break;
    }
    
    lastCommand = command;
    
    // 移动时进行机会性拾取检查
    // 注意：不直接调用拾取，让定时器处理避免冲突
}

qreal AIController::getCharacterHeight() const {
    if (character) {
        // 尝试获取角色的实际高度
        QRectF bounds = character->boundingRect();
        if (bounds.height() > 0) {
            return bounds.height();
        }
    }
    // 如果无法获取或角色为空，使用默认值128
    return 128.0;
}

#pragma endregion

#pragma region Pickup System

void AIController::updatePickupBehavior() {
    if (!character || !character->scene()) {
        return;
    }
    
    // 定期扫描附近的可拾取物品
    pickupUpdateCounter++;
    if (pickupUpdateCounter % 30 == 0) { // 每30帧扫描一次，减少性能消耗
        scanForPickableItems();
    }
    
    // 每10帧清理一次无效物品
    if (pickupUpdateCounter % 10 == 0) {
        cleanupInvalidItems();
    }
}

void AIController::cleanupInvalidItems() {
    if (!character || !character->scene()) {
        // 如果角色无效，直接清空所有物品
        nearbyPickableItems.clear();
        return;
    }
    
    QGraphicsScene* characterScene = character->scene();
    
    // 使用迭代器手动清理，避免访问可能已删除的对象
    auto it = nearbyPickableItems.begin();
    while (it != nearbyPickableItems.end()) {
        QGraphicsItem* item = *it;
        
        // 检查指针是否为空
        if (!item) {
            it = nearbyPickableItems.erase(it);
            continue;
        }
        
        // 检查物品是否仍在场景的物品列表中（更安全的检查方式）
        bool isValid = false;
        QList<QGraphicsItem*> sceneItems = characterScene->items();
        for (QGraphicsItem* sceneItem : sceneItems) {
            if (sceneItem == item) {
                isValid = true;
                break;
            }
        }
        
        if (!isValid) {
            it = nearbyPickableItems.erase(it);
        } else {
            ++it;
        }
    }
}

void AIController::scanForPickableItems() {
    nearbyPickableItems.clear();
    
    if (!character || !character->scene()) {
        return;
    }
    
    QPointF aiPos = character->pos();
    
    // 定义扫描范围，比拾取范围稍大一些，便于提前规划
    qreal scanRange = PhysicsConstants::PICKUP_DISTANCE * 2.0; // 扩大扫描范围
    QRectF scanArea(aiPos.x() - scanRange, aiPos.y() - scanRange,
                    scanRange * 2, scanRange * 2);
    
    QList<QGraphicsItem*> itemsInArea = character->scene()->items(scanArea);
    
    for (QGraphicsItem* item : itemsInArea) {
        // 严格的有效性检查 - 使用更安全的方式检查
        if (item && 
            item != character && 
            character->scene() && 
            character->scene()->items().contains(item) && 
            shouldPickupItem(item)) {
            nearbyPickableItems.append(item);
        }
    }
    
    // 按优先级排序，同时过滤掉无效物品
    nearbyPickableItems.erase(
        std::remove_if(nearbyPickableItems.begin(), nearbyPickableItems.end(),
                       [this](QGraphicsItem* item) {
                           return !item || !character->scene() || !character->scene()->items().contains(item);
                       }),
        nearbyPickableItems.end()
    );
    
    std::sort(nearbyPickableItems.begin(), nearbyPickableItems.end(),
              [this](QGraphicsItem* a, QGraphicsItem* b) {
                  // 再次检查有效性 - 使用更安全的方式
                  if (!a || !character->scene() || !character->scene()->items().contains(a)) return false;
                  if (!b || !character->scene() || !character->scene()->items().contains(b)) return true;
                  return calculateItemPriority(a) > calculateItemPriority(b);
              });
}

QGraphicsItem* AIController::selectBestPickupTarget() {
    if (nearbyPickableItems.isEmpty()) {
        return nullptr;
    }
    
    // 返回优先级最高且在拾取范围内的物品
    // 创建一个临时列表来存储仍然有效的物品
    QVector<QGraphicsItem*> validItems;
    
    for (QGraphicsItem* item : nearbyPickableItems) {
        // 检查物品是否仍然有效 - 更安全的检查方式
        if (item && character->scene() && character->scene()->items().contains(item)) {
            validItems.append(item);
        }
    }
    
    // 更新 nearbyPickableItems 为只包含有效物品
    nearbyPickableItems = validItems;
    
    // 在有效物品中查找可拾取的目标
    for (QGraphicsItem* item : nearbyPickableItems) {
        if (isItemInPickupRange(item)) {
            return item;
        }
    }
    
    return nullptr;
}

bool AIController::shouldPickupItem(QGraphicsItem* item) {
    if (!item || !character || !character->scene() || !character->scene()->items().contains(item)) return false;
    
    // 检查是否是武器
    if (auto weapon = dynamic_cast<Weapon*>(item)) {
        // 检查武器是否已被装备或者是否在地面上
        return weapon->parentItem() == nullptr;
    }
    
    // 检查是否是药物
    if (auto medicine = dynamic_cast<Medicine*>(item)) {
        // 药物总是值得拾取（用于控制资源）
        return true;
    }
    
    // 检查是否是其他可拾取物品
    if (dynamic_cast<Pickable*>(item)) {
        return true;
    }
    
    return false;
}

qreal AIController::calculateItemPriority(QGraphicsItem* item) {
    if (!character || !item) return 0.0;
    
    // 检查物品是否仍然有效（可能已被删除）- 使用更安全的方式
    if (!character->scene() || !character->scene()->items().contains(item)) return 0.0;
    
    QPointF aiPos = character->pos();
    QPointF itemPos = item->pos();
    qreal distance = QLineF(aiPos, itemPos).length();
    
    // 基础优先级（距离越近优先级越高）
    qreal basePriority = 1000.0 / (distance + 1.0);
    
    // 根据物品类型调整优先级
    if (auto weapon = dynamic_cast<Weapon*>(item)) {
        // 武器优先级
        if (!character->getWeapon()) {
            // 如果没有武器，武器优先级很高
            basePriority *= 3.0;
        } else {
            // 如果已有武器，仍然拾取用于控制资源
            basePriority *= 1.5;
        }
        
        // 在逃跑模式下，武器优先级降低
        if (isFleeMode) {
            basePriority *= 0.5;
        }
    }
    else if (auto medicine = dynamic_cast<Medicine*>(item)) {
        // 药物优先级
        QString medicineType = medicine->getTypeName();
        
        if (medicineType.contains("Health") || medicineType.contains("FullHeart")) {
            // 血瓶优先级根据当前血量调整 (0-100)
            qreal healthPercent = character->getHealthPercentage();
            if (healthPercent < 50.0) {
                basePriority *= 2.5; // 血量低时血瓶优先级很高
            } else {
                basePriority *= 1.3; // 血量充足时仍然拾取用于控制资源
            }
            
            // 在逃跑模式下，药物优先级大幅提升
            if (isFleeMode) {
                basePriority *= 3.0;
            }
        }
        else if (medicineType.contains("Gem")) {
            // 宝石优先级
            if (!character->hasGem()) {
                basePriority *= 2.0; // 没有宝石时优先级较高
            } else {
                basePriority *= 1.2; // 有宝石时仍拾取用于控制资源
            }
            
            // 在逃跑模式下，宝石优先级也提升
            if (isFleeMode) {
                basePriority *= 2.0;
            }
        }
        else {
            // 其他药物类型
            basePriority *= 1.5;
            
            // 在逃跑模式下，所有药物优先级都提升
            if (isFleeMode) {
                basePriority *= 2.0;
            }
        }
    }
    else if (dynamic_cast<Pickable*>(item)) {
        // 其他可拾取物品
        basePriority *= 1.2;
        
        // 在逃跑模式下，其他物品优先级降低
        if (isFleeMode) {
            basePriority *= 0.3;
        }
    }
    
    return basePriority;
}

bool AIController::isItemInPickupRange(QGraphicsItem* item) {
    if (!character || !item) return false;
    
    // 检查物品是否仍然有效（可能已被删除）- 使用更安全的方式
    if (!character->scene() || !character->scene()->items().contains(item)) return false;
    
    QPointF aiPos = character->pos();
    QPointF itemPos = item->pos();
    qreal distance = QLineF(aiPos, itemPos).length();
    
    return distance <= PhysicsConstants::PICKUP_DISTANCE;
}

void AIController::performOpportunisticPickup() {
    if (!aiEnabled || !character) {
        return;
    }
    
    // 寻找最佳拾取目标
    QGraphicsItem* targetItem = selectBestPickupTarget();
    
    // 多层检查目标物品的有效性 - 使用更安全的方式
    if (targetItem && 
        character->scene() && 
        character->scene()->items().contains(targetItem) && // 确保物品仍在场景中
        isItemInPickupRange(targetItem)) {
        
        // 执行拾取动作
        character->setPickDown(true);
        
        // 短暂延迟后释放拾取键，模拟按键操作
        QTimer::singleShot(50, [this]() {
            if (character) {
                character->setPickDown(false);
            }
        });
    }
}

#pragma endregion

#pragma region Weapon Strategy System

bool AIController::hasWeapon() const {
    return character && character->getWeapon() != nullptr;
}

bool AIController::isWeaponMelee() const {
    if (!hasWeapon()) return true; // 没有武器时使用拳头，属于近战
    
    Weapon* weapon = character->getWeapon();
    return weapon && weapon->type == WeaponType::Melee;
}

bool AIController::isWeaponRanged() const {
    if (!hasWeapon()) return false; // 没有武器时使用拳头，不是远程
    
    Weapon* weapon = character->getWeapon();
    return weapon && weapon->type == WeaponType::Ranged;
}

qreal AIController::getOptimalAttackDistance() const {
    if (!hasWeapon()) {
        // 没有武器时使用拳头，需要贴身攻击
        return 25.0;
    }
    
    Weapon* weapon = character->getWeapon();
    
    if (weapon->type == WeaponType::Melee) {
        // 近战武器：稍微接近一点以确保命中
        qreal weaponRange = weapon->getAttackRange();
        return weaponRange * 0.8; // 使用武器范围的80%作为最佳距离
    } else {
        // 远程武器：保持中等距离，既能攻击又能保持安全
        qreal weaponRange = weapon->getAttackRange();
        // 远程武器保持200-400像素的距离，根据武器射程调整
        return qMin(400.0, qMax(200.0, weaponRange * 0.3));
    }
}

void AIController::updateCombatStrategy() {
    if (!character || !playerTarget || !aiEnabled) {
        return;
    }
    
    // 根据武器类型调整AI行为参数
    if (hasWeapon() && isWeaponRanged()) {
        // 远程武器策略：保持距离，频繁攻击
        // 可以在这里调整远程武器的特殊行为
    } else {
        // 近战武器策略：积极接近，近身搏斗
        // 可以在这里调整近战武器的特殊行为
    }
}

#pragma endregion

#pragma region Flee and Survival Strategy System

bool AIController::shouldFlee() const {
    if (!character || !playerTarget) return false;
    
    // 获取双方血量百分比 (0-100)
    qreal aiHealthPercent = character->getHealthPercentage();
    qreal playerHealthPercent = playerTarget->getHealthPercentage();
    
    // 计算血量差值（正值表示AI血量更高，负值表示玩家血量更高）
    qreal healthDifference = aiHealthPercent - playerHealthPercent;
    
    // 情况1：AI血量低于20%，且玩家血量明显更高（差值超过10%）
    if (aiHealthPercent < 20.0 && healthDifference < -10.0) {
        return true;
    }
      
    // 情况2：AI血量低于30%，且玩家血量明显更高（差值超过20%）
    if (aiHealthPercent < 30.0 && healthDifference < -20.0) {
        return true;
    }
    
    // 情况3：AI血量低于50%，且玩家血量显著更高（差值超过30%）
    if (aiHealthPercent < 50.0 && healthDifference < -30.0) {
        return true;
    }
    
    // 情况4：受到远程攻击威胁且血量劣势
    if (isUnderRangedAttack()) {
        // 如果受到远程攻击威胁，且AI血量低于50%，且不占优势
        if (aiHealthPercent < 50.0 && healthDifference <= 10.0) {
            return true;
        }
        // 或者AI血量低于40%，无论血量差异如何
        if (aiHealthPercent < 40.0) {
            return true;
        }
    }
    
    // 情况5：双方血量都较低时，优先考虑谁能先拿到药物
    if (aiHealthPercent < 40.0 && playerHealthPercent < 40.0) {
        // 如果AI血量更低，且附近可能有药物可以争夺，考虑逃跑去拿药
        if (healthDifference < -10.0) {
            // 简单检查：如果AI血量劣势且都血量较低，倾向于逃跑寻找药物
            return true;
        }
    }
    
    return false;
}

void AIController::updateFleeStrategy() {
    if (!character || !playerTarget) {
        return;
    }
    
    fleeUpdateCounter++;
    
    // 优先寻找药物
    QGraphicsItem* nearestMedicine = findNearestMedicine();
    if (nearestMedicine) {
        // 如果找到了药物，优先去拾取
        QPointF medicinePos = nearestMedicine->pos();
        
        // 更频繁地检查和更新路径到药物 - 每30帧而不是60帧
        if (fleeUpdateCounter % 30 == 0 || !isPathing()) {
            bool pathSuccess = moveToTarget(medicinePos);
            if (!pathSuccess) {
                // 如果寻路失败，尝试直接朝药物方向移动
                performDirectMovement(medicinePos);
            }
        }
    } else {
        // 如果没有药物，寻找安全位置逃跑
        QPointF safePosition = findFleePosition();
        
        // 更频繁地检查和更新逃跑路径 - 每20帧而不是40帧
        if (fleeUpdateCounter % 20 == 0 || !isPathing()) {
            if (QLineF(character->pos(), safePosition).length() > 50.0) {
                bool pathSuccess = moveToTarget(safePosition);
                if (!pathSuccess) {
                    // 如果寻路失败，尝试直接移动
                    performDirectMovement(safePosition);
                }
            }
        }
    }
}

QPointF AIController::findFleePosition() {
    if (!character || !playerTarget || !map) {
        return character ? character->pos() : QPointF(0, 0);
    }
    
    QPointF aiPos = character->pos();
    QPointF playerPos = playerTarget->pos();
    
    // 计算远离玩家的方向
    QPointF fleeDirection = aiPos - playerPos;
    qreal length = QLineF(QPointF(0, 0), fleeDirection).length();
    
    if (length < 1.0) {
        // 如果距离太近，随机选择一个方向
        fleeDirection = QPointF(1.0, 0.0);
        length = 1.0;
    }
    
    // 标准化方向向量
    fleeDirection = fleeDirection / length;
    
    // 尝试多个逃跑距离，从远到近
    QVector<qreal> fleeDistances = {400.0, 300.0, 200.0, 150.0};
    
    for (qreal distance : fleeDistances) {
        QPointF candidate = aiPos + fleeDirection * distance;
        
        // 检查候选位置是否有效
        if (isValidPosition(candidate)) {
            return candidate;
        }
        
        // 如果直线逃跑不行，尝试左右偏移
        for (qreal angle : {-45.0, 45.0, -90.0, 90.0}) {
            qreal radians = angle * M_PI / 180.0;
            QPointF rotatedDirection(
                fleeDirection.x() * cos(radians) - fleeDirection.y() * sin(radians),
                fleeDirection.x() * sin(radians) + fleeDirection.y() * cos(radians)
            );
            
            QPointF offsetCandidate = aiPos + rotatedDirection * distance;
            if (isValidPosition(offsetCandidate)) {
                return offsetCandidate;
            }
        }
    }
    
    // 如果都不行，就在当前位置附近找一个安全点
    for (qreal radius = 100.0; radius <= 200.0; radius += 50.0) {
        for (qreal angle = 0; angle < 360; angle += 45) {
            qreal radians = angle * M_PI / 180.0;
            QPointF candidate = aiPos + QPointF(cos(radians) * radius, sin(radians) * radius);
            
            if (isValidPosition(candidate)) {
                return candidate;
            }
        }
    }
    
    // 实在找不到就原地不动
    return aiPos;
}

QGraphicsItem* AIController::findNearestMedicine() {
    if (!character || !character->scene()) {
        return nullptr;
    }
    
    QPointF aiPos = character->pos();
    QGraphicsItem* nearestMedicine = nullptr;
    qreal nearestDistance = std::numeric_limits<qreal>::max();
    
    // 扩大搜索范围来寻找药物
    qreal searchRange = 600.0; // 搜索范围600像素
    QRectF searchArea(aiPos.x() - searchRange, aiPos.y() - searchRange,
                      searchRange * 2, searchRange * 2);
    
    QList<QGraphicsItem*> itemsInArea = character->scene()->items(searchArea);
    
    for (QGraphicsItem* item : itemsInArea) {
        if (item && item != character && character->scene()->items().contains(item)) {
            // 检查是否是药物
            if (auto medicine = dynamic_cast<Medicine*>(item)) {
                qreal distance = QLineF(aiPos, item->pos()).length();
                if (distance < nearestDistance) {
                    nearestDistance = distance;
                    nearestMedicine = item;
                }
            }
        }
    }
    
    return nearestMedicine;
}

void AIController::performEvasiveManeuvers() {
    if (!character || !isFleeMode || !isUnderRangedAttack()) {
        return;
    }
    
    dodgeCounter++;
    
    // 随机跳跃来躲避远程攻击
    if (dodgeCounter % 3 == 0) { // 每3次躲避检查执行一次跳跃
        // 随机选择跳跃方向
        int randomDirection = QRandomGenerator::global()->bounded(3); // 0, 1, 2
        
        switch (randomDirection) {
            case 0:
                jump(); // 原地跳跃
                break;
            case 1:
                // 左跳
                character->setLeftDown(true);
                character->setUpDown(true);
                QTimer::singleShot(100, [this]() {
                    if (character) {
                        character->setLeftDown(false);
                        character->setUpDown(false);
                    }
                });
                break;
            case 2:
                // 右跳
                character->setRightDown(true);
                character->setUpDown(true);
                QTimer::singleShot(100, [this]() {
                    if (character) {
                        character->setRightDown(false);
                        character->setUpDown(false);
                    }
                });
                break;
        }
    }
}

bool AIController::isUnderRangedAttack() const {
    if (!character || !playerTarget) {
        return false;
    }
    
    // 简单判断：如果玩家有远程武器且距离在射程内，认为受到远程攻击威胁
    Weapon* playerWeapon = playerTarget->getWeapon();
    if (!playerWeapon || playerWeapon->type != WeaponType::Ranged) {
        return false;
    }
    
    qreal distanceToPlayer = QLineF(character->pos(), playerTarget->pos()).length();
    qreal weaponRange = playerWeapon->getAttackRange();
    
    // 如果玩家距离在武器射程的1.5倍内，认为受到威胁
    return distanceToPlayer <= weaponRange * 1.5;
}

void AIController::performDirectMovement(const QPointF& target) {
    if (!character) return;
    
    QPointF currentPos = character->pos();
    QPointF direction = target - currentPos;
    qreal distance = QLineF(currentPos, target).length();
    
    // 如果距离很近，就停止移动
    if (distance < 50.0) {
        stopMovement();
        return;
    }
    
    // 计算移动命令
    MoveCommand command = calculateMoveCommand(currentPos, target);
    
    // 停止寻路但保持直接移动
    stopPathing();
    
    // 执行移动命令
    executeMoveCommand(command);

}

#pragma endregion