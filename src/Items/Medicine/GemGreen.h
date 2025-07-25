//
// Created by user on 7/25/25
//

#ifndef QT_PROGRAMMING_2024_GEMGREEN_H
#define QT_PROGRAMMING_2024_GEMGREEN_H

#include "Medicine.h"
#include "../Characters/BuffSystem.h"

// 绿宝石药物类 - 对近战武器和投掷石无效果，免疫75%的raygun和raygunbig伤害，总共能吸收200点伤害
class GemGreen : public Medicine {
public:
    explicit GemGreen(QGraphicsItem *parent = nullptr);
    virtual ~GemGreen() = default;

    // 重写基类方法
    qreal getEffectValue() const override;
    int getEffectDuration() const override;
    QString getDescription() const override;
    QString getTypeName() const override { return "GemGreen"; }

protected:
    // 实现具体的药物效果
    void applyMedicineEffect(Character* character) override;

private:
    static constexpr qreal SHIELD_CAPACITY = 200.0;        // 护盾总容量
    static constexpr int PERMANENT_DURATION = -1;          // 永久效果（直到护盾耗尽）
};

#endif //QT_PROGRAMMING_2024_GEMGREEN_H
