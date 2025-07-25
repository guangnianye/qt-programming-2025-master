//
// Created by user on 7/24/25
//

#ifndef QT_PROGRAMMING_2024_BUFFSYSTEM_H
#define QT_PROGRAMMING_2024_BUFFSYSTEM_H

#include <QString>
#include <QMap>

// 武器防护结构
struct WeaponProtection {
    QString weaponName;       // 武器名称（如 "fist", "sword"）
    qreal damageMultiplier;   // 伤害倍数（0.0 = 免疫，0.5 = 50%减免，1.0 = 正常伤害）
    
    WeaponProtection() = default;
    WeaponProtection(const QString& name, qreal multiplier) 
        : weaponName(name), damageMultiplier(multiplier) {}
};

// 增益效果结构
struct BuffEffect {
    QString name;             // 增益名称
    qreal speedMultiplier;    // 速度倍数（1.0 = 正常速度，1.5 = 1.5倍速度）
    qreal healthRegenRate;    // 血量回复速度（每秒）
    int duration;             // 持续时间（毫秒）
    int tickInterval;         // 触发间隔（毫秒，用于持续性效果如回血）
    QMap<QString, qreal> weaponProtections; // 武器防护映射：武器名 -> 伤害倍数
    qreal damageShield;       // 伤害护盾值（能吸收的总伤害量）
    qreal maxDamageShield;    // 最大护盾值（用于显示护盾百分比）
    
    // 默认构造函数
    BuffEffect() : damageShield(0.0), maxDamageShield(0.0) {}

    // 参数构造函数
    BuffEffect(const QString& n, qreal sm = 1.0, qreal hrr = 0.0, int d = 0, int ti = 1000)
        : name(n), speedMultiplier(sm), healthRegenRate(hrr), duration(d), tickInterval(ti), 
          damageShield(0.0), maxDamageShield(0.0) {}
        
    // 添加武器防护
    void addWeaponProtection(const QString& weaponName, qreal damageMultiplier) {
        weaponProtections[weaponName] = damageMultiplier;
    }
    
    // 获取对特定武器的防护倍数
    qreal getWeaponProtection(const QString& weaponName) const {
        return weaponProtections.value(weaponName, 1.0); // 默认返回1.0（无防护）
    }
    
    // 设置伤害护盾
    void setDamageShield(qreal shieldValue) {
        damageShield = shieldValue;
        maxDamageShield = shieldValue;
    }
    
    // 消耗护盾值，返回剩余需要承受的伤害
    qreal consumeShield(qreal damage) {
        if (damageShield <= 0) {
            return damage; // 护盾已耗尽，全部伤害穿透
        }
        
        if (damage <= damageShield) {
            damageShield -= damage;
            return 0.0; // 护盾完全吸收伤害
        } else {
            qreal remainingDamage = damage - damageShield;
            damageShield = 0.0;
            return remainingDamage; // 护盾部分吸收，剩余伤害穿透
        }
    }
    
    // 检查护盾是否耗尽
    bool isShieldDepleted() const {
        return damageShield <= 0 && maxDamageShield > 0;
    }
    
    // 获取护盾剩余百分比
    qreal getShieldPercentage() const {
        if (maxDamageShield <= 0) return 0.0;
        return (damageShield / maxDamageShield) * 100.0;
    }
};

#endif //QT_PROGRAMMING_2024_BUFFSYSTEM_H
