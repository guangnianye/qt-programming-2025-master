//
// Created by gerw on 8/20/24.
//

#ifndef QT_PROGRAMMING_2024_BATTLESCENE_H
#define QT_PROGRAMMING_2024_BATTLESCENE_H

#include <QKeyEvent>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include "Scene.h"
#include "../Items/Maps/Map.h"
#include "../Items/Characters/Character.h"
#include "../Items/Weapons/WeaponManager.h"

class BattleScene : public Scene {
Q_OBJECT

public:
    explicit BattleScene(QObject *parent);

    void processInput() override;

    void processMovement() override;

    void processPhysics(); //处理重力物理

    void processAttacks(); // 处理攻击逻辑

    void processCharacterAttack(Character* attacker, const QString& attackerName); // 处理单个角色的攻击逻辑

    void processMeleeAttack(Character* attacker, const QString& attackerName); // 处理近战攻击逻辑

    void processRangedAttack(Character* attacker, const QString& attackerName); // 处理远程攻击逻辑

    void processWeaponPickup(); // 处理武器拾取逻辑

    void processCharacterWeaponPickup(Character* character); // 处理单个角色的武器拾取

protected slots:

    void update() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

private:
    Weapon* findNearestWeapon(const QPointF& pos, qreal distanceThreshold); // 查找附近的武器
    
    bool isWeaponEquipped(Weapon* weapon); // 检查武器是否被装备

    Map *map;
    Character *character;
    Character *enemy; // 可能的敌人角色
    WeaponManager *weaponManager; // 武器管理器
    
    // 血量显示UI元素
    QGraphicsRectItem *healthBarBackground;  // 血量条背景
    QGraphicsRectItem *healthBarForeground;  // 血量条前景
    QGraphicsTextItem *healthText;           // 血量数值文本

signals:
    void returnToMapSelection();

};


#endif //QT_PROGRAMMING_2024_BATTLESCENE_H
