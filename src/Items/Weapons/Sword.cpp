//
// Created by guangnianye on 7/22/25
//

#include "Sword.h"
#include <QDebug>

Sword::Sword(QGraphicsItem *parent) 
    : MeleeWeapon(parent, ":/Items/Weapons/Sword/Sword.png") {  // 使用剑的图像资源
    // 设置剑的属性
    setDamage(15.0);  // 剑伤害较高，比拳头(5.0)高
    setAttackRange(50.0);  // 剑攻击范围比拳头(30.0)长
    setWeaponname("sword");
    type = WeaponType::Melee;  // 剑属于近战武器
    
}

void Sword::attack() {
    // 剑攻击逻辑
    qDebug() << "Sword attack! Damage:" << getDamage() << "Range:" << getAttackRange();
    
    // 这里可以添加剑特有的攻击效果
    // 比如挥砍动画、音效、粒子效果等
}
