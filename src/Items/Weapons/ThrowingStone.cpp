//
// Created by guangnianye on 7/21/25
//

#include "ThrowingStone.h"
#include "../../Physics/PhysicsConstants.h"
#include "../Characters/Character.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QtMath>
#include <QDebug>

// StoneProjectile 实现
StoneProjectile::StoneProjectile(const QPointF& startPos, const QPointF& direction, qreal damage, QGraphicsItem *parent)
    : Projectile(startPos, direction, damage, ":/Items/Weapons/ThrowingStone/ThrowingStone_Icon.png", parent)
{
    // 设置受重力影响
    setGravityAffected(true);
    
    // 计算抛物线运动的初始速度
    qreal angle = qDegreesToRadians(THROW_ANGLE);
    qreal speed = INITIAL_SPEED;
    
    // 根据方向调整速度分量
    qreal directionLength = qSqrt(direction.x() * direction.x() + direction.y() * direction.y());
    if (directionLength > 0) {
        QPointF normalizedDir = direction / directionLength;
        QPointF newVelocity;
        newVelocity.setX(normalizedDir.x() * speed * qCos(angle));
        newVelocity.setY(normalizedDir.y() * speed * qSin(angle) - 200); // 向上的初始速度分量
        setVelocity(newVelocity);
    } else {
        setVelocity(QPointF(speed * qCos(angle), -200));
    }
}

void StoneProjectile::updatePosition() {
    // 调用父类的更新位置方法
    Projectile::updatePosition();
}

void StoneProjectile::onCharacterHit(Character* character) {
    if (character && character->isAlive()) {
        // 对角色造成伤害
        character->takeDamage(getDamage());
        qDebug() << "Stone hit character! Damage:" << getDamage() 
                << "Target health:" << character->getCurrentHealth();
    }
    
    // 撞击后销毁石头
    cleanup();
}

// ThrowingStone 实现
ThrowingStone::ThrowingStone(QGraphicsItem *parent)
    : RangedWeapon(parent, ":/Items/Weapons/ThrowingStone/ThrowingStone_Icon.png")
{
    setWeaponname("ThrowingStone");
    setDamage(15.0);  // 设置伤害值
    type = WeaponType::Ranged;
}

void ThrowingStone::attack() {
    if (!canUse()) {
        qDebug() << "ThrowingStone: No throws remaining!";
        return;
    }
    
    // 默认向右投掷
    QPointF defaultDirection(1.0, 0.0);
    performRangedAttack(defaultDirection);
}

void ThrowingStone::throwStone(const QPointF& direction) {
    if (!canUse()) {
        qDebug() << "ThrowingStone: No throws remaining!";
        return;
    }
    
    performRangedAttack(direction);
}

void ThrowingStone::performRangedAttack(const QPointF& direction) {
    if (!canUse()) {
        return;
    }
    
    // 创建并发射投射物
    StoneProjectile* projectile = createProjectile(direction);
    if (projectile && scene()) {
        scene()->addItem(projectile);
        projectile->startFlight();
        
        // 减少剩余投掷次数
        remainingbullet--;
        
        qDebug() << "ThrowingStone: Remaining throws:" << remainingbullet;
        
        // 如果用完了，可以在这里添加移除武器的逻辑
        if (remainingbullet <= 0) {
            qDebug() << "ThrowingStone: Weapon exhausted!";
            // 这里可以添加移除武器的逻辑，比如从角色装备中移除
        }
    }
}

StoneProjectile* ThrowingStone::createProjectile(const QPointF& direction) {
    if (!scene()) {
        return nullptr;
    }
    
    // 从武器当前位置创建投射物
    QPointF startPos = scenePos();
    
    // 稍微调整起始位置，使其从武器前方发出
    QPointF normalizedDir = direction;
    qreal length = qSqrt(direction.x() * direction.x() + direction.y() * direction.y());
    if (length > 0) {
        normalizedDir /= length;
        startPos += normalizedDir * 30;  // 在武器前方30像素处开始
    }
    
    return new StoneProjectile(startPos, direction, getDamage());
}
