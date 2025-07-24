//
// Created by AI Assistant on 7/24/25
//

#include "PathFinder.h"
#include <QDebug>
#include <QtMath>
#include <QQueue>
#include <algorithm>

PathFinder::PathFinder(QObject* parent)
    : QObject(parent)
    , currentMap(nullptr)
{
}

PathFinder::~PathFinder() {
    cleanupNodes();
}

void PathFinder::initializeNavigation(Map* map) {
    if (!map) {
        qWarning() << "PathFinder::initializeNavigation: Map is null";
        return;
    }
    
    clearNavigation();
    currentMap = map;
    
    // 生成导航节点
    generateNavigationNodes();
    
    // 连接节点
    connectNodes();
    
    qDebug() << "PathFinder: Navigation initialized with" << allNodes.size() << "nodes";
}

NavigationPath PathFinder::findPath(const QPointF& start, const QPointF& goal) {
    NavigationPath path;
    
    if (!currentMap || allNodes.isEmpty()) {
        qWarning() << "PathFinder::findPath: Navigation not initialized";
        return path;
    }
    
    // 找到起点和终点最近的节点
    NavigationNode* startNode = getNearestNode(start);
    NavigationNode* goalNode = getNearestNode(goal);
    
    if (!startNode || !goalNode) {
        qWarning() << "PathFinder::findPath: Could not find start or goal node";
        return path;
    }
    
    if (startNode == goalNode) {
        // 起点和终点是同一个节点
        path.addStep(goalNode, NavigationNode::MovementType::Walk);
        path.isValid = true;
        return path;
    }
    
    // 清理所有节点的寻路数据
    for (auto* node : allNodes) {
        node->clearPathfindingData();
    }
    
    // A*算法实现
    QVector<NavigationNode*> openSet;
    QSet<NavigationNode*> openSetLookup;
    QSet<NavigationNode*> closedSet;
    
    // 初始化起点
    startNode->gCost = 0;
    startNode->hCost = startNode->getHeuristicCost(goalNode);
    openSet.append(startNode);
    openSetLookup.insert(startNode);
    startNode->inOpenSet = true;
    
    while (!openSet.isEmpty()) {
        // 找到f代价最小的节点
        NavigationNode* currentNode = *std::min_element(openSet.begin(), openSet.end(),
            [](NavigationNode* a, NavigationNode* b) {
                return a->fCost() < b->fCost();
            });
        
        // 从开放集合中移除当前节点
        openSet.removeOne(currentNode);
        openSetLookup.remove(currentNode);
        currentNode->inOpenSet = false;
        
        // 添加到关闭集合
        closedSet.insert(currentNode);
        currentNode->visited = true;
        
        // 检查是否到达目标
        if (currentNode == goalNode) {
            path = reconstructPath(goalNode);
            break;
        }
        
        // 检查所有邻居节点
        for (const auto& connection : currentNode->connections) {
            NavigationNode* neighbor = connection.target;
            
            if (closedSet.contains(neighbor)) {
                continue; // 已在关闭集合中
            }
            
            // 计算新的g代价
            qreal tentativeGCost = currentNode->gCost + connection.cost;
            
            bool inOpenSet = openSetLookup.contains(neighbor);
            
            if (!inOpenSet || tentativeGCost < neighbor->gCost) {
                neighbor->gCost = tentativeGCost;
                neighbor->hCost = neighbor->getHeuristicCost(goalNode);
                neighbor->parent = currentNode;
                
                if (!inOpenSet) {
                    openSet.append(neighbor);
                    openSetLookup.insert(neighbor);
                    neighbor->inOpenSet = true;
                }
            }
        }
    }
    
    if (!path.isValid) {
        qDebug() << "PathFinder: No path found from" << start << "to" << goal;
    }
    
    return path;
}

NavigationNode* PathFinder::getNearestNode(const QPointF& position) const {
    if (allNodes.isEmpty()) return nullptr;
    
    NavigationNode* nearest = nullptr;
    qreal minDistance = INFINITY;
    
    for (auto* node : allNodes) {
        NavigationNode tempNode(position);
        qreal distance = node->getDistanceTo(&tempNode);
        if (distance < minDistance) {
            minDistance = distance;
            nearest = node;
        }
    } 
    return nearest;
}

