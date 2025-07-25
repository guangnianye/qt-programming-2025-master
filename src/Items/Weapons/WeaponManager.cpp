//
// Created by user on 7/23/25
//

#include "WeaponManager.h"
#include "../Characters/Character.h"
#include "ThrowingStone.h"
#include "Sword.h"
#include "Raygun.h"
#include "RaygunBig.h"
#include "RangedWeapon.h"
#include "Fist.h"

WeaponManager::WeaponManager(QGraphicsScene* scene, QObject* parent)
    : QObject(parent), gameScene(scene), dropInterval(DEFAULT_DROP_INTERVAL) {
    
    // 初始化掉落计时器
    dropTimer = new QTimer(this);
    connect(dropTimer, &QTimer::timeout, this, &WeaponManager::dropRandomWeapon);
    
    // 设置默认掉落区域（整个场景宽度）
    if (gameScene) {
        dropArea = QRectF(gameScene->sceneRect().left(), 
                         gameScene->sceneRect().top() + DROP_HEIGHT_OFFSET,
                         gameScene->sceneRect().width(), 
                         50); // 掉落带高度
    }
}

WeaponManager::~WeaponManager() {
    stopWeaponDrops();
}

void WeaponManager::startWeaponDrops() {
    if (dropTimer && !dropTimer->isActive()) {
        dropTimer->start(dropInterval);
    }
}

void WeaponManager::stopWeaponDrops() {
    if (dropTimer && dropTimer->isActive()) {
        dropTimer->stop();
    }
}

void WeaponManager::dropWeapon(WeaponDropType weaponType, const QPointF& position) {
    if (!gameScene) {
        return;
    }
    
    // 创建武器
    Weapon* weapon = createWeapon(weaponType);
    if (weapon) {
        // 设置位置并添加到场景
        weapon->setPos(position);
        gameScene->addItem(weapon);
    }
}

void WeaponManager::dropRandomWeapon() {
    // 获取随机武器类型和位置
    WeaponDropType weaponType = getRandomWeaponType();
    QPointF dropPosition = getRandomDropPosition();
    
    // 掉落武器
    dropWeapon(weaponType, dropPosition);
}

Weapon* WeaponManager::createWeapon(WeaponDropType weaponType, QGraphicsItem* parent) {
    switch (weaponType) {
        case WeaponDropType::ThrowingStone:
            return new ThrowingStone(parent);
        case WeaponDropType::Sword:
            return new Sword(parent);
        case WeaponDropType::Raygun:
            return new Raygun(parent);
        case WeaponDropType::RaygunBig:
            return new RaygunBig(parent);
        default:
            return new Sword(parent);
    }
}

WeaponDropType WeaponManager::getRandomWeaponType() {
    // 定义武器掉落权重（可以调整各种武器的掉落概率）
    QVector<WeaponDropType> weaponPool = {
        WeaponDropType::ThrowingStone,  // 20% 几率
        WeaponDropType::ThrowingStone,
        WeaponDropType::Sword,          // 30% 几率
        WeaponDropType::Sword,
        WeaponDropType::Sword,
        WeaponDropType::Raygun,         // 30% 几率
        WeaponDropType::Raygun,
        WeaponDropType::Raygun,
        WeaponDropType::RaygunBig,      // 20% 几率
        WeaponDropType::RaygunBig
    };
    
    int randomIndex = QRandomGenerator::global()->bounded(weaponPool.size());
    return weaponPool[randomIndex];
}

Weapon* WeaponManager::handleWeaponPickup(Character* character, Weapon* newWeapon) {
    if (!character || !newWeapon) {
        return nullptr;
    }
    
    Weapon* oldWeapon = character->getWeapon();
    
    // 如果角色当前没有武器或只有拳头
    if (!oldWeapon || dynamic_cast<Fist*>(oldWeapon)) {
        // 直接装备新武器
        character->equipWeapon(newWeapon);
        return nullptr; // 没有被替换的武器
    } else {
        // 角色已有武器，需要替换
        character->unequipWeapon(); // 卸下当前武器
        
        // 装备新武器
        character->equipWeapon(newWeapon);
        // 返回被替换的武器（调用者需要决定如何处理，通常是删除）
        return oldWeapon;
    }
}

void WeaponManager::removeDepletedRangedWeapon(Character* character) {
    if (!character) {
        return;
    }
    
    Weapon* weapon = character->getWeapon();
    if (!weapon) {
        return;
    }
    
    // 检查是否是用完的远程武器
    RangedWeapon* rangedWeapon = dynamic_cast<RangedWeapon*>(weapon);
    if (rangedWeapon && !rangedWeapon->canUse()) {
        
        // 卸下武器
        character->unequipWeapon();
        
        // 装备拳头作为默认武器
        Fist* defaultWeapon = new Fist(character);
        character->equipWeapon(defaultWeapon);
        
        // 删除用完的武器
        delete weapon;
    }
}

QPointF WeaponManager::getRandomDropPosition() const {
    if (dropArea.isEmpty()) {
        return QPointF(0, DROP_HEIGHT_OFFSET);
    }
    
    // 在掉落区域内生成随机X坐标
    qreal randomX = dropArea.left() + 
                   QRandomGenerator::global()->generateDouble() * dropArea.width();
    
    // Y坐标设置在屏幕上方，让物品自然掉落
    qreal dropY = DROP_HEIGHT_OFFSET;  // 从屏幕上方开始掉落
    
    return QPointF(randomX, dropY);
}
