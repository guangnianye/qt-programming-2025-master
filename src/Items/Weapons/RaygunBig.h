//
// Created by user on 7/23/25
//

#ifndef QT_PROGRAMMING_2024_RAYGUNBIG_H
#define QT_PROGRAMMING_2024_RAYGUNBIG_H

#include "RangedWeapon.h"
#include "Projectile.h"
#include <QElapsedTimer>

// 大型激光子弹投射物类
class BigLaserBullet : public Projectile {
    Q_OBJECT
public:
    explicit BigLaserBullet(const QPointF& startPos, const QPointF& direction, qreal damage, QGraphicsItem *parent = nullptr, Character* shooter = nullptr);

protected:
    // 重写角色碰撞处理
    void onCharacterHit(Character* character) override;

private:
    static constexpr qreal BULLET_SPEED = 600.0; // 子弹速度（比普通raygun慢一些）
};

// 大型激光枪武器类
class RaygunBig : public RangedWeapon {
public:
    explicit RaygunBig(QGraphicsItem *parent);
    
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
    static constexpr int MAX_BULLETS = 3;       // 最大子弹数（较少）
    static constexpr qint64 SHOOT_INTERVAL = 800; // 射击间隔（毫秒）（较长）
    static constexpr qreal BULLET_SPEED = 600.0; // 子弹速度
    static constexpr qreal HIGH_DAMAGE = 50.0;  // 高伤害
    
    QElapsedTimer lastShootTime;    // 记录上次射击时间
    
    // 创建大型子弹投射物
    BigLaserBullet* createBigBullet(const QPointF& direction);
    
    // 获取角色面向方向
    QPointF getCharacterDirection() const;
};

#endif //QT_PROGRAMMING_2024_RAYGUNBIG_H