QVector<NavigationNode*> PathFinder::getNodesAt(const QPointF& position, qreal tolerance) const {
    QVector<NavigationNode*> result;
    for (auto* node : allNodes) {
        NavigationNode tempNode(position);
        if (node->getDistanceTo(&tempNode) <= tolerance) {
            result.append(node);
        }
    } 
    return result;
}

void PathFinder::clearNavigation() {
    cleanupNodes();
    platformNodes.clear();
    currentMap = nullptr;
}

void PathFinder::generateNavigationNodes() {
    if (!currentMap) return;
    
    // 生成平台节点
    generatePlatformNodes();
    
    // 生成地面节点
    generateGroundNodes();
    
    qDebug() << "PathFinder: Generated" << allNodes.size() << "navigation nodes";
}

void PathFinder::generatePlatformNodes() {
    if (!currentMap) return;
    
    // 获取所有平台
    const auto& platforms = currentMap->getPlatforms();
    
    for (int i = 0; i < platforms.size(); ++i) {
        const Platform& platform = platforms[i];
        
        // 在平台上生成节点
        qreal startX = platform.rect.left() + config.nodeSpacing / 2;
        qreal endX = platform.rect.right() - config.nodeSpacing / 2;
        
        QVector<NavigationNode*> platformNodeList;
        
        for (qreal x = startX; x <= endX; x += config.nodeSpacing) {
            QPointF nodePos(x, platform.height);
            auto* node = new NavigationNode(nodePos, NavigationNode::NodeType::Platform);
            node->platformId = i;
            
            allNodes.append(node);
            platformNodeList.append(node);
        }
        
        // 在平台边缘添加额外节点（用于跳跃）
        if (!platformNodeList.isEmpty()) {
            // 左边缘跳跃节点
            QPointF leftJumpPos(platform.rect.left() - config.nodeSpacing, platform.height);
            auto* leftJumpNode = new NavigationNode(leftJumpPos, NavigationNode::NodeType::Jump);
            leftJumpNode->platformId = i;
            allNodes.append(leftJumpNode);
            platformNodeList.append(leftJumpNode);
            
            // 右边缘跳跃节点
            QPointF rightJumpPos(platform.rect.right() + config.nodeSpacing, platform.height);
            auto* rightJumpNode = new NavigationNode(rightJumpPos, NavigationNode::NodeType::Jump);
            rightJumpNode->platformId = i;
            allNodes.append(rightJumpNode);
            platformNodeList.append(rightJumpNode);
        }
        
        platformNodes[i] = platformNodeList;
    }
}

void PathFinder::generateGroundNodes() {
    if (!currentMap) return;
    
    // 获取场景边界
    QRectF sceneBounds = currentMap->sceneBoundingRect();
    
    // 在地面上生成节点
    qreal groundHeight = sceneBounds.bottom();
    
    for (qreal x = sceneBounds.left(); x <= sceneBounds.right(); x += config.nodeSpacing) {
        // 获取该位置的实际地面高度
        qreal actualGroundHeight = currentMap->getFloorHeightAt(x);
        
        QPointF nodePos(x, actualGroundHeight);
        auto* node = new NavigationNode(nodePos, NavigationNode::NodeType::Ground);
        node->platformId = -1; // 地面节点
        
        allNodes.append(node);
    }
}

void PathFinder::connectNodes() {
    // 连接平台内部节点
    connectPlatformNodes();
    
    // 连接跳跃连接
    connectJumpConnections();
    
    // 连接下落连接
    connectFallConnections();
    
    // 连接蹲下穿越连接
    connectCrouchConnections();
    
    qDebug() << "PathFinder: Node connections established";
}

