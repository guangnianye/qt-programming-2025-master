//
// Created by AI Assistant on 7/24/25
//

#include "AIController.h"
#include "../Items/Weapons/Weapon.h"
#include "../Items/Medicine/Medicine.h"
#include "../Items/Pickable.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QtMath>
#include <QRandomGenerator>

AIController::AIController(Character* controlledCharacter, QObject* parent)
    : QObject(parent)
    , character(controlledCharacter)
    , pathFinder(nullptr)
    , updateTimer(new QTimer(this))
    , currentState(AIState::Idle)
    , previousState(AIState::Idle)
    , targetType(TargetType::None)
    , targetCharacter(nullptr)
    , currentPathStep(0)
    , aiEnabled(false)
    , debugEnabled(false)
    , isWaitingForJump(false)
    , isCrouchingForPlatform(false)
    , lastDistanceToTarget(INFINITY)
    , stuckCounter(0)
{
    if (!character) {
        qWarning() << "AIController: Character is null";
        return;
    }
    
    // 连接更新定时器
    connect(updateTimer, &QTimer::timeout, this, &AIController::update);
    
    // 启动状态计时器
    stateTimer.start();
    targetUpdateTimer.start();
    pathUpdateTimer.start();
    movementTimer.start();
    actionTimer.start();
    
    debugLog("AIController initialized");
}

AIController::~AIController() {
    setEnabled(false);
    debugLog("AIController destroyed");
}

void AIController::initialize(PathFinder* pathFinderInstance) {
    pathFinder = pathFinderInstance;
    
    if (!pathFinder) {
        qWarning() << "AIController::initialize: PathFinder is null";
        return;
    }
    
    debugLog("AIController initialized with PathFinder");
}

void AIController::setEnabled(bool enabled) {
    if (aiEnabled == enabled) return;
    
    aiEnabled = enabled;
    
    if (enabled) {
        if (!pathFinder) {
            qWarning() << "AIController::setEnabled: PathFinder not initialized";
            return;
        }
        
        // 启动AI更新
        updateTimer->start(static_cast<int>(config.updateInterval));
        changeState(AIState::Idle);
        debugLog("AI enabled");
    } else {
        // 停用AI
        updateTimer->stop();
        clearAllInputs();
        clearTarget();
        changeState(AIState::Idle);
        debugLog("AI disabled");
    }
}

void AIController::setTargetPosition(const QPointF& position) {
    targetType = TargetType::Position;
    targetPosition = position;
    targetCharacter = nullptr;
    lastTargetPosition = position;
    
    debugLog(QString("Target position set to (%1, %2)").arg(position.x()).arg(position.y()));
    
    // 重新寻找路径
    if (aiEnabled) {
        findPathToTarget();
    }
}

void AIController::setTargetCharacter(Character* target) {
    if (!target) {
        clearTarget();
        return;
    }
    
    targetType = TargetType::Character;
    targetCharacter = target;
    targetPosition = target->pos();
    lastTargetPosition = targetPosition;
    
    debugLog(QString("Target character set"));
    
    // 重新寻找路径
    if (aiEnabled) {
        findPathToTarget();
    }
}

void AIController::clearTarget() {
    targetType = TargetType::None;
    targetCharacter = nullptr;
    targetPosition = QPointF();
    currentPath.clear();
    currentPathStep = 0;
    
    debugLog("Target cleared");
}

void AIController::update() {
    if (!aiEnabled || !character || !character->isAlive()) {
        return;
    }
    
    // 更新行为逻辑
    updateBehavior();
    
    // 更新移动逻辑
    updateMovement();
    
    // 更新战斗逻辑
    if (config.enableCombat) {
        updateCombat();
    }
    
    // 更新物品搜索逻辑
    if (config.enableItemCollection) {
        updateItemSearch();
    }
}

void AIController::changeState(AIState newState) {
    if (currentState == newState) return;
    
    previousState = currentState;
    currentState = newState;
    stateTimer.restart();
    
    debugLog(QString("State changed: %1 -> %2").arg(getStateString()).arg(static_cast<int>(newState)));
    
    // 清除输入状态
    clearAllInputs();
    
    // 重置状态相关标志
    isWaitingForJump = false;
    isCrouchingForPlatform = false;
}

