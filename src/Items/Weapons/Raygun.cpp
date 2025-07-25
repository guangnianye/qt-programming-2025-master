//
// Created by user on 7/23/25
//

#include "Raygun.h"
#include "../Characters/Character.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QtMath>

// LaserBullet 实现
LaserBullet::LaserBullet(const QPointF& startPos, const QPointF& direction, qreal damage, QGraphicsItem *parent)
    : Projectile(startPos, direction, damage, ":/Items/Weapons/Raygun/bullet.png", "raygun", parent) {
    
    // 设置子弹属性
    setVelocity(direction * BULLET_SPEED);
    setGravityAffected(false); // 激光子弹不受重力影响
    
    // 设置子弹大小（比较小的子弹）
    setScale(0.3);
    
    qDebug() << "LaserBullet created at" << startPos << "with direction" << direction;
}

void LaserBullet::onCharacterHit(Character* character) {
    if (character) {
        qDebug() << "LaserBullet hit character, dealing" << getDamage() << "damage";
        character->takeDamage(getDamage(), "raygun");
        cleanup(); // 子弹击中后立即消失
    }
}

// Raygun 实现
Raygun::Raygun(QGraphicsItem *parent)
    : RangedWeapon(parent, ":/Items/Weapons/Raygun/raygun.png") {
    
    // 设置武器属性
    setWeaponname("Raygun");
    setDamage(15.0);           // 激光枪伤害值
    setAttackRange(1000.0);    // 射程很远
    type = WeaponType::Ranged;
    remainingbullet = MAX_BULLETS; // 初始化子弹数
    
    // 启动计时器
    lastShootTime.start();
    
    qDebug() << "Raygun created with" << MAX_BULLETS << "bullets";
}

void Raygun::attack() {
    if (!canUse() || !canShoot()) {
        qDebug() << "Cannot shoot: bullets remaining:" << remainingbullet;
        return;
    }
    
    // 获取角色面向方向
    QPointF direction = getCharacterDirection();
    shoot(direction);
}

void Raygun::shoot(const QPointF& direction) {
    if (!canUse() || !canShoot()) {
        return;
    }
    
    // 创建并发射子弹
    LaserBullet* bullet = createBullet(direction);
    if (bullet && scene()) {
        scene()->addItem(bullet);
        bullet->startFlight();
        
        // 消耗一发子弹
        remainingbullet--;
        qDebug() << "Shot fired! Remaining bullets:" << remainingbullet;
        
        // 记录射击时间
        lastShootTime.restart();
        
        // 如果子弹用完，从角色身上移除武器
        if (remainingbullet <= 0) {
            qDebug() << "Raygun out of ammo!";
            // 这里可以添加武器用完后的处理逻辑
            // 比如通知角色武器已经用完
        }
    }
}

bool Raygun::canShoot() const {
    // 检查是否有子弹以及是否过了射击间隔
    return canUse() && lastShootTime.elapsed() >= SHOOT_INTERVAL;
}

void Raygun::performRangedAttack(const QPointF& direction) {
    shoot(direction);
}

LaserBullet* Raygun::createBullet(const QPointF& direction) {
    // 获取角色位置作为子弹起始位置
    Character* character = qgraphicsitem_cast<Character*>(parentItem());
    QPointF startPos;
    
    if (character) {
        // 从角色中心位置开始
        startPos = character->pos();
        
        // 根据角色朝向调整子弹起始位置
        if (character->isDirectionRight()) {
            startPos.setX(startPos.x() + character->boundingRect().width() / 2 + 10); // 从角色右侧射出
        } else {
            startPos.setX(startPos.x() - character->boundingRect().width() / 2 - 10); // 从角色左侧射出
        }
        
        // 调整Y坐标到角色中心高度
        startPos.setY(startPos.y() + character->boundingRect().height() / 3);
    } else {
        // 如果没有找到角色，使用武器位置
        startPos = pos();
        startPos += direction * 30.0;
    }
    
    // 创建子弹
    LaserBullet* bullet = new LaserBullet(startPos, direction, getDamage());
    bullet->setPos(startPos);
    
    return bullet;
}

QPointF Raygun::getCharacterDirection() const {
    // 尝试从父对象（应该是Character）获取方向
    Character* character = qgraphicsitem_cast<Character*>(parentItem());
    if (character) {
        // 根据角色面向方向返回单位向量
        if (character->isDirectionRight()) {
            return QPointF(1.0, 0.0); // 向右
        } else {
            return QPointF(-1.0, 0.0); // 向左
        }
    }
    
    // 默认向右
    return QPointF(1.0, 0.0);
}

