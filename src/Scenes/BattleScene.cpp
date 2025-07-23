//
// Created by gerw on 8/20/24.
//

#include <QDebug>
#include "BattleScene.h"
#include "../Items/Characters/Link.h"
#include "../Items/Maps/Battlefield.h"
#include "../Items/Armors/FlamebreakerArmor.h"
#include "../Physics/PhysicsConstants.h"
#include "../Items/Weapons/ThrowingStone.h"
#include "../Items/Weapons/Sword.h"

BattleScene::BattleScene(QObject *parent) : Scene(parent) {
    // This is useful if you want the scene to have the exact same dimensions as the view
    setSceneRect(0, 0, 960, 640);
    map = new Battlefield();
    character = new Link();
    Weapon* newWeapon = new Sword(character);
    character->equipWeapon(newWeapon);
    enemy = new Link(); // 这里可以替换为其他敌人角色
    spareArmor = new FlamebreakerArmor();
    addItem(map);
    addItem(character);
    addItem(enemy);
    addItem(spareArmor);
    map->scaleToFitScene(this);
    
    // 设置角色的地图引用
    character->setMap(map);
    // 设置敌人的地图引用
    enemy->setMap(map);
    
    // 设置角色位置并确保在地面上
    QPointF spawnPos = map->getSpawnPos();
    character->setPos(spawnPos);
    character->handleGroundCollision(map->getFloorHeightAt(spawnPos.x())); // 确保角色在地面上

    // 设置敌人位置并确保在地面上
    QPointF enemySpawnPos = QPointF(spawnPos.x() + 400, spawnPos.y());
    enemy->setPos(enemySpawnPos);
    enemy->handleGroundCollision(map->getFloorHeightAt(enemySpawnPos.x())); // 确保敌人在地面上
    
    spareArmor->unmount();
    spareArmor->setPos(sceneRect().left() + (sceneRect().right() - sceneRect().left()) * 0.75, map->getFloorHeightAt(sceneRect().left() + (sceneRect().right() - sceneRect().left()) * 0.75));

}

void BattleScene::processInput() {
    Scene::processInput();
    if (character != nullptr) {
        character->processInput();
    }
    if (enemy != nullptr) {
        enemy->processInput();
    }
}

void BattleScene::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        // 玩家角色控制 (WASD + JK)
        case Qt::Key_A:
            if (character != nullptr) {
                character->setLeftDown(true);
            }
            break;
        case Qt::Key_D:
            if (character != nullptr) {
                character->setRightDown(true);
            }
            break;
        case Qt::Key_W: // 跳跃键
            if (character != nullptr) {
                character->setUpDown(true);
            }
            break;
        case Qt::Key_S: // 蹲下键
            if (character != nullptr) {
                character->setSquatting(true);
            }
            break;
        case Qt::Key_J:
            if (character != nullptr) {
                character->setPickDown(true);
            }
            break;
        case Qt::Key_K: // 攻击键
            if (character != nullptr) {
                character->setAttacking(true);
            }
            break;
            
        // 敌人角色控制 (方向键 + NM)
        case Qt::Key_Left:
            if (enemy != nullptr) {
                enemy->setLeftDown(true);
            }
            break;
        case Qt::Key_Right:
            if (enemy != nullptr) {
                enemy->setRightDown(true);
            }
            break;
        case Qt::Key_Up: // 跳跃键
            if (enemy != nullptr) {
                enemy->setUpDown(true);
            }
            break;
        case Qt::Key_Down: // 蹲下键
            if (enemy != nullptr) {
                enemy->setSquatting(true);
            }
            break;
        case Qt::Key_N:
            if (enemy != nullptr) {
                enemy->setPickDown(true);
            }
            break;
        case Qt::Key_M: // 攻击键
            if (enemy != nullptr) {
                enemy->setAttacking(true);
            }
            break;
        case Qt::Key_Escape: // 返回地图选择
            qDebug() << "返回地图选择";
            emit returnToMapSelection();
            break;
        default:
            Scene::keyPressEvent(event);
    }
}

void BattleScene::keyReleaseEvent(QKeyEvent *event) {
    switch (event->key()) {
        // 玩家角色控制 (WASD + JK)
        case Qt::Key_A:
            if (character != nullptr) {
                character->setLeftDown(false);
            }
            break;
        case Qt::Key_D:
            if (character != nullptr) {
                character->setRightDown(false);
            }
            break;
        case Qt::Key_W:
            if (character != nullptr) {
                character->setUpDown(false);
            }
            break;
        case Qt::Key_S:
            if (character != nullptr) {
                character->setSquatting(false);
            }
            break;
        case Qt::Key_J:
            if (character != nullptr) {
                character->setPickDown(false);
            }
            break;
        case Qt::Key_K: // 攻击键
            if (character != nullptr) {
                character->setAttacking(false);
            }
            break;
            
        // 敌人角色控制 (方向键 + NM)
        case Qt::Key_Left:
            if (enemy != nullptr) {
                enemy->setLeftDown(false);
            }
            break;
        case Qt::Key_Right:
            if (enemy != nullptr) {
                enemy->setRightDown(false);
            }
            break;
        case Qt::Key_Up:
            if (enemy != nullptr) {
                enemy->setUpDown(false);
            }
            break;
        case Qt::Key_Down:
            if (enemy != nullptr) {
                enemy->setSquatting(false);
            }
            break;
        case Qt::Key_N:
            if (enemy != nullptr) {
                enemy->setPickDown(false);
            }
            break;
        case Qt::Key_M: // 攻击键
            if (enemy != nullptr) {
                enemy->setAttacking(false);
            }
            break;
        default:
            Scene::keyReleaseEvent(event);
    }
}

