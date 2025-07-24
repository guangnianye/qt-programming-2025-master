//
// Created by AI Assistant on 7/24/25
//

#ifndef QT_PROGRAMMING_2024_AICONTROLLER_H
#define QT_PROGRAMMING_2024_AICONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QPointF>
#include <QElapsedTimer>
#include "PathFinder.h"
#include "NavigationNode.h"
#include "../Items/Characters/Character.h"

/**
 * @brief AI控制器类，负责控制AI角色的行为和移动
 * 
 * 该类集成了路径寻找、行为状态机和移动控制，
 * 使AI角色能够智能地在多层平台环境中移动
 */
class AIController : public QObject {
    Q_OBJECT

public:
    // AI行为状态枚举
    enum class AIState {
        Idle,           // 空闲状态
        Moving,         // 移动状态
        Jumping,        // 跳跃状态
        Falling,        // 下落状态
        Crouching,      // 蹲下状态
        Attacking,      // 攻击状态
        PickingUp,      // 拾取状态
        Searching       // 搜索状态（寻找目标）
    };
    
    // AI目标类型枚举
    enum class TargetType {
        None,           // 无目标
        Position,       // 位置目标
        Character,      // 角色目标
        Weapon,         // 武器目标
        Medicine        // 药物目标
    };

public:
    explicit AIController(Character* controlledCharacter, QObject* parent = nullptr);
    ~AIController();

    // AI控制接口
    void initialize(PathFinder* pathFinder);
    void setEnabled(bool enabled);
    bool isEnabled() const { return aiEnabled; }
    
    // 目标设置
    void setTargetPosition(const QPointF& position);
    void setTargetCharacter(Character* target);
    void clearTarget();
    
    // AI配置参数
    struct AIConfig {
        qreal updateInterval = 100.0;           // AI更新间隔（毫秒）
        qreal arrivalThreshold = 20.0;          // 到达阈值（像素）
        qreal pathRecalculationInterval = 2000.0; // 路径重新计算间隔（毫秒）
        qreal reactionTime = 500.0;             // 反应时间（毫秒）
        qreal searchRadius = 300.0;             // 搜索半径（像素）
        qreal combatRange = 100.0;              // 战斗范围（像素）
        qreal retreatHealthThreshold = 30.0;    // 撤退血量阈值
        bool enableCombat = true;               // 是否启用战斗
        bool enableItemCollection = true;       // 是否启用物品收集
    } config;
    
    // 状态查询
    AIState getCurrentState() const { return currentState; }
    TargetType getTargetType() const { return targetType; }
    QPointF getTargetPosition() const { return targetPosition; }
    Character* getTargetCharacter() const { return targetCharacter; }
    bool hasValidPath() const { return currentPath.isValid; }
    
    // 调试信息
    void setDebugEnabled(bool enabled) { debugEnabled = enabled; }
    QString getStateString() const;

public slots:
    // 主要更新循环
    void update();
    
    // 状态切换
    void changeState(AIState newState);

private slots:
    // 内部更新方法
    void updateMovement();
    void updateBehavior();
    void updateCombat();
    void updateItemSearch();

private:
    // 核心组件
    Character* character;           // 控制的角色
    PathFinder* pathFinder;         // 路径寻找器
    QTimer* updateTimer;            // 更新定时器
    
    // AI状态
    AIState currentState;           // 当前状态
    AIState previousState;          // 前一个状态
    QElapsedTimer stateTimer;       // 状态计时器
    
    // 目标管理
    TargetType targetType;          // 目标类型
    QPointF targetPosition;         // 目标位置
    Character* targetCharacter;     // 目标角色
    QElapsedTimer targetUpdateTimer; // 目标更新计时器
    
    // 路径管理
    NavigationPath currentPath;     // 当前路径
    int currentPathStep;            // 当前路径步骤索引
    QElapsedTimer pathUpdateTimer;  // 路径更新计时器
    QPointF lastTargetPosition;     // 上次目标位置
    
    // 移动控制
    bool aiEnabled;                 // AI是否启用
    bool debugEnabled;              // 调试是否启用
    QElapsedTimer movementTimer;    // 移动计时器
    QElapsedTimer actionTimer;      // 动作计时器
    
    // 内部状态
    bool isWaitingForJump;          // 是否等待跳跃
    bool isCrouchingForPlatform;    // 是否为穿越平台而蹲下
    qreal lastDistanceToTarget;     // 到目标的最后距离
    int stuckCounter;               // 卡住计数器
    
    // 路径寻找和移动
    bool findPathToTarget();
    void followCurrentPath();
    void executePathStep(const NavigationPath::PathStep& step);
    
    // 移动控制方法
    void moveTowards(const QPointF& position);
    void performJump();
    void performCrouch();
    void stopMovement();
    
    // 行为决策
    void decideBehavior();
    void handleIdleState();
    void handleMovingState();
    void handleJumpingState();
    void handleFallingState();
    void handleCrouchingState();
    void handleAttackingState();
    void handlePickingUpState();
    void handleSearchingState();
    
    // 目标检测和评估
    void searchForTargets();
    Character* findNearestEnemy();
    QGraphicsItem* findNearestWeapon();
    QGraphicsItem* findNearestMedicine();
    QPointF evaluateBestPosition();
    
    // 战斗相关
    bool isInCombatRange() const;
    bool shouldRetreat() const;
    bool shouldAttack() const;
    void executeCombatAction();
    
    // 物品相关
    bool isNearPickableItem() const;
    QGraphicsItem* getNearestPickableItem() const;
    bool shouldPickUpItem(QGraphicsItem* item) const;
    
    // 工具方法
    qreal getDistanceToTarget() const;
    bool hasReachedTarget() const;
    bool isStuck() const;
    void handleStuckSituation();
    QPointF getCharacterPosition() const;
    
    // 输入模拟
    void simulateInput(bool left, bool right, bool up, bool down, bool pick, bool attack);
    void clearAllInputs();
    
    // 调试输出
    void debugLog(const QString& message) const;
    QString pathToString(const NavigationPath& path) const;
};

#endif //QT_PROGRAMMING_2024_AICONTROLLER_H
