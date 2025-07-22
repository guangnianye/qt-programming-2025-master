//
// Created by guangnianye on 7/21/25
//

#ifndef QT_PROGRAMMING_2024_MELEEWEAPON_H
#define QT_PROGRAMMING_2024_MELEEWEAPON_H

#include "Weapon.h"

class MeleeWeapon : public Weapon {
public:
    explicit MeleeWeapon(QGraphicsItem *parent, const QString &pixmapPath);

    // 近战武器的通用攻击方法（可以被子类重写）
    void attack() override;

protected:
    // 近战武器特有的攻击逻辑
    virtual void performMeleeAttack();
};

#endif //QT_PROGRAMMING_2024_MELEEWEAPON_H
