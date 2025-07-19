//
// Created by guangnianye on 7/19/25
//

#include "Weapon.h"

Weapon::Weapon(QGraphicsItem *parent, const QString &pixmapPath) 
    : Item(parent, pixmapPath) {
    // 武器类的构造函数实现
    // 设置默认武器类型
    weaponType = "Basic Weapon";
}

