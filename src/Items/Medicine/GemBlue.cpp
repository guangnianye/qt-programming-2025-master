//
// Created by user on 7/25/25
//

#include "GemBlue.h"
#include "../Characters/Character.h"
#include <QDebug>

GemBlue::GemBlue(QGraphicsItem *parent) 
    : Medicine(parent, ":/Items/Medicine/gem_blue.png", MedicineEffectType::Special) {
    typeName = "GemBlue";
    description = "蓝宝石：永久免疫拳头攻击，减免50%剑攻击伤害";
}

qreal GemBlue::getEffectValue() const {
    return 1.0; // 表示防护效果的基础值
}

int GemBlue::getEffectDuration() const {
    return PERMANENT_DURATION; // 永久效果
}

QString GemBlue::getDescription() const {
    return description;
}

void GemBlue::applyMedicineEffect(Character* character) {
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
    // 注意：这里需要创建一个新的GemBlue实例用于显示，因为原来的会被删除
    GemBlue* displayGem = new GemBlue();
    character->equipGem(displayGem);
    
    // 创建永久防护buff
    BuffEffect protectionBuff;
    protectionBuff.name = "蓝宝石防护";
    protectionBuff.speedMultiplier = 1.0;     // 不影响速度
    protectionBuff.healthRegenRate = 0.0;     // 不回血
    protectionBuff.duration = 0;              // 永久效果（duration为0表示不会自动移除）
    protectionBuff.tickInterval = 0;          // 不需要周期性触发
    
    // 添加武器防护：拳头完全免疫，剑减免50%
    protectionBuff.addWeaponProtection("fist", FIST_IMMUNITY);
    protectionBuff.addWeaponProtection("sword", SWORD_REDUCTION);
    
    // 应用buff到角色
    character->applyBuff(protectionBuff);
    
    qDebug() << "GemBlue applied to character: Fist immunity and 50% sword damage reduction";
}
