//
// Created by user on 7/24/25
//

#ifndef QT_PROGRAMMING_2024_HEALTHPOTION_H
#define QT_PROGRAMMING_2024_HEALTHPOTION_H

#include "Medicine.h"

// 健康药水类 - 拾取后立即恢复25血量
class HealthPotion : public Medicine {
public:
    explicit HealthPotion(QGraphicsItem *parent = nullptr);
    virtual ~HealthPotion() = default;

    // 重写基类方法
    qreal getEffectValue() const override;
    QString getDescription() const override;

protected:
    // 实现具体的药物效果
    void applyMedicineEffect(Character* character) override;

private:
    static constexpr qreal HEALING_AMOUNT = 25.0;  // 恢复的血量
};

#endif //QT_PROGRAMMING_2024_HEALTHPOTION_H
