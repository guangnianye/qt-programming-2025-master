//
// Created by user on 7/24/25
//

#ifndef QT_PROGRAMMING_2024_MAGICPOTION_H
#define QT_PROGRAMMING_2024_MAGICPOTION_H

#include "Medicine.h"

// 魔法药水类 - 瞬间回满血
class MagicPotion : public Medicine {
public:
    explicit MagicPotion(QGraphicsItem *parent = nullptr);
    virtual ~MagicPotion() = default;

    // 重写基类方法
    qreal getEffectValue() const override;
    QString getDescription() const override;

protected:
    // 实现具体的药物效果
    void applyMedicineEffect(Character* character) override;
};

#endif //QT_PROGRAMMING_2024_MAGICPOTION_H
