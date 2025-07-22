//
// Created by guangnianye on 7/19/25
//

#ifndef QT_PROGRAMMING_2024_WEAPON_H
#define QT_PROGRAMMING_2024_WEAPON_H

#include "../Item.h"

enum class WeaponType {
    Melee,
    Ranged
};

class Weapon : public Item {
public:
    explicit Weapon(QGraphicsItem *parent, const QString &pixmapPath);

    // 武器的攻击方法
    virtual void attack() = 0;

    // 设置武器的伤害值
    void setDamage(qreal damage) {
        this->damage = damage;
    }

    // 获取武器的伤害值
    [[nodiscard]] qreal getDamage() const {
        return damage;
    }

    // 设置武器的攻击范围
    void setAttackRange(qreal range) {  
        this->attackRange = range;
    }

    // 获取武器的攻击范围
    [[nodiscard]] qreal getAttackRange() const {
        return attackRange;
    }

    // 设置武器名称
    void setWeaponname(const QString& type) {
        this->weaponname = type;
    }
    // 获取武器类型名称
    [[nodiscard]] QString getWeaponname() const {
        return weaponname;
    }

private:
    qreal damage{10.0};        // 默认伤害值
    QString weaponname{"Unknown"}; // 武器类型
    qreal attackRange{50.0};   // 攻击范围，默认值为50.0

public:
    WeaponType type{WeaponType::Melee}; // 武器类型枚举
};

#endif //QT_PROGRAMMING_2024_WEAPON_H
