//
// Created by guangnianye on 7/22/25
//

#ifndef QT_PROGRAMMING_2024_SWORD_H
#define QT_PROGRAMMING_2024_SWORD_H

#include "MeleeWeapon.h"

class Sword : public MeleeWeapon {
public:
    explicit Sword(QGraphicsItem *parent);

    // 实现纯虚函数：剑攻击
    void attack() override;

private:
    // 剑特有的属性可以在这里添加
};

#endif //QT_PROGRAMMING_2024_SWORD_H
