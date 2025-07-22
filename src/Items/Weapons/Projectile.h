//
// Created by guangnianye on 7/22/25
//

#ifndef QT_PROGRAMMING_2024_PROJECTILE_H
#define QT_PROGRAMMING_2024_PROJECTILE_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QPointF>

class Character; // 前向声明

// 投射物基类
class Projectile : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit Projectile(const QPointF& startPos, const QPointF& direction, qreal damage, 
                       const QString& imagePath = "", QGraphicsItem *parent = nullptr);
    virtual ~Projectile();
    
    // 开始飞行
    virtual void startFlight();
    
    // 停止飞行
    virtual void stopFlight();
    
    // 获取伤害值
    qreal getDamage() const { return damage; }
    
    // 设置速度
    void setVelocity(const QPointF& vel) { velocity = vel; }
    QPointF getVelocity() const { return velocity; }
    
    // 设置重力影响
    void setGravityAffected(bool affected) { gravityAffected = affected; }
    bool isGravityAffected() const { return gravityAffected; }

protected slots:
    virtual void updatePosition();

protected:
    // 检查碰撞（只检查角色）
    virtual void checkCollisions();
    
    // 碰撞处理
    virtual void onCharacterHit(Character* character);
    
    // 清理投射物
    virtual void cleanup();
    
    // 检查是否超出边界
    virtual bool isOutOfBounds() const;

protected:
    QTimer *flightTimer;
    QPointF velocity;           // 速度向量
    QPointF position;           // 当前位置
    qreal damage;               // 伤害值
    qreal gravity;              // 重力加速度
    bool gravityAffected;       // 是否受重力影响
    int timeElapsed;            // 飞行时间（毫秒）
    int maxFlightTime;          // 最大飞行时间（毫秒）
    
private:
    static constexpr int DEFAULT_MAX_FLIGHT_TIME = 5000; // 默认最大飞行时间5秒
};

#endif //QT_PROGRAMMING_2024_PROJECTILE_H
