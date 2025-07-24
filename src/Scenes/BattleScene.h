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
#include "../Items/Medicine/MedicineManager.h"
#include "../Items/PickupManager.h"
#include "../AI/PathFinder.h"
#include "../AI/AIController.h"
#include "../GameMode.h"

class BattleScene : public Scene {
Q_OBJECT

public:
    explicit BattleScene(QObject *parent, int mapId = 1, GameMode gameMode = GameMode::PvP);
    ~BattleScene(); // 析构函数，确保正确清理资源

    void processInput() override;

    void processMovement() override;

    void processPhysics(); //处理重力物理

    void processAttacks(); // 处理攻击逻辑

    void processCharacterAttack(Character* attacker, const QString& attackerName); // 处理单个角色的攻击逻辑

    void processMeleeAttack(Character* attacker, const QString& attackerName); // 处理近战攻击逻辑

    void processRangedAttack(Character* attacker, const QString& attackerName); // 处理远程攻击逻辑

    void processWeaponPickup(); // 处理武器拾取逻辑

    void processCharacterWeaponPickup(Character* character); // 处理单个角色的武器拾取
    
    void processItemPickup(); // 处理物品拾取逻辑（包括武器和药物）
    
    void processCharacterItemPickup(Character* character); // 处理单个角色的物品拾取
    
    // AI相关方法
    void initializeAI(); // 初始化AI系统
    void enableAI(bool enabled); // 启用/禁用AI
    void setAITarget(Character* aiCharacter, const QPointF& target); // 设置AI目标位置
    void setAITargetCharacter(Character* aiCharacter, Character* target); // 设置AI目标角色
    bool isAIEnabled() const; // 检查AI是否启用
    
    // 游戏模式相关方法
    GameMode getCurrentGameMode() const { return currentGameMode; }
    void setGameMode(GameMode mode);
    QString getGameModeDisplayText() const;

protected slots:

    void update() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

private:
    Weapon* findNearestWeapon(const QPointF& pos, qreal distanceThreshold); // 查找附近的武器
    
    bool isWeaponEquipped(Weapon* weapon); // 检查武器是否被装备
    
    void checkGameOver(); // 检查游戏是否结束

    Map *map;
    Character *character;
    Character *enemy; // 可能的敌人角色
    WeaponManager *weaponManager; // 武器管理器
    MedicineManager *medicineManager; // 药物管理器
    PickupManager *pickupManager; // 统一拾取管理器
    
    // 游戏模式
    GameMode currentGameMode; // 当前游戏模式
    
    // AI系统组件
    PathFinder *pathFinder; // 路径寻找器
    AIController *aiController; // AI控制器（控制enemy角色）
    bool aiEnabled; // AI是否启用
    
    // UI元素
    QGraphicsTextItem *gameModeText; // 游戏模式显示文本
    
    // 血量显示UI元素
    QGraphicsRectItem *healthBarBackground;  // 血量条背景
    QGraphicsRectItem *healthBarForeground;  // 血量条前景
    QGraphicsTextItem *healthText;           // 血量数值文本

signals:
    void returnToMapSelection();
    void gameOver(const QString& winner); // 游戏结束信号，传递获胜者名称

};


#endif //QT_PROGRAMMING_2024_BATTLESCENE_H
