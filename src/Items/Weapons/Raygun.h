//
// Created by user on 7/23/25
//

#ifndef QT_PROGRAMMING_2024_RAYGUN_H
#define QT_PROGRAMMING_2024_RAYGUN_H

#include "RangedWeapon.h"
#include "Projectile.h"
#include <QElapsedTimer>

// 激光子弹投射物类
class LaserBullet : public Projectile {
    Q_OBJECT
public:
    explicit LaserBullet(const QPointF& startPos, const QPointF& direction, qreal damage, QGraphicsItem *parent = nullptr);

protected:
    // 重写角色碰撞处理
    void onCharacterHit(Character* character) override;

private:
    static constexpr qreal BULLET_SPEED = 800.0; // 子弹速度
};

// 激光枪武器类
class Raygun : public RangedWeapon {
public:
    explicit Raygun(QGraphicsItem *parent);
    
    // 重写攻击方法
    void attack() override;
    
    // 朝指定方向射击
    void shoot(const QPointF& direction);
    
    // 获取剩余子弹数
    int getRemainingBullets() const { return remainingbullet; }
    
    // 检查是否可以射击（考虑射击间隔）
    bool canShoot() const;

protected:
    // 重写远程攻击逻辑
    void performRangedAttack(const QPointF& direction) override;

private:
    static constexpr int MAX_BULLETS = 10;      // 最大子弹数
    static constexpr qint64 SHOOT_INTERVAL = 200; // 射击间隔（毫秒）
    static constexpr qreal BULLET_SPEED = 800.0; // 子弹速度
    
    QElapsedTimer lastShootTime;    // 记录上次射击时间
    
    // 创建子弹投射物
    LaserBullet* createBullet(const QPointF& direction);
    
    // 获取角色面向方向
    QPointF getCharacterDirection() const;
};

#endif //QT_PROGRAMMING_2024_RAYGUN_H