void BattleScene::update() {
    Scene::update();
    processPhysics(); // 处理重力物理
    processAttacks(); // 处理攻击逻辑
    // 血量显示现在由角色自己管理，不需要在场景中更新
}

void BattleScene::processPhysics() {
    // 为所有可装备物品应用重力
    for (QGraphicsItem *item : items()) {
        if (auto mountable = dynamic_cast<Mountable *>(item)) {
            if (!mountable->isMounted()) {
                // 为未装备的物品应用简单的重力,适配新的平台机制而不是原来的地面机制
                QPointF itemPos = item->pos();
                qreal floorHeight = map->getFloorHeightAt(itemPos.x());
                const Platform *platform = map->getNearestPlatform(item->pos());
                if (platform)
                {
                    // 如果物品在平台上，保持其高度
                    if (platform->containsPoint(itemPos)) {
                        item->setPos(itemPos.x(), platform->height);
                    } else {
                        // 否则应用重力
                        if (itemPos.y() < floorHeight) {
                            item->setPos(itemPos.x(), floorHeight);
                        } else {
                            item->setPos(itemPos.x(), itemPos.y() + PhysicsConstants::GRAVITY_ACCELERATION);
                        }
                    }
                } else {
                    // 如果没有找到平台，应用重力到地面高度
                    if (itemPos.y() < floorHeight) {
                        item->setPos(itemPos.x(), floorHeight);
                    } else {
                        item->setPos(itemPos.x(), itemPos.y() + PhysicsConstants::GRAVITY_ACCELERATION);
                    }
                }
                
            }
        }
    }
}

void BattleScene::processMovement() {
    Scene::processMovement();    
    // 对场景中的所有角色应用移动和边界检查
    for (QGraphicsItem *item : items()) {
        if (auto characteritem = dynamic_cast<Character *>(item)) {
            characteritem->processMovementAndBounds(deltaTime, this->sceneRect());
        }
    }
}

void BattleScene::processPicking() {
    Scene::processPicking();
    if (character->isPicking()) {
        auto mountable = findNearestUnmountedMountable(character->pos(), PhysicsConstants::PICKUP_DISTANCE);
        if (mountable != nullptr) {
            spareArmor = dynamic_cast<Armor *>(pickupMountable(character, mountable));
        }
    }
}

Mountable *BattleScene::findNearestUnmountedMountable(const QPointF &pos, qreal distance_threshold) {
    Mountable *nearest = nullptr;
    qreal minDistance = distance_threshold;

    for (QGraphicsItem *item: items()) {
        if (auto mountable = dynamic_cast<Mountable *>(item)) {
            if (!mountable->isMounted()) {
                qreal distance = QLineF(pos, item->pos()).length();
                if (distance < minDistance) {
                    minDistance = distance;
                    nearest = mountable;
                }
            }
        }
    }

    return nearest;
}

Mountable *BattleScene::pickupMountable(Character *character, Mountable *mountable) {
    // Limitation: currently only supports armor
    if (auto armor = dynamic_cast<Armor *>(mountable)) {
        return character->pickupArmor(armor);
    }
    return nullptr;
}

void BattleScene::processAttacks() {
    // 处理玩家角色的攻击
    processCharacterAttack(character, "Player");
    
    // 处理敌人的攻击
    processCharacterAttack(enemy, "Enemy");
}

void BattleScene::processCharacterAttack(Character* attacker, const QString& attackerName) {
    if (!attacker || !attacker->isCurrentlyAttacking()) {
        return;
    }
    
    Weapon* weapon = attacker->getWeapon();
    if (!weapon) {
        return;
    }
    
    // 根据武器类型采用不同的攻击处理策略
    if (weapon->type == WeaponType::Melee) {
        // 近战武器：使用范围检测
        processMeleeAttack(attacker, attackerName);
    } else if (weapon->type == WeaponType::Ranged) {
        // 远程武器：投射物由武器系统自己处理
        processRangedAttack(attacker, attackerName);
    }
}

void BattleScene::processMeleeAttack(Character* attacker, const QString& attackerName) {
    // 获取攻击范围
    QRectF attackRange = attacker->getAttackRange();
    
    // 检查攻击范围内的物品
    for (QGraphicsItem *item : items()) {
        // 检查是否与攻击范围相交
        if (item != attacker && item->boundingRect().translated(item->pos()).intersects(attackRange)) {
            // 检查是否攻击到其他角色
            if (auto targetCharacter = dynamic_cast<Character *>(item)) {
                if (targetCharacter->isAlive()) {
                    // 使用攻击者当前武器的伤害值
                    qreal weaponDamage = attacker->getWeapon()->getDamage();
                    
                    targetCharacter->takeDamage(weaponDamage);
                    qDebug() << attackerName << "melee attacked character with" << attacker->getWeapon()->getWeaponname() 
                            << "! Damage:" << weaponDamage 
                            << "Target health:" << targetCharacter->getCurrentHealth();
                }
            }
        }
    }
}

void BattleScene::processRangedAttack(Character* attacker, const QString& attackerName) {
    // 对于远程武器，主要逻辑由武器系统的投射物处理
    // 这里主要负责记录攻击信息和可能的额外逻辑
    
    qDebug() << attackerName << "performed ranged attack with" << attacker->getWeapon()->getWeaponname();
    
    // 注意：远程武器的实际伤害计算由投射物的碰撞检测处理
    // 这里可以添加一些额外的逻辑，比如：
    // 1. 攻击特效
    // 2. 音效
    // 3. 攻击统计
    // 4. 特殊状态效果等
    
    // 如果需要在场景中跟踪投射物或添加特殊逻辑，可以在这里实现
}