void PathFinder::connectPlatformNodes() {
    // 连接同一平台上的相邻节点
    for (const auto& nodeList : platformNodes) {
        for (int i = 0; i < nodeList.size() - 1; ++i) {
            NavigationNode* node1 = nodeList[i];
            NavigationNode* node2 = nodeList[i + 1];
            
            if (canWalkBetween(node1, node2)) {
                qreal cost = node1->getDistanceTo(node2);
                node1->addConnection(node2, NavigationNode::MovementType::Walk, cost);
                node2->addConnection(node1, NavigationNode::MovementType::Walk, cost);
            }
        }
    }
    
    // 连接地面节点
    QVector<NavigationNode*> groundNodes;
    for (auto* node : allNodes) {
        if (node->type == NavigationNode::NodeType::Ground) {
            groundNodes.append(node);
        }
    }
    
    // 按X坐标排序
    std::sort(groundNodes.begin(), groundNodes.end(),
        [](NavigationNode* a, NavigationNode* b) {
            return a->position.x() < b->position.x();
        });
    
    // 连接相邻地面节点
    for (int i = 0; i < groundNodes.size() - 1; ++i) {
        NavigationNode* node1 = groundNodes[i];
        NavigationNode* node2 = groundNodes[i + 1];
        
        if (canWalkBetween(node1, node2)) {
            qreal cost = node1->getDistanceTo(node2);
            node1->addConnection(node2, NavigationNode::MovementType::Walk, cost);
            node2->addConnection(node1, NavigationNode::MovementType::Walk, cost);
        }
    }
}

void PathFinder::connectJumpConnections() {
    // 连接可以跳跃到达的节点
    for (auto* fromNode : allNodes) {
        for (auto* toNode : allNodes) {
            if (fromNode == toNode) continue;
            
            if (canJumpBetween(fromNode, toNode)) {
                qreal cost = calculateMovementCost(fromNode, toNode, NavigationNode::MovementType::Jump);
                fromNode->addConnection(toNode, NavigationNode::MovementType::Jump, cost);
            }
        }
    }
}

void PathFinder::connectFallConnections() {
    // 连接可以下落到达的节点
    for (auto* fromNode : allNodes) {
        for (auto* toNode : allNodes) {
            if (fromNode == toNode) continue;
            
            if (canFallBetween(fromNode, toNode)) {
                qreal cost = calculateMovementCost(fromNode, toNode, NavigationNode::MovementType::Fall);
                fromNode->addConnection(toNode, NavigationNode::MovementType::Fall, cost);
            }
        }
    }
}

void PathFinder::connectCrouchConnections() {
    // 连接需要蹲下才能通过的节点
    for (auto* fromNode : allNodes) {
        for (auto* toNode : allNodes) {
            if (fromNode == toNode) continue;
            
            if (canCrouchThrough(fromNode, toNode)) {
                qreal cost = calculateMovementCost(fromNode, toNode, NavigationNode::MovementType::CrouchThrough);
                fromNode->addConnection(toNode, NavigationNode::MovementType::CrouchThrough, cost, true);
            }
        }
    }
}

bool PathFinder::canWalkBetween(NavigationNode* from, NavigationNode* to) const {
    if (!from || !to) return false;
    
    // 检查高度差
    qreal heightDiff = qAbs(from->position.y() - to->position.y());
    if (heightDiff > config.walkTolerance) return false;
    
    // 检查水平距离
    qreal horizontalDist = qAbs(from->position.x() - to->position.x());
    if (horizontalDist > config.nodeSpacing * 2) return false;
    
    // 检查路径上是否有障碍
    return !hasObstacleBetween(from->position, to->position);
}

bool PathFinder::canJumpBetween(NavigationNode* from, NavigationNode* to) const {
    if (!from || !to) return false;
    
    // 只能从较低位置跳到较高位置，或者水平跳跃
    qreal heightDiff = to->position.y() - from->position.y();
    if (heightDiff > config.maxJumpHeight) return false;
    
    // 检查水平距离
    qreal horizontalDist = qAbs(from->position.x() - to->position.x());
    if (horizontalDist > config.maxJumpDistance) return false;
    
    // 使用物理模拟检查是否可以跳跃到达
    return isJumpReachable(from->position, to->position);
}

bool PathFinder::canFallBetween(NavigationNode* from, NavigationNode* to) const {
    if (!from || !to) return false;
    
    // 只能从较高位置下落到较低位置
    qreal heightDiff = from->position.y() - to->position.y();
    if (heightDiff <= 0) return false;
    if (heightDiff > config.maxFallDistance) return false;
    
    // 检查水平距离（下落时的水平移动能力有限）
    qreal horizontalDist = qAbs(from->position.x() - to->position.x());
    if (horizontalDist > config.maxJumpDistance * 0.5) return false;
    
    return true;
}

