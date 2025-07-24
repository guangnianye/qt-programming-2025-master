//
// Created for gravity system
//

#ifndef QT_PROGRAMMING_2024_PHYSICSCONSTANTS_H
#define QT_PROGRAMMING_2024_PHYSICSCONSTANTS_H

#include <QtGlobal>

class PhysicsConstants {
public:
    // 重力相关常量
    static constexpr qreal GRAVITY_ACCELERATION = 980.0;    // 像素/秒²
    static constexpr qreal MAX_FALL_SPEED = 500.0;          // 像素/秒
    static constexpr qreal JUMP_SPEED = -500.0;             // 像素/秒 (负值表示向上)
    
    // 移动相关常量
    static constexpr qreal HORIZONTAL_MOVE_SPEED = 300.0;   // 像素/秒
    static constexpr qreal ITEM_FALL_SPEED = 200.0;         // 物品下落速度 像素/秒
    static constexpr qreal ICE_FRICTION = 1.5;          // 冰面摩擦系数 (速度加快)
    
    // 碰撞检测相关
    static constexpr qreal GROUND_TOLERANCE = 5.0;          // 地面检测容差 像素
    static constexpr qreal PICKUP_DISTANCE = 100.0;         // 拾取距离 像素
    
    // 攻击相关常量
    static constexpr qreal ATTACK_DURATION = 0.0;           // 攻击持续时间 毫秒 (瞬间完成，使用标记触发)
    static constexpr qreal ATTACK_RECOVERY = 200.0;         // 攻击后摇时间 毫秒 (显示attack状态时间)
    static constexpr qreal ATTACK_INTERVAL = 300.0;         // 攻击间隔时间 毫秒 (长按连击间隔，包含后摇+normal显示时间)
    static constexpr qreal ATTACK_RANGE = 80.0;             // 攻击范围 像素
    static constexpr qreal ATTACK_DAMAGE = 10.0;            // 攻击伤害
    
    // 血量系统相关常量
    static constexpr qreal DEFAULT_MAX_HEALTH = 200.0;      // 默认最大血量
};

#endif //QT_PROGRAMMING_2024_PHYSICSCONSTANTS_H
