//
// Created by guangnianye on 7/21/25
//

#include "RangedWeapon.h"
#include <QtMath>

RangedWeapon::RangedWeapon(QGraphicsItem *parent, const QString &pixmapPath)
    : Weapon(parent, pixmapPath) {
    // 远程武器的默认设置
}

void RangedWeapon::attack() {
    // 默认水平向右攻击
    QPointF horizontalDirection(1.0, 0.0);
    performRangedAttack(horizontalDirection);
}

void RangedWeapon::attack(const QPointF& targetPosition) {
    // 计算攻击方向
    QPointF currentPos = pos();
    QPointF direction = targetPosition - currentPos;
    
    // 归一化方向向量
    qreal length = qSqrt(direction.x() * direction.x() + direction.y() * direction.y());
    if (length > 0) {
        direction /= length;
    }
    
    performRangedAttack(direction);
}

void RangedWeapon::performRangedAttack(const QPointF& direction) {
    // 这里可以添加远程攻击的特效
    // 比如发射弹道、音效等
}