bool PathFinder::canCrouchThrough(NavigationNode* from, NavigationNode* to) const {
    if (!from || !to) return false;
    
    // 检查是否在草地平台上（可以蹲下穿越）
    Platform* fromPlatform = getPlatformAt(from->position);
    if (!fromPlatform || fromPlatform->type != Platform::Type::Grass) {
        return false;
    }
    
    // 检查目标位置是否在平台下方
    qreal heightDiff = from->position.y() - to->position.y();
    if (heightDiff < config.crouchHeight || heightDiff > config.maxFallDistance) {
        return false;
    }
    
    // 检查水平距离
    qreal horizontalDist = qAbs(from->position.x() - to->position.x());
    if (horizontalDist > config.nodeSpacing * 3) return false;
    
    return true;
}

NavigationPath PathFinder::reconstructPath(NavigationNode* goalNode) const {
    NavigationPath path;
    
    if (!goalNode) {
        return path;
    }
    
    // 从目标节点回溯到起点
    QVector<NavigationNode*> nodeSequence;
    NavigationNode* current = goalNode;
    
    while (current) {
        nodeSequence.prepend(current);
        current = current->parent;
    }
    
    // 构建路径步骤
    for (int i = 0; i < nodeSequence.size(); ++i) {
        NavigationNode* node = nodeSequence[i];
        NavigationNode::MovementType moveType = NavigationNode::MovementType::Walk;
        bool requiresCrouch = false;
        qreal estimatedTime = 0.0;
        
        if (i > 0) {
            NavigationNode* prevNode = nodeSequence[i - 1];
            
            // 查找连接类型
            for (const auto& conn : prevNode->connections) {
                if (conn.target == node) {
                    moveType = conn.moveType;
                    requiresCrouch = conn.requiresCrouch;
                    break;
                }
            }
            
            // 估算移动时间
            switch (moveType) {
                case NavigationNode::MovementType::Walk:
                    estimatedTime = node->getDistanceTo(prevNode) / PhysicsConstants::HORIZONTAL_MOVE_SPEED;
                    break;
                case NavigationNode::MovementType::Jump:
                    estimatedTime = calculateJumpTime(prevNode->position, node->position);
                    break;
                case NavigationNode::MovementType::Fall:
                    estimatedTime = calculateFallTime(prevNode->position.y() - node->position.y());
                    break;
                case NavigationNode::MovementType::CrouchThrough:
                    estimatedTime = calculateFallTime(prevNode->position.y() - node->position.y()) + 0.5; // 额外蹲下时间
                    break;
            }
        }
        
        path.addStep(node, moveType, requiresCrouch, estimatedTime);
    }
    
    path.calculateTotals();
    path.isValid = true;
    
    return path;
}

qreal PathFinder::calculateMovementCost(NavigationNode* from, NavigationNode* to, NavigationNode::MovementType moveType) const {
    if (!from || !to) return INFINITY;
    
    qreal baseCost = from->getDistanceTo(to);
    
    switch (moveType) {
        case NavigationNode::MovementType::Walk:
            return baseCost;
        case NavigationNode::MovementType::Jump:
            return baseCost * 2.0; // 跳跃代价更高
        case NavigationNode::MovementType::Fall:
            return baseCost * 1.5; // 下落代价中等
        case NavigationNode::MovementType::CrouchThrough:
            return baseCost * 3.0; // 蹲下穿越代价最高
    }
    
    return baseCost;
}

