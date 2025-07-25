//
// Created by user on 7/24/25
//

#include "EnergyBoost.h"
#include "../Characters/Character.h"

EnergyBoost::EnergyBoost(QGraphicsItem *parent)
    : Medicine(parent, ":/Items/Medicine/mushroom.png", MedicineEffectType::Buff) {
    
    // 设置药物属性
    effectValue = SPEED_BOOST_MULTIPLIER;
    effectDuration = EFFECT_DURATION;
    description = QString("Energy Boost: +%1% speed and %2 HP/sec for %3 seconds")
                     .arg((SPEED_BOOST_MULTIPLIER - 1.0) * 100, 0, 'f', 0)
                     .arg(HEALTH_REGEN_PER_TICK, 0, 'f', 0)
                     .arg(EFFECT_DURATION / 1000);
}

qreal EnergyBoost::getEffectValue() const {
    return SPEED_BOOST_MULTIPLIER;
}

int EnergyBoost::getEffectDuration() const {
    return EFFECT_DURATION;
}

QString EnergyBoost::getDescription() const {
    return description;
}

void EnergyBoost::applyMedicineEffect(Character* character) {
    if (!character) {
        return;
    }
    
    // 创建增益效果
    BuffEffect energyBuff(
        "Energy Boost",           // 名称
        SPEED_BOOST_MULTIPLIER,   // 速度倍数
        HEALTH_REGEN_PER_TICK,    // 血量回复速度
        EFFECT_DURATION,          // 持续时间
        REGEN_TICK_INTERVAL       // 触发间隔
    );
    
    // 应用增益效果到角色
    character->applyBuff(energyBuff);
}
