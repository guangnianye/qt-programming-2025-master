//
// Created by user on 7/24/25
//

#ifndef QT_PROGRAMMING_2024_PICKUPMANAGER_H
#define QT_PROGRAMMING_2024_PICKUPMANAGER_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QVector>

class Character;
class Weapon;
class Medicine;
class Pickable;
class WeaponManager;
class MedicineManager;

// 统一的拾取管理器
class PickupManager : public QObject {
    Q_OBJECT

public:
    explicit PickupManager(QGraphicsScene* scene, QObject* parent = nullptr);
    virtual ~PickupManager();
    
    // 设置武器和药物管理器
    void setWeaponManager(WeaponManager* weaponMgr) { weaponManager = weaponMgr; }
    void setMedicineManager(MedicineManager* medicineMgr) { medicineManager = medicineMgr; }
    
    // 处理角色拾取操作
    bool handlePickup(Character* character);
    
    // 查找角色周围的可拾取物品
    QVector<QGraphicsItem*> findPickableItems(Character* character) const;
    
    // 获取最近的可拾取物品
    QGraphicsItem* getClosestPickableItem(Character* character) const;
    
    // 检查物品是否可拾取
    static bool isPickableItem(QGraphicsItem* item);
    
    // 计算两点间距离
    static qreal calculateDistance(const QPointF& pos1, const QPointF& pos2);

private:
    QGraphicsScene* gameScene;
    WeaponManager* weaponManager;
    MedicineManager* medicineManager;
    
    // 处理武器拾取
    bool handleWeaponPickup(Character* character, Weapon* weapon);
    
    // 处理药物拾取
    bool handleMedicinePickup(Character* character, Medicine* medicine);
    
    // 处理通用可拾取物品
    bool handlePickableItemPickup(Character* character, Pickable* pickable);
};

#endif //QT_PROGRAMMING_2024_PICKUPMANAGER_H
