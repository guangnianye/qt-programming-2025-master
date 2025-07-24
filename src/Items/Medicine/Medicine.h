//
// Created by user on 7/24/25
//

#ifndef QT_PROGRAMMING_2024_MEDICINE_H
#define QT_PROGRAMMING_2024_MEDICINE_H

#include "../Pickable.h"

// 药物效果类型枚举
enum class MedicineEffectType {
    Healing,        // 治疗类
    Buff,          // 增益类
    Debuff,        // 减益类（未来可能用于毒药等）
    Special        // 特殊效果类
};

// 药物基类
class Medicine : public Pickable {
public:
    explicit Medicine(QGraphicsItem *parent, const QString &pixmapPath, 
                      MedicineEffectType effectType);
    virtual ~Medicine() = default;

    // 继承自Pickable的纯虚函数
    void applyEffect(Character* character) override;
    QString getDescription() const override;
    QString getTypeName() const override { return "Medicine"; }

    // 药物特有的方法
    virtual qreal getEffectValue() const = 0;
    virtual int getEffectDuration() const { return 0; } // 持续时间（毫秒），0表示即时效果
    
    MedicineEffectType getEffectType() const { return effectType; }

protected:
    // 具体的药物效果实现，由子类重写
    virtual void applyMedicineEffect(Character* character) = 0;
    
    MedicineEffectType effectType;
    qreal effectValue;        // 效果数值
    int effectDuration;       // 效果持续时间（毫秒）
};

#endif //QT_PROGRAMMING_2024_MEDICINE_H