void AIController::updateMovement() {
    if (!character || !pathFinder) return;
    
    // 更新目标角色的位置
    if (targetType == TargetType::Character && targetCharacter) {
        QPointF newPos = targetCharacter->pos();
        qreal distanceChange = QPointF(newPos - lastTargetPosition).manhattanLength();
        
        // 如果目标角色移动了较大距离，重新计算路径
        if (distanceChange > config.arrivalThreshold * 2) {
            targetPosition = newPos;
            lastTargetPosition = newPos;
            findPathToTarget();
        }
    }
    
    // 定期重新计算路径
    if (pathUpdateTimer.elapsed() > config.pathRecalculationInterval) {
        if (targetType != TargetType::None) {
            findPathToTarget();
        }
        pathUpdateTimer.restart();
    }
    
    // 执行当前路径
    followCurrentPath();
}

void AIController::updateBehavior() {
    // 行为决策逻辑
    decideBehavior();
    
    // 根据当前状态执行相应行为
    switch (currentState) {
        case AIState::Idle:
            handleIdleState();
            break;
        case AIState::Moving:
            handleMovingState();
            break;
        case AIState::Jumping:
            handleJumpingState();
            break;
        case AIState::Falling:
            handleFallingState();
            break;
        case AIState::Crouching:
            handleCrouchingState();
            break;
        case AIState::Attacking:
            handleAttackingState();
            break;
        case AIState::PickingUp:
            handlePickingUpState();
            break;
        case AIState::Searching:
            handleSearchingState();
            break;
    }
}

void AIController::updateCombat() {
    if (!config.enableCombat) return;
    
    // 检查是否有敌人在战斗范围内
    Character* enemy = findNearestEnemy();
    
    if (enemy && isInCombatRange()) {
        // 在战斗范围内，考虑攻击
        if (shouldAttack() && currentState != AIState::Attacking) {
            changeState(AIState::Attacking);
            setTargetCharacter(enemy);
        }
    } else if (enemy && config.enableCombat) {
        // 敌人在附近但不在战斗范围内，移动到战斗位置
        if (currentState != AIState::Moving) {
            setTargetCharacter(enemy);
            changeState(AIState::Moving);
        }
    }
    
    // 检查是否需要撤退
    if (shouldRetreat()) {
        QPointF retreatPosition = evaluateBestPosition();
        setTargetPosition(retreatPosition);
        changeState(AIState::Moving);
    }
}

void AIController::updateItemSearch() {
    if (!config.enableItemCollection) return;
    
    // 如果当前没有其他重要目标，搜索物品
    if (targetType == TargetType::None || currentState == AIState::Idle) {
        searchForTargets();
    }
}

bool AIController::findPathToTarget() {
    if (!pathFinder || targetType == TargetType::None) {
        currentPath.clear();
        return false;
    }
    
    QPointF startPos = getCharacterPosition();
    QPointF goalPos = targetPosition;
    
    // 寻找路径
    NavigationPath newPath = pathFinder->findPath(startPos, goalPos);
    
    if (newPath.isValid) {
        currentPath = newPath;
        currentPathStep = 0;
        debugLog(QString("Path found with %1 steps").arg(currentPath.size()));
        return true;
    } else {
        currentPath.clear();
        debugLog("No path found to target");
        return false;
    }
}

void AIController::followCurrentPath() {
    if (!currentPath.isValid || currentPathStep >= currentPath.size()) {
        return;
    }
    
    // 检查是否已到达当前步骤的目标
    const auto& currentStep = currentPath.at(currentPathStep);
    QPointF stepTarget = currentStep.node->position;
    qreal distanceToStep = QPointF(getCharacterPosition() - stepTarget).manhattanLength();
    
    if (distanceToStep <= config.arrivalThreshold) {
        // 到达当前步骤，移动到下一步
        currentPathStep++;
        
        if (currentPathStep >= currentPath.size()) {
            // 路径完成
            debugLog("Path completed");
            if (targetType != TargetType::Character) {
                // 如果不是跟踪角色，清除目标
                clearTarget();
                changeState(AIState::Idle);
            }
            return;
        }
    }
    
    // 执行当前路径步骤
    if (currentPathStep < currentPath.size()) {
        executePathStep(currentPath.at(currentPathStep));
    }
}

