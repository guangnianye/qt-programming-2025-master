//
// Created by user on 7/24/25
//

#ifndef QT_PROGRAMMING_2024_BUFFSYSTEM_H
#define QT_PROGRAMMING_2024_BUFFSYSTEM_H

#include <QString>

// 增益效果结构
struct BuffEffect {
    QString name;             // 增益名称
    qreal speedMultiplier;    // 速度倍数（1.0 = 正常速度，1.5 = 1.5倍速度）
    qreal healthRegenRate;    // 血量回复速度（每秒）
    int duration;             // 持续时间（毫秒）
    int tickInterval;         // 触发间隔（毫秒，用于持续性效果如回血）
    
    // 默认构造函数
    BuffEffect() = default;

    // 参数构造函数
    BuffEffect(const QString& n, qreal sm = 1.0, qreal hrr = 0.0, int d = 0, int ti = 1000)
        : name(n), speedMultiplier(sm), healthRegenRate(hrr), duration(d), tickInterval(ti) {}
};

#endif //QT_PROGRAMMING_2024_BUFFSYSTEM_H
