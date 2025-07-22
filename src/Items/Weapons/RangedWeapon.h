//
// Created by guangnianye on 7/21/25
//

#ifndef QT_PROGRAMMING_2024_RANGEDWEAPON_H
#define QT_PROGRAMMING_2024_RANGEDWEAPON_H

#include "Weapon.h"
#include <QPointF>

class RangedWeapon : public Weapon {
public:
    explicit RangedWeapon(QGraphicsItem *parent, const QString &pixmapPath);

    // 远程武器的攻击方法
    void attack() override;

    // 水平远程攻击
    void attack(const QPointF& targetPosition);

    // 获取剩余投掷次数
    int getRemainingbullets() const { return remainingbullet; }
    
    // 检查武器是否还可用
    bool canUse() const { return remainingbullet > 0; }

protected:
    // 远程武器特有的攻击逻辑
    virtual void performRangedAttack(const QPointF& direction);
    int remainingbullet{10};
};

#endif //QT_PROGRAMMING_2024_RANGEDWEAPON_H