void AIController::executePathStep(const NavigationPath::PathStep& step) {
    QPointF targetPos = step.node->position;
    
    switch (step.moveType) {
        case NavigationNode::MovementType::Walk:
            moveTowards(targetPos);
            if (currentState != AIState::Moving) {
                changeState(AIState::Moving);
            }
            break;
            
        case NavigationNode::MovementType::Jump:
            if (!isWaitingForJump) {
                moveTowards(targetPos);
                performJump();
                changeState(AIState::Jumping);
                isWaitingForJump = true;
            }
            break;
            
        case NavigationNode::MovementType::Fall:
            moveTowards(targetPos);
            if (currentState != AIState::Falling) {
                changeState(AIState::Falling);
            }
            break;
            
        case NavigationNode::MovementType::CrouchThrough:
            if (!isCrouchingForPlatform) {
                performCrouch();
                changeState(AIState::Crouching);
                isCrouchingForPlatform = true;
            }
            moveTowards(targetPos);
            break;
    }
}

void AIController::moveTowards(const QPointF& position) {
    QPointF currentPos = getCharacterPosition();
    QPointF direction = position - currentPos;
    
    bool moveLeft = direction.x() < -5.0;
    bool moveRight = direction.x() > 5.0;
    
    simulateInput(moveLeft, moveRight, false, false, false, false);
}

void AIController::performJump() {
    simulateInput(false, false, true, false, false, false);
}

void AIController::performCrouch() {
    simulateInput(false, false, false, true, false, false);
}

void AIController::stopMovement() {
    clearAllInputs();
}

void AIController::decideBehavior() {
    // 基于环境和状态做出行为决策
    
    // 检查是否卡住
    if (isStuck()) {
        handleStuckSituation();
        return;
    }
    
    // 根据优先级决定行为
    if (config.enableCombat && findNearestEnemy()) {
        // 战斗优先
        return; // 战斗逻辑在updateCombat中处理
    }
    
    if (config.enableItemCollection && isNearPickableItem()) {
        // 物品拾取
        if (currentState != AIState::PickingUp) {
            changeState(AIState::PickingUp);
        }
        return;
    }
    
    // 如果没有目标，进入搜索状态
    if (targetType == TargetType::None && currentState == AIState::Idle) {
        changeState(AIState::Searching);
    }
}

void AIController::handleIdleState() {
    // 空闲状态 - 寻找目标或随机移动
    if (stateTimer.elapsed() > 2000) { // 2秒后开始搜索
        searchForTargets();
        if (targetType == TargetType::None) {
            // 没有找到目标，随机移动
            QPointF randomPos = evaluateBestPosition();
            setTargetPosition(randomPos);
            changeState(AIState::Moving);
        }
    }
}

void AIController::handleMovingState() {
    // 移动状态 - 检查是否到达目标
    if (hasReachedTarget()) {
        clearTarget();
        changeState(AIState::Idle);
    }
}

void AIController::handleJumpingState() {
    // 跳跃状态 - 等待角色落地
    if (character->isOnGround()) {
        isWaitingForJump = false;
        changeState(AIState::Moving);
    }
}

void AIController::handleFallingState() {
    // 下落状态 - 等待角色落地
    if (character->isOnGround()) {
        changeState(AIState::Moving);
    }
}

void AIController::handleCrouchingState() {
    // 蹲下状态 - 继续蹲下直到穿越平台
    if (stateTimer.elapsed() > 1000) { // 蹲下1秒后恢复
        isCrouchingForPlatform = false;
        changeState(AIState::Moving);
    }
}

void AIController::handleAttackingState() {
    // 攻击状态 - 执行攻击
    if (shouldAttack() && isInCombatRange()) {
        executeCombatAction();
    } else {
        changeState(AIState::Moving);
    }
}

void AIController::handlePickingUpState() {
    // 拾取状态 - 尝试拾取物品
    QGraphicsItem* item = getNearestPickableItem();
    if (item && shouldPickUpItem(item)) {
        simulateInput(false, false, false, false, true, false); // 拾取按键
    }
    
    // 短时间后回到移动状态
    if (stateTimer.elapsed() > 500) {
        changeState(AIState::Moving);
    }
}

