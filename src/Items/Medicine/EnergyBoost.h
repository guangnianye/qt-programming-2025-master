//
// Created by user on 7/24/25
//

#ifndef QT_PROGRAMMING_2024_ENERGYBOOST_H
#define QT_PROGRAMMING_2024_ENERGYBOOST_H

#include "Medicine.h"
#include "../Characters/BuffSystem.h"

// 能量增益药水类 - 增加移动速度并持续回血
class EnergyBoost : public Medicine {
public:
    explicit EnergyBoost(QGraphicsItem *parent = nullptr);
    virtual ~EnergyBoost() = default;

    // 重写基类方法
    qreal getEffectValue() const override;
    int getEffectDuration() const override;
    QString getDescription() const override;

protected:
    // 实现具体的药物效果
    void applyMedicineEffect(Character* character) override;

private:
    static constexpr qreal SPEED_BOOST_MULTIPLIER = 1.5;  // 速度增益倍数
    static constexpr qreal HEALTH_REGEN_PER_TICK = 1.0;   // 每秒回复的血量
    static constexpr int EFFECT_DURATION = 25000;         // 持续时间（25秒）
    static constexpr int REGEN_TICK_INTERVAL = 1000;      // 回血间隔（1秒）
};

#endif //QT_PROGRAMMING_2024_ENERGYBOOST_H
