//
// Created by user on 7/24/25
//

#include "MagicPotion.h"
#include "../Characters/Character.h"

MagicPotion::MagicPotion(QGraphicsItem *parent)
    : Medicine(parent, ":/Items/Medicine/full_heart.png", MedicineEffectType::Healing) {
    
    // 设置药物属性
    effectValue = 0; // 回满血没有固定数值
    effectDuration = 0; // 即时效果
    description = "Magic Potion: Instantly restores full health";
    
}

qreal MagicPotion::getEffectValue() const {
    // 返回 0，因为这是回满血，具体数值取决于角色的最大血量
    return 0;
}

QString MagicPotion::getDescription() const {
    return "Magic Potion: Instantly restores full health";
}

void MagicPotion::applyMedicineEffect(Character* character) {
    if (!character) {
        return;
    }
    
    // 获取角色的最大血量和当前血量
    qreal maxHealth = character->getMaxHealth();
    qreal currentHealth = character->getCurrentHealth();
    
    // 计算需要回复的血量
    qreal healAmount = maxHealth - currentHealth;
    
    // 回满血
    character->heal(healAmount);
}
