//
// Created by user on 7/24/25
//

#include "Medicine.h"
#include "../Characters/Character.h"

Medicine::Medicine(QGraphicsItem *parent, const QString &pixmapPath, 
                   MedicineEffectType effectType)
    : Pickable(parent, pixmapPath), effectType(effectType), effectValue(0), effectDuration(0) {
    typeName = "Medicine";
}

void Medicine::applyEffect(Character* character) {
    if (!character) return;
    
    // 调用具体的药物效果
    applyMedicineEffect(character);
    
    // 播放拾取音效（如果实现了）
    playPickupSound();
}

QString Medicine::getDescription() const {
    return description;
}
