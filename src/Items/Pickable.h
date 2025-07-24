//
// Created by user on 7/24/25
//

#ifndef QT_PROGRAMMING_2024_PICKABLE_H
#define QT_PROGRAMMING_2024_PICKABLE_H

#include "Item.h"

// 可拾取物品的基类
class Pickable : public Item {
public:
    explicit Pickable(QGraphicsItem *parent, const QString &pixmapPath);
    virtual ~Pickable() = default;

    // 纯虚函数，子类必须实现使用效果
    virtual void applyEffect(class Character* character) = 0;
    
    // 获取物品描述
    virtual QString getDescription() const = 0;
    
    // 获取物品类型名称
    virtual QString getTypeName() const = 0;
    
    // 是否可以被拾取（默认为true，某些情况下可能被禁用）
    virtual bool canBePickedUp() const { return true; }
    
    // 拾取时的音效（可选，留给子类实现）
    virtual void playPickupSound() const {}

protected:
    // 物品描述
    QString description;
    
    // 物品类型名称
    QString typeName;
};

#endif //QT_PROGRAMMING_2024_PICKABLE_H
