//
// Created by gerw on 8/20/24.
//

#include <QDebug>
#include "BattleScene.h"
#include "../Items/Characters/Link.h"
#include "../Items/Maps/Battlefield.h"
#include "../Items/Armors/FlamebreakerArmor.h"
#include "../Physics/PhysicsConstants.h"

BattleScene::BattleScene(QObject *parent) : Scene(parent) {
    // This is useful if you want the scene to have the exact same dimensions as the view
    setSceneRect(0, 0, 960, 640);
    map = new Battlefield();
    character = new Link();
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
    
    // 初始化血量显示UI
    setupHealthDisplay();
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
        default:
            Scene::keyPressEvent(event);
    }
}

void BattleScene::keyReleaseEvent(QKeyEvent *event) {
    switch (event->key()) {
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
        default:
            Scene::keyReleaseEvent(event);
    }
}

void BattleScene::update() {
    Scene::update();
    processPhysics(); // 处理重力物理
    processAttacks(); // 处理攻击逻辑
    updateHealthDisplay(); // 更新血量显示
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
    if (character && character->isCurrentlyAttacking()) {
        // 获取攻击范围
        QRectF attackRange = character->getAttackRange();
        
        // 检查攻击范围内的物品
        for (QGraphicsItem *item : items()) {
            // 检查是否与攻击范围相交
            if (item != character && item->boundingRect().translated(item->pos()).intersects(attackRange)) {
                // 检查是否攻击到其他角色
                if (auto targetCharacter = dynamic_cast<Character *>(item)) {
                    if (targetCharacter->isAlive()) {
                        targetCharacter->takeDamage(PhysicsConstants::ATTACK_DAMAGE);
                        qDebug() << "Attacked character! Target health:" << targetCharacter->getCurrentHealth();
                    }
                }
            }
        }
    }
}

void BattleScene::setupHealthDisplay() {
    // 血量条的位置和大小
    qreal barWidth = 200;
    qreal barHeight = 20;
    qreal barX = 20; // 距离屏幕左边的距离
    qreal barY = 20; // 距离屏幕顶部的距离
    
    // 创建血量条背景（灰色）
    healthBarBackground = new QGraphicsRectItem(barX, barY, barWidth, barHeight);
    healthBarBackground->setBrush(QBrush(QColor(100, 100, 100))); // 深灰色背景
    healthBarBackground->setPen(QPen(QColor(0, 0, 0), 2)); // 黑色边框
    healthBarBackground->setZValue(1000); // 确保在最上层显示
    addItem(healthBarBackground);
    
    // 创建血量条前景（红色）
    healthBarForeground = new QGraphicsRectItem(barX, barY, barWidth, barHeight);
    healthBarForeground->setBrush(QBrush(QColor(255, 0, 0))); // 红色前景
    healthBarForeground->setPen(QPen(Qt::NoPen)); // 无边框
    healthBarForeground->setZValue(1001); // 在背景之上
    addItem(healthBarForeground);
    
    // 创建血量数值文本
    healthText = new QGraphicsTextItem();
    healthText->setPos(barX + barWidth + 10, barY - 5); // 位于血量条右侧
    healthText->setDefaultTextColor(QColor(255, 255, 255)); // 白色文字
    healthText->setFont(QFont("Arial", 12, QFont::Bold)); // 粗体字体
    healthText->setZValue(1002); // 在最上层
    addItem(healthText);
    
    // 初始更新血量显示
    updateHealthDisplay();
}

void BattleScene::updateHealthDisplay() {
    if (character && healthBarBackground && healthBarForeground && healthText) {
        qreal currentHealth = character->getCurrentHealth();
        qreal maxHealth = character->getMaxHealth();
        qreal healthPercentage = (maxHealth > 0) ? (currentHealth / maxHealth) : 0;
        
        // 更新血量条宽度
        qreal barWidth = 200;
        qreal currentWidth = barWidth * healthPercentage;
        
        // 更新前景血量条的宽度
        QRectF rect = healthBarForeground->rect();
        rect.setWidth(currentWidth);
        healthBarForeground->setRect(rect);
        
        // 根据血量百分比改变颜色
        QColor barColor;
        if (healthPercentage > 0.6) {
            barColor = QColor(0, 255, 0); // 绿色 (健康)
        } else if (healthPercentage > 0.3) {
            barColor = QColor(255, 255, 0); // 黄色 (警告)
        } else {
            barColor = QColor(255, 0, 0); // 红色 (危险)
        }
        healthBarForeground->setBrush(QBrush(barColor));
        
        // 更新血量数值文本
        QString healthString = QString("%1/%2").arg(static_cast<int>(currentHealth)).arg(static_cast<int>(maxHealth));
        healthText->setPlainText(healthString);
        
        // 如果角色死亡，显示特殊文本
        if (!character->isAlive()) {
            healthText->setPlainText("DEAD");
            healthText->setDefaultTextColor(QColor(255, 0, 0)); // 红色死亡文字
        } else {
            healthText->setDefaultTextColor(QColor(255, 255, 255)); // 白色正常文字
        }
    }
}