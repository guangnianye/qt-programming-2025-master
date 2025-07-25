//
// Created by user on 7/25/25
//

#include "GemGreen.h"
#include "../Characters/Character.h"
#include <QDebug>

GemGreen::GemGreen(QGraphicsItem *parent) 
    : Medicine(parent, ":/Items/Medicine/gem_green.png", MedicineEffectType::Special) {
    typeName = "GemGreen";
    description = "绿宝石：减免75%远程武器伤害，护盾容量200点";
}

qreal GemGreen::getEffectValue() const {
    return SHIELD_CAPACITY; // 表示护盾容量
}

int GemGreen::getEffectDuration() const {
    return PERMANENT_DURATION; // 永久效果（直到护盾耗尽）
}

QString GemGreen::getDescription() const {
    return description;
}

void GemGreen::applyMedicineEffect(Character* character) {
    if (!character) return;
    
    // 检查是否已经装备了其他宝石，如果有则移除其效果
    if (character->hasGem()) {
        Medicine* oldGem = character->getCurrentGem();
        if (oldGem) {
            QString oldGemName = oldGem->getTypeName();
            // 移除旧宝石的buff效果
            if (oldGemName == "GemBlue") {
                character->removeBuff("蓝宝石防护");
            } else if (oldGemName == "GemGreen") {
                character->removeBuff("绿宝石护盾");
            }
            qDebug() << "Removed old gem:" << oldGemName << "and its effects";
        }
    }
    
    // 装备新宝石（这会自动卸下旧宝石）
    // 注意：这里需要创建一个新的GemGreen实例用于显示，因为原来的会被删除
    GemGreen* displayGem = new GemGreen();
    character->equipGem(displayGem);
    
    // 创建远程武器护盾buff
    BuffEffect shieldBuff;
    shieldBuff.name = "绿宝石护盾";
    shieldBuff.speedMultiplier = 1.0;     // 不影响速度
    shieldBuff.healthRegenRate = 0.0;     // 不回血
    shieldBuff.duration = 0;              // 永久效果（duration为0表示不会自动移除，但护盾耗尽时会被手动移除）
    shieldBuff.tickInterval = 0;          // 不需要周期性触发
    
    // 设置护盾容量
    shieldBuff.setDamageShield(SHIELD_CAPACITY);
    
    // 对于绿宝石，我们使用特殊的武器标记来表示这是一个纯护盾效果
    // 设置为特殊值1.0表示没有额外的倍数减免，完全依靠护盾吸收
    shieldBuff.addWeaponProtection("raygun", 1.0);
    shieldBuff.addWeaponProtection("raygunbig", 1.0);
    // 注意：不添加对"fist", "sword", "throwingstone"的防护，这样它们就不会被护盾影响
    
    // 应用buff到角色
    character->applyBuff(shieldBuff);
    
    qDebug() << "GemGreen applied to character: Shield protection for ranged weapons with" 
             << SHIELD_CAPACITY << "shield capacity";
}
