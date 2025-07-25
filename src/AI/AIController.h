//
// Created by AI Assistant on 7/24/25
//

#ifndef QT_PROGRAMMING_2024_AICONTROLLER_H
#define QT_PROGRAMMING_2024_AICONTROLLER_H

#include <QObject>
#include <QPointF>
#include <QVector>
#include <QTimer>
#include <QGraphicsItem>
#include <queue>
#include <functional>
#include <unordered_set>
#include "../Items/Characters/Character.h"
#include "../Items/Maps/Map.h"

// 前向声明
class Weapon;
class Medicine;
class Pickable;

// 简单的寻路节点
struct PathNode {
    QPointF position;
    qreal gCost;  // 从起点到当前节点的实际代价
    qreal hCost;  // 从当前节点到终点的启发式代价
    qreal fCost() const { return gCost + hCost; } // 总代价
    PathNode* parent;
    
    PathNode(QPointF pos, qreal g = 0, qreal h = 0, PathNode* p = nullptr)
        : position(pos), gCost(g), hCost(h), parent(p) {}
    
    // 用于优先队列的比较函数（注意：优先队列默认是最大堆，所以这里用 > 来实现最小堆）
    bool operator>(const PathNode& other) const {
        return fCost() > other.fCost();
    }
};

// 用于哈希表的位置哈希函数
struct PositionHash {
    std::size_t operator()(const QPointF& pos) const {
        // 将位置量化到网格来减少精度问题
        int x = static_cast<int>(pos.x() / 20.0) * 20;
        int y = static_cast<int>(pos.y() / 20.0) * 20;
        return std::hash<int>()(x) ^ (std::hash<int>()(y) << 1);
    }
};

// 用于哈希表的位置相等比较
struct PositionEqual {
    bool operator()(const QPointF& lhs, const QPointF& rhs) const {
        // 在20像素范围内认为是同一个位置
        return QLineF(lhs, rhs).length() < 20.0;
    }
};

// 移动指令
enum class MoveCommand {
    Stop,
    MoveLeft,
    MoveRight, 
    Jump,
    JumpLeft,  // 左跳（斜方向）
    JumpRight  // 右跳（斜方向）
};

/**
 * @brief AI控制器类
 */
class AIController : public QObject {
    Q_OBJECT

public:
    explicit AIController(Character* controlledCharacter, QObject* parent = nullptr);
    ~AIController();

    // 基本控制接口
    void setEnabled(bool enabled);
    bool isEnabled() const { return aiEnabled; }
    
    // 玩家目标相关接口
    void setPlayerTarget(Character* player);
    Character* getPlayerTarget() const { return playerTarget; }
    QPointF getPlayerPosition() const;
    qreal getDistanceToPlayer() const;
    bool hasPlayerTarget() const { return playerTarget != nullptr; }
    
    // 地图设置接口
    void setMap(Map* gameMap) { map = gameMap; }
    
    // AI主要逻辑接口
    void updateAI(); // 处理AI的主要决策逻辑（包括寻路决策、攻击决策等）
    
    // 寻路相关接口
    bool moveToTarget(const QPointF& target);
    void stopPathing();
    bool isPathing() const { return currentPath.size() > 0; }
    void updatePathing(); // 更新寻路状态
    
    // 直接角色操作接口
    void moveLeft();
    void moveRight();
    void jump();
    void attack();
    void pickup();
    void squat();
    void stopMovement();

    void universalAction(bool left, bool right, bool up, bool down, bool attack, bool pickup);

private:
    // 核心组件
    Character* character;           // 控制的角色
    Character* playerTarget;        // 玩家角色引用
    Map* map;                      // 地图引用
    bool aiEnabled;                 // AI是否启用
    
    // 寻路系统
    QVector<QPointF> currentPath;   // 当前路径
    int currentPathIndex;           // 当前路径索引
    QTimer* pathUpdateTimer;        // 路径更新定时器
    MoveCommand lastCommand;        // 上一次的移动指令
    
    // AI决策系统状态
    QPointF lastPlayerPos;          // 玩家上次位置，用于检测玩家移动
    int updateCounter;              // 更新计数器，用于控制检查频率
    
    // 逃跑系统状态
    bool isFleeMode;                // 是否处于逃跑模式
    QPointF fleeTarget;             // 逃跑目标位置
    int fleeUpdateCounter;          // 逃跑更新计数器
    QTimer* dodgeTimer;             // 躲避定时器
    int dodgeCounter;               // 躲避计数器
    
    // 拾取系统状态
    QVector<QGraphicsItem*> nearbyPickableItems; // 附近的可拾取物品
    QTimer* pickupTimer;            // 拾取定时器
    bool isPickingMode;             // 是否处于拾取模式
    int pickupUpdateCounter;        // 拾取更新计数器
    
    // A*寻路算法
    QVector<QPointF> findPath(const QPointF& start, const QPointF& goal);
    qreal calculateHeuristic(const QPointF& a, const QPointF& b);
    QVector<QPointF> getNeighbors(const QPointF& pos);
    bool isValidPosition(const QPointF& pos);
    MoveCommand calculateMoveCommand(const QPointF& currentPos, const QPointF& targetPos);
    void executeMoveCommand(MoveCommand command);
    qreal getCharacterHeight() const; // 获取角色高度的辅助方法
    
    // 武器和战斗策略相关方法
    bool hasWeapon() const;           // 检查是否有武器
    bool isWeaponMelee() const;       // 检查当前武器是否为近战武器
    bool isWeaponRanged() const;      // 检查当前武器是否为远程武器
    qreal getOptimalAttackDistance() const; // 获取最佳攻击距离
    void updateCombatStrategy();      // 更新战斗策略
    
    // 逃跑和求生策略相关方法
    bool shouldFlee() const;          // 判断是否应该逃跑
    void updateFleeStrategy();        // 更新逃跑策略
    QPointF findFleePosition();       // 寻找安全的逃跑位置
    QGraphicsItem* findNearestMedicine(); // 寻找最近的药物
    void performEvasiveManeuvers();   // 执行规避动作
    bool isUnderRangedAttack() const; // 判断是否正在受到远程攻击
    void performDirectMovement(const QPointF& target); // 执行直接移动（寻路失败时的备用方案）
    
    // 拾取系统相关方法
    void updatePickupBehavior();      // 更新拾取行为
    void scanForPickableItems();      // 扫描附近的可拾取物品
    void cleanupInvalidItems();       // 清理无效物品
    QGraphicsItem* selectBestPickupTarget(); // 选择最佳拾取目标
    bool shouldPickupItem(QGraphicsItem* item); // 判断是否应该拾取某物品
    qreal calculateItemPriority(QGraphicsItem* item); // 计算物品拾取优先级
    bool isItemInPickupRange(QGraphicsItem* item); // 检查物品是否在拾取范围内
    void performOpportunisticPickup();  // 在移动过程中进行机会性拾取
    
private slots:
    void onPathUpdate(); // 定时器槽函数
};

#endif //QT_PROGRAMMING_2024_AICONTROLLER_H

