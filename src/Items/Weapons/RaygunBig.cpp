//
// Created by user on 7/23/25
//

#include "RaygunBig.h"
#include "../Characters/Character.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QtMath>

// BigLaserBullet 实现
BigLaserBullet::BigLaserBullet(const QPointF& startPos, const QPointF& direction, qreal damage, QGraphicsItem *parent)
    : Projectile(startPos, direction, damage, ":/Items/Weapons/Raygun/bullet.png", parent) {
    
    // 设置大型子弹属性
    setVelocity(direction * BULLET_SPEED);
    setGravityAffected(false); // 激光子弹不受重力影响
    
    // 设置子弹大小（比普通子弹大）
    setScale(0.6);
    
    qDebug() << "BigLaserBullet created at" << startPos << "with direction" << direction << "damage:" << damage;
}

void BigLaserBullet::onCharacterHit(Character* character) {
    if (character) {
        qDebug() << "BigLaserBullet hit character, dealing" << getDamage() << "damage";
        character->takeDamage(getDamage());
        cleanup(); // 子弹击中后立即消失
    }
}

// RaygunBig 实现
RaygunBig::RaygunBig(QGraphicsItem *parent)
    : RangedWeapon(parent, ":/Items/Weapons/Raygun/raygunBig.png") {
    
    // 设置武器属性
    setWeaponname("RaygunBig");
    setDamage(HIGH_DAMAGE);        // 高伤害值
    setAttackRange(1200.0);        // 射程更远
    type = WeaponType::Ranged;
    remainingbullet = MAX_BULLETS; // 初始化子弹数（较少）
    
    // 启动计时器
    lastShootTime.start();
    
    qDebug() << "RaygunBig created with" << MAX_BULLETS << "bullets, damage:" << HIGH_DAMAGE;
}

void RaygunBig::attack() {
    if (!canUse() || !canShoot()) {
        qDebug() << "Cannot shoot RaygunBig: bullets remaining:" << remainingbullet 
                 << "time since last shot:" << lastShootTime.elapsed();
        return;
    }
    
    // 获取角色面向方向
    QPointF direction = getCharacterDirection();
    shoot(direction);
}

void RaygunBig::shoot(const QPointF& direction) {
    if (!canUse() || !canShoot()) {
        return;
    }
    
    // 创建并发射大型子弹
    BigLaserBullet* bullet = createBigBullet(direction);
    if (bullet && scene()) {
        scene()->addItem(bullet);
        bullet->startFlight();
        
        // 消耗一发子弹
        remainingbullet--;
        qDebug() << "Big shot fired! Remaining bullets:" << remainingbullet;
        
        // 记录射击时间
        lastShootTime.restart();
        
        // 如果子弹用完，从角色身上移除武器
        if (remainingbullet <= 0) {
            qDebug() << "RaygunBig out of ammo!";
            // 这里可以添加武器用完后的处理逻辑
        }
    }
}

bool RaygunBig::canShoot() const {
    // 检查是否有子弹以及是否过了射击间隔（间隔较长）
    return canUse() && lastShootTime.elapsed() >= SHOOT_INTERVAL;
}

void RaygunBig::performRangedAttack(const QPointF& direction) {
    shoot(direction);
}

BigLaserBullet* RaygunBig::createBigBullet(const QPointF& direction) {
    // 获取角色位置作为子弹起始位置
    Character* character = qgraphicsitem_cast<Character*>(parentItem());
    QPointF startPos;
    
    if (character) {
        // 从角色中心位置开始
        startPos = character->pos();
        
        // 根据角色朝向调整子弹起始位置
        if (character->isDirectionRight()) {
            startPos.setX(startPos.x() + character->boundingRect().width() / 2 + 15); // 从角色右侧射出，稍微远一点
        } else {
            startPos.setX(startPos.x() - character->boundingRect().width() / 2 - 15); // 从角色左侧射出
        }
        
        // 调整Y坐标到角色中心高度
        startPos.setY(startPos.y() + character->boundingRect().height() / 3);
    } else {
        // 如果没有找到角色，使用武器位置
        startPos = pos();
        startPos += direction * 40.0; // 更大的偏移
    }
    
    // 创建大型子弹
    BigLaserBullet* bullet = new BigLaserBullet(startPos, direction, getDamage());
    bullet->setPos(startPos);
    
    return bullet;
}

QPointF RaygunBig::getCharacterDirection() const {
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
