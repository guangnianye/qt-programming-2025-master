//
// Created by guangnianye on 7/21/25
//

#ifndef QT_PROGRAMMING_2024_THROWINGSTONE_H
#define QT_PROGRAMMING_2024_THROWINGSTONE_H

#include "RangedWeapon.h"
#include "Projectile.h"

// 投掷石头的飞行物体类
class StoneProjectile : public Projectile {
    Q_OBJECT
public:
    explicit StoneProjectile(const QPointF& startPos, const QPointF& direction, qreal damage, QGraphicsItem *parent = nullptr);

protected:
    // 重写更新位置方法，实现抛物线运动
    void updatePosition() override;
    
    // 重写角色碰撞处理
    void onCharacterHit(Character* character) override;

private:
    static constexpr qreal THROW_ANGLE = 30.0;  // 投掷角度（度）
    static constexpr qreal INITIAL_SPEED = 400.0; // 初始速度
};

// 投掷石头武器类
class ThrowingStone : public RangedWeapon {
public:
    explicit ThrowingStone(QGraphicsItem *parent);
    
    // 重写攻击方法
    void attack() override;
    
    // 朝指定方向投掷
    void throwStone(const QPointF& direction);

protected:
    // 重写远程攻击逻辑
    void performRangedAttack(const QPointF& direction) override;

private:
    static constexpr int MAX_THROWS = 10;   // 最大投掷次数
    static constexpr qreal THROW_FORCE = 400.0; // 投掷力度
    static constexpr qreal THROW_ANGLE = 30.0;  // 投掷角度（度）
    
    // 创建投射物
    StoneProjectile* createProjectile(const QPointF& direction);
};

#endif //QT_PROGRAMMING_2024_THROWINGSTONE_H
