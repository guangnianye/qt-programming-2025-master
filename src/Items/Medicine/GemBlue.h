//
// Created by user on 7/25/25
//

#ifndef QT_PROGRAMMING_2024_GEMBLUE_H
#define QT_PROGRAMMING_2024_GEMBLUE_H

#include "Medicine.h"
#include "../Characters/BuffSystem.h"

// 蓝宝石药物类 - 拾取后永久免疫拳头攻击，降低50%剑攻击，对远程武器无效果
class GemBlue : public Medicine {
public:
    explicit GemBlue(QGraphicsItem *parent = nullptr);
    virtual ~GemBlue() = default;

    // 重写基类方法
    qreal getEffectValue() const override;
    int getEffectDuration() const override;
    QString getDescription() const override;
    QString getTypeName() const override { return "GemBlue"; }

protected:
    // 实现具体的药物效果
    void applyMedicineEffect(Character* character) override;

private:
    static constexpr qreal FIST_IMMUNITY = 0.0;      // 拳头完全免疫
    static constexpr qreal SWORD_REDUCTION = 0.5;    // 剑伤害减免50%
    static constexpr int PERMANENT_DURATION = -1;    // 永久效果
};

#endif //QT_PROGRAMMING_2024_GEMBLUE_H