void AIController::handleSearchingState() {
    // 搜索状态 - 主动寻找目标
    searchForTargets();
    
    if (targetType != TargetType::None) {
        changeState(AIState::Moving);
    } else if (stateTimer.elapsed() > 3000) {
        // 搜索3秒后回到空闲状态
        changeState(AIState::Idle);
    }
}

void AIController::searchForTargets() {
    // 按优先级搜索目标
    
    // 1. 搜索敌人
    if (config.enableCombat) {
        Character* enemy = findNearestEnemy();
        if (enemy) {
            setTargetCharacter(enemy);
            return;
        }
    }
    
    // 2. 搜索武器
    if (config.enableItemCollection) {
        QGraphicsItem* weapon = findNearestWeapon();
        if (weapon && shouldPickUpItem(weapon)) {
            setTargetPosition(weapon->pos());
            targetType = TargetType::Weapon;
            return;
        }
    }
    
    // 3. 搜索药物
    if (config.enableItemCollection) {
        QGraphicsItem* medicine = findNearestMedicine();
        if (medicine && shouldPickUpItem(medicine)) {
            setTargetPosition(medicine->pos());
            targetType = TargetType::Medicine;
            return;
        }
    }
}

Character* AIController::findNearestEnemy() {
    if (!character || !character->scene()) return nullptr;
    
    Character* nearestEnemy = nullptr;
    qreal minDistance = config.searchRadius;
    
    QList<QGraphicsItem*> items = character->scene()->items();
    for (QGraphicsItem* item : items) {
        if (auto otherCharacter = dynamic_cast<Character*>(item)) {
            if (otherCharacter != character && otherCharacter->isAlive()) {
                qreal distance = QPointF(character->pos() - otherCharacter->pos()).manhattanLength();
                if (distance < minDistance) {
                    minDistance = distance;
                    nearestEnemy = otherCharacter;
                }
            }
        }
    }
    
    return nearestEnemy;
}

QGraphicsItem* AIController::findNearestWeapon() {
    if (!character || !character->scene()) return nullptr;
    
    QGraphicsItem* nearestWeapon = nullptr;
    qreal minDistance = config.searchRadius;
    
    QList<QGraphicsItem*> items = character->scene()->items();
    for (QGraphicsItem* item : items) {
        if (auto weapon = dynamic_cast<Weapon*>(item)) {
            if (!weapon->parentItem()) { // 未被装备的武器
                qreal distance = QPointF(character->pos() - weapon->pos()).manhattanLength();
                if (distance < minDistance) {
                    minDistance = distance;
                    nearestWeapon = weapon;
                }
            }
        }
    }
    
    return nearestWeapon;
}

QGraphicsItem* AIController::findNearestMedicine() {
    if (!character || !character->scene()) return nullptr;
    
    QGraphicsItem* nearestMedicine = nullptr;
    qreal minDistance = config.searchRadius;
    
    QList<QGraphicsItem*> items = character->scene()->items();
    for (QGraphicsItem* item : items) {
        if (auto medicine = dynamic_cast<Medicine*>(item)) {
            qreal distance = QPointF(character->pos() - medicine->pos()).manhattanLength();
            if (distance < minDistance) {
                minDistance = distance;
                nearestMedicine = medicine;
            }
        }
    }
    
    return nearestMedicine;
}

QPointF AIController::evaluateBestPosition() {
    // 评估最佳位置 - 可以扩展为更复杂的策略
    QRectF sceneBounds = character->scene()->sceneRect();
    
    qreal x = sceneBounds.left() + QRandomGenerator::global()->bounded(static_cast<int>(sceneBounds.width()));
    qreal y = sceneBounds.bottom(); // 尝试移动到地面
    return QPointF(x, y);
}

bool AIController::isInCombatRange() const {
    if (!targetCharacter) return false;
    
    qreal distance = getDistanceToTarget();
    return distance <= config.combatRange;
}

bool AIController::shouldRetreat() const {
    if (!character) return false;
    
    return character->getCurrentHealth() <= config.retreatHealthThreshold;
}

bool AIController::shouldAttack() const {
    if (!config.enableCombat || !targetCharacter) return false;
    
    return isInCombatRange() && actionTimer.elapsed() > config.reactionTime;
}

