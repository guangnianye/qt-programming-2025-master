//
// Created by user on 7/24/25
//

#include "HealthPotion.h"
#include "../Characters/Character.h"
#include <QDebug>

HealthPotion::HealthPotion(QGraphicsItem *parent)
    : Medicine(parent, ":/Items/Medicine/half_heart.png", MedicineEffectType::Healing) {
    
    // 设置药物属性
    effectValue = HEALING_AMOUNT;
    effectDuration = 0; // 即时效果
    description = QString("Health Potion: Restores %1 health points").arg(HEALING_AMOUNT);
    
}

qreal HealthPotion::getEffectValue() const {
    return HEALING_AMOUNT;
}

QString HealthPotion::getDescription() const {
    return description;
}

void HealthPotion::applyMedicineEffect(Character* character) {
    if (!character) {
        qDebug() << "HealthPotion: Invalid character pointer";
        return;
    }
    
    // 恢复角色血量
    qreal healedAmount = character->getCurrentHealth();
    character->heal(HEALING_AMOUNT);
    healedAmount = character->getCurrentHealth() - healedAmount;
    
    qDebug() << "HealthPotion used! Character healed" << healedAmount << "points."
             << "Current health:" << character->getCurrentHealth() << "/" << character->getMaxHealth();
}
