//
// Created by AI Assistant on 7/24/25
//

#ifndef QT_PROGRAMMING_2024_PATHFINDER_H
#define QT_PROGRAMMING_2024_PATHFINDER_H

#include <QObject>
#include <QPointF>
#include <QVector>
#include <QSet>
#include <QMap>
#include "NavigationNode.h"
#include "../Items/Maps/Map.h"
#include "../Physics/PhysicsConstants.h"

/**
 * @brief A*路径寻找算法实现
 * 
 * 该类负责在导航网格中寻找从起点到终点的最优路径
 * 支持多层平台、跳跃、下落和蹲下穿越等移动方式
 */
class PathFinder : public QObject {
    Q_OBJECT

public:
    explicit PathFinder(QObject* parent = nullptr);
    ~PathFinder();

    // 初始化导航网格
    void initializeNavigation(Map* map);
    
    // 寻找路径
    NavigationPath findPath(const QPointF& start, const QPointF& goal);
    
    // 获取最近的导航节点
    NavigationNode* getNearestNode(const QPointF& position) const;
    
    // 获取指定位置的所有可用节点
    QVector<NavigationNode*> getNodesAt(const QPointF& position, qreal tolerance = 20.0) const;
    
    // 清理导航数据
    void clearNavigation();
    
    // 调试方法
    void debugPrintNavigation() const;
    int getNodeCount() const { return allNodes.size(); }
    
    // 导航网格参数
    struct NavigationConfig {
        qreal nodeSpacing = 32.0;           // 节点间距
        qreal maxJumpHeight = 150.0;        // 最大跳跃高度
        qreal maxJumpDistance = 120.0;      // 最大跳跃距离
        qreal maxFallDistance = 300.0;      // 最大下落距离
        qreal walkTolerance = 10.0;         // 步行连接容忍度
        qreal crouchHeight = 64.0;          // 蹲下高度阈值
    } config;

private:
    // 导航节点管理
    QVector<NavigationNode*> allNodes;      // 所有导航节点
    QMap<int, QVector<NavigationNode*>> platformNodes; // 按平台分组的节点
    Map* currentMap;                        // 当前地图引用
    
    // A*算法实现
    NavigationPath reconstructPath(NavigationNode* goalNode) const;
    QVector<NavigationNode*> getNeighbors(NavigationNode* node) const;
    qreal calculateMovementCost(NavigationNode* from, NavigationNode* to, NavigationNode::MovementType moveType) const;
    
    // 导航网格构建
    void generateNavigationNodes();
    void generatePlatformNodes();
    void generateGroundNodes();
    void connectNodes();
    void connectPlatformNodes();
    void connectJumpConnections();
    void connectFallConnections();
    void connectCrouchConnections();
    
    // 节点连接判断
    bool canWalkBetween(NavigationNode* from, NavigationNode* to) const;
    bool canJumpBetween(NavigationNode* from, NavigationNode* to) const;
    bool canFallBetween(NavigationNode* from, NavigationNode* to) const;
    bool canCrouchThrough(NavigationNode* from, NavigationNode* to) const;
    
    // 几何计算辅助方法
    bool isPositionWalkable(const QPointF& position) const;
    bool hasObstacleBetween(const QPointF& start, const QPointF& end) const;
    qreal getGroundHeightAt(qreal x) const;
    Platform* getPlatformAt(const QPointF& position) const;
    
    // 物理模拟辅助方法
    bool isJumpReachable(const QPointF& start, const QPointF& target) const;
    qreal calculateJumpTime(const QPointF& start, const QPointF& target) const;
    qreal calculateFallTime(qreal height) const;
    
    // 内存管理
    void cleanupNodes();
    
    // 调试辅助
    QString nodeTypeToString(NavigationNode::NodeType type) const;
    QString movementTypeToString(NavigationNode::MovementType type) const;
};

#endif //QT_PROGRAMMING_2024_PATHFINDER_H