void AIController::executeCombatAction() {
    if (!targetCharacter || !isInCombatRange()) return;
    
    // 面向目标
    QPointF direction = targetCharacter->pos() - character->pos();
    character->setDirection(direction.x() > 0);
    
    // 执行攻击
    simulateInput(false, false, false, false, false, true);
    actionTimer.restart();
}

bool AIController::isNearPickableItem() const {
    return getNearestPickableItem() != nullptr;
}

QGraphicsItem* AIController::getNearestPickableItem() const {
    if (!character || !character->scene()) return nullptr;
    
    QGraphicsItem* nearestItem = nullptr;
    qreal minDistance = config.arrivalThreshold * 2;
    
    QList<QGraphicsItem*> items = character->scene()->items();
    for (QGraphicsItem* item : items) {
        if (dynamic_cast<Pickable*>(item) || dynamic_cast<Weapon*>(item) || dynamic_cast<Medicine*>(item)) {
            qreal distance = QPointF(character->pos() - item->pos()).manhattanLength();
            if (distance < minDistance) {
                minDistance = distance;
                nearestItem = item;
            }
        }
    }
    
    return nearestItem;
}

bool AIController::shouldPickUpItem(QGraphicsItem* item) const {
    if (!item || !character) return false;
    
    // 检查武器
    if (auto weapon = dynamic_cast<Weapon*>(item)) {
        // 如果没有武器或当前武器较弱，拾取
        Weapon* currentWeapon = character->getWeapon();
        if (!currentWeapon || currentWeapon->getWeaponname() == "fist") {
            return true;
        }
        // 可以添加更复杂的武器比较逻辑
    }
    
    // 检查药物
    if (auto medicine = dynamic_cast<Medicine*>(item)) {
        // 如果血量较低，拾取药物
        return character->getCurrentHealth() < character->getMaxHealth() * 0.8;
    }
    
    return false;
}

qreal AIController::getDistanceToTarget() const {
    if (targetType == TargetType::None) return INFINITY;
    
    QPointF currentPos = getCharacterPosition();
    return QPointF(currentPos - targetPosition).manhattanLength();
}

bool AIController::hasReachedTarget() const {
    return getDistanceToTarget() <= config.arrivalThreshold;
}

bool AIController::isStuck() const {
    qreal currentDistance = getDistanceToTarget();
    
    // 如果距离目标的距离没有明显变化，可能卡住了
    if (qAbs(currentDistance - lastDistanceToTarget) < 5.0) {
        return true;
    }
    
    return false;
}

void AIController::handleStuckSituation() {
    stuckCounter++;
    
    if (stuckCounter > 10) { // 卡住超过10次
        // 尝试跳跃
        performJump();
        stuckCounter = 0;
        
        // 或者寻找新的目标
        if (stuckCounter > 20) {
            clearTarget();
            changeState(AIState::Searching);
            stuckCounter = 0;
        }
    }
}

QPointF AIController::getCharacterPosition() const {
    return character ? character->pos() : QPointF();
}

void AIController::simulateInput(bool left, bool right, bool up, bool down, bool pick, bool attack) {
    if (!character) return;
    
    character->setLeftDown(left);
    character->setRightDown(right);
    character->setUpDown(up);
    character->setSquatting(down);
    character->setPickDown(pick);
    character->setAttacking(attack);
}

void AIController::clearAllInputs() {
    simulateInput(false, false, false, false, false, false);
}

void AIController::debugLog(const QString& message) const {
    if (debugEnabled) {
        qDebug() << "AI:" << message;
    }
}

QString AIController::getStateString() const {
    switch (currentState) {
        case AIState::Idle: return "Idle";
        case AIState::Moving: return "Moving";
        case AIState::Jumping: return "Jumping";
        case AIState::Falling: return "Falling";
        case AIState::Crouching: return "Crouching";
        case AIState::Attacking: return "Attacking";
        case AIState::PickingUp: return "PickingUp";
        case AIState::Searching: return "Searching";
    }
    return "Unknown";
}

QString AIController::pathToString(const NavigationPath& path) const {
    QString result = QString("Path(%1 steps): ").arg(path.size());
    for (const auto& step : path.steps) {
        result += QString("[%1] ").arg(static_cast<int>(step.moveType));
    }
    return result;
}

#include "AIController.moc"
