//
// Created by AI Assistant on 7/24/25
//

#ifndef QT_PROGRAMMING_2024_NAVIGATIONNODE_H
#define QT_PROGRAMMING_2024_NAVIGATIONNODE_H

#include <QPointF>
#include <QVector>
#include <QList>

/**
 * @brief 导航节点类，用于构建导航网格和路径寻找
 * 
 * 导航节点表示地图上可以移动到的位置，包含位置信息、连接信息和寻路算法相关的数据
 */
class NavigationNode {
public:
    // 节点类型枚举
    enum class NodeType {
        Ground,          // 地面节点
        Platform,        // 平台节点  
        Jump,            // 跳跃节点
        Fall,            // 下落节点
        Crouch           // 蹲下穿越节点
    };
    
    // 移动类型枚举
    enum class MovementType {
        Walk,            // 步行
        Jump,            // 跳跃
        Fall,            // 下落  
        CrouchThrough    // 蹲下穿越
    };

public:
    explicit NavigationNode(const QPointF& position, NodeType type = NodeType::Ground);
    
    // 基本属性
    QPointF position;           // 节点位置
    NodeType type;              // 节点类型
    int platformId;             // 所属平台ID（-1表示地面）
    
    // 连接信息
    struct Connection {
        NavigationNode* target;     // 目标节点
        MovementType moveType;      // 移动类型
        qreal cost;                 // 移动代价
        bool requiresCrouch;        // 是否需要蹲下
        
        Connection(NavigationNode* t, MovementType mt, qreal c, bool rc = false)
            : target(t), moveType(mt), cost(c), requiresCrouch(rc) {}
    };
    
    QVector<Connection> connections;  // 连接到其他节点的列表
    
    // A*算法相关数据
    qreal gCost;                // 从起点到当前节点的实际代价
    qreal hCost;                // 从当前节点到终点的启发式代价
    qreal fCost() const { return gCost + hCost; }  // 总代价
    
    NavigationNode* parent;     // 父节点（用于路径重建）
    bool visited;               // 是否已访问
    bool inOpenSet;             // 是否在开放集合中
    
    // 节点操作方法
    void addConnection(NavigationNode* target, MovementType moveType, qreal cost, bool requiresCrouch = false);
    void clearPathfindingData();
    qreal getDistanceTo(const NavigationNode* other) const;
    qreal getHeuristicCost(const NavigationNode* target) const;
    
    // 检查是否可以从当前节点移动到目标节点
    bool canMoveTo(const NavigationNode* target) const;
    
    // 获取指定移动类型的连接
    QVector<Connection> getConnectionsByType(MovementType moveType) const;
};

/**
 * @brief 路径结构，包含路径节点和移动指令
 */
struct NavigationPath {
    struct PathStep {
        NavigationNode* node;           // 路径节点
        NavigationNode::MovementType moveType;  // 移动类型
        bool requiresCrouch;            // 是否需要蹲下
        qreal estimatedTime;            // 预估移动时间
        
        PathStep(NavigationNode* n, NavigationNode::MovementType mt, bool rc = false, qreal time = 0.0)
            : node(n), moveType(mt), requiresCrouch(rc), estimatedTime(time) {}
    };
    
    QVector<PathStep> steps;            // 路径步骤
    qreal totalCost;                    // 总代价
    qreal totalTime;                    // 总时间
    bool isValid;                       // 路径是否有效
    
    NavigationPath() : totalCost(0.0), totalTime(0.0), isValid(false) {}
    
    // 路径操作方法
    void clear();
    bool isEmpty() const { return steps.isEmpty(); }
    int size() const { return steps.size(); }
    PathStep& at(int index) { return steps[index]; }
    const PathStep& at(int index) const { return steps[index]; }
    
    // 添加路径步骤
    void addStep(NavigationNode* node, NavigationNode::MovementType moveType, bool requiresCrouch = false, qreal time = 0.0);
    
    // 计算总代价和时间
    void calculateTotals();
};

#endif //QT_PROGRAMMING_2024_NAVIGATIONNODE_H
