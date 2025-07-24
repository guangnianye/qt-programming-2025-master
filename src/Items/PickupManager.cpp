//
// Created by user on 7/24/25
//

#include "PickupManager.h"
#include "Characters/Character.h"
#include "Weapons/Weapon.h"
#include "Weapons/WeaponManager.h"
#include "Medicine/Medicine.h"
#include "Medicine/MedicineManager.h"
#include "../Physics/PhysicsConstants.h"
#include <QLineF>
#include <QGraphicsScene>

PickupManager::PickupManager(QGraphicsScene* scene, QObject* parent)
    : QObject(parent), gameScene(scene), weaponManager(nullptr), medicineManager(nullptr) {
}

PickupManager::~PickupManager() = default;

bool PickupManager::handlePickup(Character* character) {
    if (!character || !gameScene) return false;
    
    // 获取最近的可拾取物品
    QGraphicsItem* closestItem = getClosestPickableItem(character);
    if (!closestItem) return false;
    
    // 检查距离
    qreal distance = calculateDistance(character->pos(), closestItem->pos());
    if (distance > PhysicsConstants::PICKUP_DISTANCE) return false;
    
    // 根据物品类型进行处理
    if (Weapon* weapon = dynamic_cast<Weapon*>(closestItem)) {
        return handleWeaponPickup(character, weapon);
    }
    else if (Medicine* medicine = dynamic_cast<Medicine*>(closestItem)) {
        return handleMedicinePickup(character, medicine);
    }
    else if (Pickable* pickable = dynamic_cast<Pickable*>(closestItem)) {
        return handlePickableItemPickup(character, pickable);
    }
    
    return false;
}

QVector<QGraphicsItem*> PickupManager::findPickableItems(Character* character) const {
    QVector<QGraphicsItem*> pickableItems;
    
    if (!gameScene || !character) return pickableItems;
    
    // 获取角色周围的所有物品
    QPointF characterPos = character->pos();
    QRectF searchArea(characterPos.x() - PhysicsConstants::PICKUP_DISTANCE,
                      characterPos.y() - PhysicsConstants::PICKUP_DISTANCE,
                      PhysicsConstants::PICKUP_DISTANCE * 2,
                      PhysicsConstants::PICKUP_DISTANCE * 2);
    
    QList<QGraphicsItem*> itemsInArea = gameScene->items(searchArea);
    
    // 筛选出可拾取的物品
    for (QGraphicsItem* item : itemsInArea) {
        if (item != character && isPickableItem(item)) {
            pickableItems.append(item);
        }
    }
    
    return pickableItems;
}

QGraphicsItem* PickupManager::getClosestPickableItem(Character* character) const {
    QVector<QGraphicsItem*> pickableItems = findPickableItems(character);
    
    if (pickableItems.isEmpty()) return nullptr;
    
    QGraphicsItem* closestItem = nullptr;
    qreal closestDistance = std::numeric_limits<qreal>::max();
    
    QPointF characterPos = character->pos();
    
    for (QGraphicsItem* item : pickableItems) {
        qreal distance = calculateDistance(characterPos, item->pos());
        if (distance < closestDistance) {
            closestDistance = distance;
            closestItem = item;
        }
    }
    
    return closestItem;
}

bool PickupManager::isPickableItem(QGraphicsItem* item) {
    if (!item) return false;
    
    // 检查是否是武器
    if (dynamic_cast<Weapon*>(item)) return true;
    
    // 检查是否是药物
    if (dynamic_cast<Medicine*>(item)) return true;
    
    // 检查是否是其他可拾取物品
    if (dynamic_cast<Pickable*>(item)) return true;
    
    return false;
}

qreal PickupManager::calculateDistance(const QPointF& pos1, const QPointF& pos2) {
    return QLineF(pos1, pos2).length();
}

bool PickupManager::handleWeaponPickup(Character* character, Weapon* weapon) {
    if (!weaponManager) return false;
    
    // 检查武器是否已经被装备
    if (weapon->parentItem() != nullptr) return false;
    
    // 使用武器管理器处理武器拾取
    Weapon* replacedWeapon = WeaponManager::handleWeaponPickup(character, weapon);
    
    // 注意：不需要手动从场景移除武器，因为setParentItem会自动处理
    // weapon->scene()->removeItem(weapon); // 这行是错误的！
    
    // 如果有被替换的武器，删除它
    if (replacedWeapon && replacedWeapon != weapon) {
        delete replacedWeapon;
    }
    
    return true;
}

bool PickupManager::handleMedicinePickup(Character* character, Medicine* medicine) {
    if (!medicineManager) return false;
    
    // 使用药物管理器处理药物拾取
    return MedicineManager::handleMedicinePickup(character, medicine);
}

bool PickupManager::handlePickableItemPickup(Character* character, Pickable* pickable) {
    if (!pickable) return false;
    
    // 应用物品效果
    pickable->applyEffect(character);
    
    // 从场景中移除物品
    if (pickable->scene()) {
        pickable->scene()->removeItem(pickable);
    }
    
    // 删除物品对象
    delete pickable;
    
    return true;
}
