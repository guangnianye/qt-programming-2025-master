//
// Created by AI Assistant on 7/24/25
//

#include "NavigationNode.h"
#include <QtMath>
#include <QDebug>

NavigationNode::NavigationNode(const QPointF& pos, NodeType nodeType)
    : position(pos)
    , type(nodeType)
    , platformId(-1)
    , gCost(0.0)
    , hCost(0.0)
    , parent(nullptr)
    , visited(false)
    , inOpenSet(false)
{
}

void NavigationNode::addConnection(NavigationNode* target, MovementType moveType, qreal cost, bool requiresCrouch) {
    if (!target) return;
    
    // 检查是否已存在连接
    for (const auto& conn : connections) {
        if (conn.target == target && conn.moveType == moveType) {
            return; // 连接已存在
        }
    }
    
    connections.append(Connection(target, moveType, cost, requiresCrouch));
}

void NavigationNode::clearPathfindingData() {
    gCost = 0.0;
    hCost = 0.0;
    parent = nullptr;
    visited = false;
    inOpenSet = false;
}

qreal NavigationNode::getDistanceTo(const NavigationNode* other) const {
    if (!other) return INFINITY;
    
    QPointF diff = position - other->position;
    return qSqrt(diff.x() * diff.x() + diff.y() * diff.y());
}

qreal NavigationNode::getHeuristicCost(const NavigationNode* target) const {
    if (!target) return INFINITY;
    
    // 使用曼哈顿距离作为启发式函数，考虑垂直移动的额外代价
    QPointF diff = position - target->position;
    qreal horizontalCost = qAbs(diff.x());
    qreal verticalCost = qAbs(diff.y()) * 1.5; // 垂直移动代价更高
    
    return horizontalCost + verticalCost;
}

bool NavigationNode::canMoveTo(const NavigationNode* target) const {
    if (!target) return false;
    
    // 检查是否有连接到目标节点
    for (const auto& conn : connections) {
        if (conn.target == target) {
            return true;
        }
    }
    
    return false;
}

QVector<NavigationNode::Connection> NavigationNode::getConnectionsByType(MovementType moveType) const {
    QVector<Connection> result;
    
    for (const auto& conn : connections) {
        if (conn.moveType == moveType) {
            result.append(conn);
        }
    }
    
    return result;
}

// NavigationPath 实现
void NavigationPath::clear() {
    steps.clear();
    totalCost = 0.0;
    totalTime = 0.0;
    isValid = false;
}

void NavigationPath::addStep(NavigationNode* node, NavigationNode::MovementType moveType, bool requiresCrouch, qreal time) {
    steps.append(PathStep(node, moveType, requiresCrouch, time));
}

void NavigationPath::calculateTotals() {
    totalCost = 0.0;
    totalTime = 0.0;
    
    for (const auto& step : steps) {
        totalTime += step.estimatedTime;
        
        // 根据移动类型计算代价
        switch (step.moveType) {
            case NavigationNode::MovementType::Walk:
                totalCost += 1.0;
                break;
            case NavigationNode::MovementType::Jump:
                totalCost += 2.0;
                break;
            case NavigationNode::MovementType::Fall:
                totalCost += 1.5;
                break;
            case NavigationNode::MovementType::CrouchThrough:
                totalCost += 3.0; // 蹲下穿越代价较高
                break;
        }
    }
    
    isValid = !steps.isEmpty();
}