bool PathFinder::isJumpReachable(const QPointF& start, const QPointF& target) const {
    // 简化的跳跃物理检查
    qreal horizontalDist = qAbs(target.x() - start.x());
    qreal verticalDist = target.y() - start.y();
    
    // 使用抛物线运动公式检查是否可达
    qreal jumpSpeed = qAbs(PhysicsConstants::JUMP_SPEED);
    qreal gravity = PhysicsConstants::GRAVITY_ACCELERATION;
    qreal moveSpeed = PhysicsConstants::HORIZONTAL_MOVE_SPEED;
    
    // 计算跳跃时间
    qreal timeToApex = jumpSpeed / gravity;
    qreal maxHeight = jumpSpeed * timeToApex - 0.5 * gravity * timeToApex * timeToApex;
    
    // 检查是否能达到目标高度
    if (verticalDist > maxHeight) {
        return false;
    }
    
    // 计算到达目标高度的时间
    qreal timeToTarget;
    if (verticalDist >= 0) {
        // 上升阶段
        timeToTarget = (jumpSpeed - qSqrt(jumpSpeed * jumpSpeed - 2 * gravity * verticalDist)) / gravity;
    } else {
        // 下降阶段
        timeToTarget = (jumpSpeed + qSqrt(jumpSpeed * jumpSpeed - 2 * gravity * verticalDist)) / gravity;
    }
    
    // 检查水平距离是否可达
    qreal maxHorizontalDist = moveSpeed * timeToTarget;
    
    return horizontalDist <= maxHorizontalDist;
}

qreal PathFinder::calculateJumpTime(const QPointF& start, const QPointF& target) const {
    qreal horizontalDist = qAbs(target.x() - start.x());
    qreal verticalDist = target.y() - start.y();
    
    qreal jumpSpeed = qAbs(PhysicsConstants::JUMP_SPEED);
    qreal gravity = PhysicsConstants::GRAVITY_ACCELERATION;
    
    // 计算到达目标高度的时间
    if (verticalDist >= 0) {
        return (jumpSpeed - qSqrt(qMax(0.0, jumpSpeed * jumpSpeed - 2 * gravity * verticalDist))) / gravity;
    } else {
        return (jumpSpeed + qSqrt(jumpSpeed * jumpSpeed - 2 * gravity * verticalDist)) / gravity;
    }
}

qreal PathFinder::calculateFallTime(qreal height) const {
    if (height <= 0) return 0.0;
    
    qreal gravity = PhysicsConstants::GRAVITY_ACCELERATION;
    return qSqrt(2 * height / gravity);
}

bool PathFinder::hasObstacleBetween(const QPointF& start, const QPointF& end) const {
    if (!currentMap) return false;
    
    // 简化的障碍检测 - 检查路径上是否有墙
    const auto& walls = currentMap->getWalls();
    
    for (const Wall& wall : walls) {
        // 简单的线段与矩形相交检测
        if (wall.rect.intersects(QRectF(start, end).normalized())) {
            return true;
        }
    }
    
    return false;
}

Platform* PathFinder::getPlatformAt(const QPointF& position) const {
    if (!currentMap) return nullptr;
    
    return currentMap->getPlatformAt(position);
}

void PathFinder::cleanupNodes() {
    for (auto* node : allNodes) {
        delete node;
    }
    allNodes.clear();
}

void PathFinder::debugPrintNavigation() const {
    qDebug() << "=== PathFinder Navigation Debug ===";
    qDebug() << "Total nodes:" << allNodes.size();
    
    for (int i = 0; i < allNodes.size() && i < 10; ++i) { // 只打印前10个节点
        const auto* node = allNodes[i];
        qDebug() << "Node" << i << ":"
                 << "Pos=" << node->position
                 << "Type=" << nodeTypeToString(node->type)
                 << "Platform=" << node->platformId
                 << "Connections=" << node->connections.size();
    }
}

QString PathFinder::nodeTypeToString(NavigationNode::NodeType type) const {
    switch (type) {
        case NavigationNode::NodeType::Ground: return "Ground";
        case NavigationNode::NodeType::Platform: return "Platform";
        case NavigationNode::NodeType::Jump: return "Jump";
        case NavigationNode::NodeType::Fall: return "Fall";
        case NavigationNode::NodeType::Crouch: return "Crouch";
    }
    return "Unknown";
}

QString PathFinder::movementTypeToString(NavigationNode::MovementType type) const {
    switch (type) {
        case NavigationNode::MovementType::Walk: return "Walk";
        case NavigationNode::MovementType::Jump: return "Jump";
        case NavigationNode::MovementType::Fall: return "Fall";
        case NavigationNode::MovementType::CrouchThrough: return "CrouchThrough";
    }
    return "Unknown";
}

#include "PathFinder.moc"
