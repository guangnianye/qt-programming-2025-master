//
// Created by guangnianye on 7/19/25
//

#include "Fist.h"
#include <QDebug>

Fist::Fist(QGraphicsItem *parent) 
    : MeleeWeapon(parent, "") {  // 拳头不需要图像资源
    // 设置拳头的属性
    setDamage(5.0);  // 拳头伤害较低
    setAttackRange(30.0);  // 拳头攻击范围较短
    setWeaponname("fist");
    type = WeaponType::Melee;  // 拳头属于近战武器
}

void Fist::attack() {
    // 拳头攻击逻辑
    qDebug() << "Fist attack! Damage:" << getDamage() << "Range:" << getAttackRange();
    
    // 这里可以添加拳头特有的攻击效果
    // 比如音效、粒子效果等
}
