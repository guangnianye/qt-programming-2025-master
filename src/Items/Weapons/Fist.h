//
// Created by guangnianye on 7/19/25
//

#ifndef QT_PROGRAMMING_2024_FIST_H
#define QT_PROGRAMMING_2024_FIST_H

#include "MeleeWeapon.h"

class Fist : public MeleeWeapon {
public:
    explicit Fist(QGraphicsItem *parent);

    // 实现纯虚函数：拳头攻击
    void attack() override;

private:
    // 拳头特有的属性可以在这里添加
};

#endif //QT_PROGRAMMING_2024_FIST_H
