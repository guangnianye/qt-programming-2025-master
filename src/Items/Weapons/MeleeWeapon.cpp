//
// Created by guangnianye on 7/21/25
//

#include "MeleeWeapon.h"
#include <QDebug>

MeleeWeapon::MeleeWeapon(QGraphicsItem *parent, const QString &pixmapPath)
    : Weapon(parent, pixmapPath) {
    // 近战武器的默认设置

}

void MeleeWeapon::attack() {
    qDebug() << "MeleeWeapon attack initiated";
    performMeleeAttack();
}

void MeleeWeapon::performMeleeAttack() {
    // 简单的近战攻击逻辑
    qDebug() << "Melee attack executed!"
             << "Damage:" << getDamage()
             << "Range:" << getAttackRange();
    
    // 这里可以添加近战攻击的特效
    // 比如挥砍动画、音效等
}
