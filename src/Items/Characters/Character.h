//
// Created by gerw on 8/20/24.
//

#ifndef QT_PROGRAMMING_2024_CHARACTER_H
#define QT_PROGRAMMING_2024_CHARACTER_H

#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include "../Weapons/Weapon.h"
#include "../Weapons/Fist.h"
#include "../../Physics/PhysicsConstants.h"
#include "../Maps/Map.h"

class Map; // 前向声明

class Character : public Item {
public:
    int edgeTolerance{0}; // 用于边界检测的容忍度
    QMap<QString,QString> characterPixmapPaths; // 角色的不同状态的图片集合
public:
    explicit Character(QGraphicsItem *parent);

    [[nodiscard]] bool isLeftDown() const;

    void setLeftDown(bool leftDown);

    [[nodiscard]] bool isRightDown() const;

    void setRightDown(bool rightDown);

    [[nodiscard]] bool isUpDown() const;

    void setUpDown(bool upDown);

    [[nodiscard]] bool isSquatting() const;

    void setSquatting(bool squatting);

    // 武器装备相关方法
    [[nodiscard]] Weapon *getWeapon() const {
        return weapon;
    }

    // 装备武器
    Weapon* equipWeapon(Weapon* newWeapon);

    // 卸下武器
    Weapon* unequipWeapon();

    [[nodiscard]] bool isPickDown() const;

    void setPickDown(bool pickDown);

    void setAttacking(bool attacking) ;

    void processAttack();

    [[nodiscard]] bool isCurrentlyAttacking() const;

    [[nodiscard]] QRectF getAttackRange() const;

    // 血量系统相关方法
    [[nodiscard]] qreal getCurrentHealth() const;
    
    [[nodiscard]] qreal getMaxHealth() const;
    
    void setMaxHealth(qreal maxHealth);
    
    void takeDamage(qreal damage);
    
    void heal(qreal amount);
    
    [[nodiscard]] bool isAlive() const;
    
    [[nodiscard]] qreal getHealthPercentage() const;

    // 血量条UI相关方法
    void setupHealthBar();
    void updateHealthBar();
    void setHealthBarVisible(bool visible);


    // 检查是否装备了武器
    [[nodiscard]] bool isEquipped() const {
        return weapon != nullptr;
    }

    [[nodiscard]] const QPointF &getVelocity() const;

    [[nodiscard]] bool isPicking() const;

    void setVelocity(const QPointF &velocity);

    void processInput();

    // 重力系统相关方法
    [[nodiscard]] bool isOnGround() const;

    void setOnGround(bool onGround);

    [[nodiscard]] qreal getGravityAcceleration() const;

    void applyGravity(qreal deltaTime);

    void handleGroundCollision(qreal groundHeight);
    
    // 移动和边界检查
    void processMovementAndBounds(qreal deltaTime, const QRectF& sceneRect);

    // 墙碰撞检查
    void checkWallCollisions(const QVector<Wall>& walls);
    
    // 地图相关方法
    void setMap(Map* map);
    
    [[nodiscard]] Map* getMap() const;

    // 角色状态和显示相关方法
    void updatePixmapItem(const QString& condition) ;
    
    // 方向控制方法
    void setDirection(bool right);
    [[nodiscard]] bool isDirectionRight() const;

    // 不同种类平台方法
    void processPlatformstypes();
  
protected:
    Weapon *weapon{};  // 当前装备的武器
    QPointF velocity{};
//    QGraphicsEllipseItem *ellipseItem; // for debugging

    // 重力系统相关属性
    bool onGround{true};                                      // 是否在地面上
    qreal gravityAcceleration{PhysicsConstants::GRAVITY_ACCELERATION};   // 重力加速度
    qreal maxFallSpeed{PhysicsConstants::MAX_FALL_SPEED};     // 最大下降速度
    qreal jumpSpeed{PhysicsConstants::JUMP_SPEED};            // 跳跃初始速度

private:
    bool leftDown{}, rightDown{},upDown{}, pickDown{};
    bool directionRight{true}; // 角色朝向，默认向右
    bool lastPickDown{};
    bool picking{};
    bool squatting{};
    bool attackDown{};           // K键是否被按下
    bool lastAttackDown{};       // 上一帧攻击键状态
    bool isPerformingAttack{};   // 是否正在执行攻击动作
    qreal attackTimer{};         // 攻击计时器
    qreal attackIntervalTimer{}; // 攻击间隔计时器（用于连击间隔）
    qreal attackRecoveryTimer{}; // 攻击后摇计时器（用于显示状态）
    
    // 血量系统相关属性
    qreal currentHealth{100.0};  // 当前血量
    qreal maxHealth{100.0};      // 最大血量
    
    // 血量条UI相关属性
    QGraphicsRectItem *healthBarBackground{nullptr};  // 血量条背景
    QGraphicsRectItem *healthBarForeground{nullptr};  // 血量条前景
    QGraphicsTextItem *healthText{nullptr};           // 血量数值文本
    bool healthBarVisible{true};                       // 血量条是否可见
    
    // 地图引用
    Map* map{nullptr};           // 当前地图的引用
};


#endif //QT_PROGRAMMING_2024_CHARACTER_H
