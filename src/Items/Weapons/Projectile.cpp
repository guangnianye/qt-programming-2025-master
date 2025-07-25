//
// Created by guangnianye on 7/22/25
//

#include "Projectile.h"
#include "../../Physics/PhysicsConstants.h"
#include "../Characters/Character.h"
#include <QGraphicsScene>
#include <QPixmap>
#include <QtMath>
#include <QDebug>

Projectile::Projectile(const QPointF& startPos, const QPointF& direction, qreal damage, 
                      const QString& imagePath, const QString& weaponName, QGraphicsItem *parent)
    : QObject(nullptr)
    , QGraphicsPixmapItem(parent)
    , flightTimer(new QTimer())
    , position(startPos)
    , damage(damage)
    , weaponName(weaponName)
    , gravity(PhysicsConstants::GRAVITY_ACCELERATION)
    , gravityAffected(true)
    , timeElapsed(0)
    , maxFlightTime(DEFAULT_MAX_FLIGHT_TIME)
{
    // 设置投射物图像
    if (!imagePath.isEmpty()) {
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
            // 缩放图像到合适大小（比如20x20像素）
            setPixmap(pixmap.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
    
    // 如果没有图像或加载失败，创建默认外观
    if (pixmap().isNull()) {
        QPixmap defaultPixmap(20, 20);
        defaultPixmap.fill(QColor(100, 100, 100)); // 灰色默认外观
        setPixmap(defaultPixmap);
    }
    
    setPos(startPos);
    
    // 设置变换原点为中心
    setTransformOriginPoint(boundingRect().center());
    
    // 设置默认速度
    qreal length = std::sqrt(direction.x() * direction.x() + direction.y() * direction.y());
    QPointF normalizedDirection = (length > 0) ? QPointF(direction.x() / length, direction.y() / length) : QPointF(1, 0);
    velocity = normalizedDirection * 400.0; // 默认速度400像素/秒
    
    // 设置定时器
    connect(flightTimer, &QTimer::timeout, this, &Projectile::updatePosition);
}

Projectile::~Projectile() {
    if (flightTimer) {
        flightTimer->deleteLater();
    }
}

void Projectile::startFlight() {
    if (flightTimer) {
        flightTimer->start(16);  // 约60FPS
    }
}

void Projectile::stopFlight() {
    if (flightTimer) {
        flightTimer->stop();
    }
}

void Projectile::updatePosition() {
    const qreal deltaTime = 0.016;  // 16ms = 0.016s
    timeElapsed += 16;
    
    // 更新位置
    position.setX(position.x() + velocity.x() * deltaTime);
    position.setY(position.y() + velocity.y() * deltaTime);
    
    // 应用重力（如果受重力影响）
    if (gravityAffected) {
        velocity.setY(velocity.y() + gravity * deltaTime);
    }
    
    // 更新图形位置
    setPos(position);
    
    // 旋转投射物以匹配运动方向
    if (velocity.x() != 0 || velocity.y() != 0) {
        qreal angle = qAtan2(velocity.y(), velocity.x()) * 180.0 / M_PI;
        setRotation(angle);
    }
    
    // 检查碰撞
    checkCollisions();
    
    // 检查是否超出边界或飞行时间过长
    if (isOutOfBounds() || timeElapsed > maxFlightTime) {
        cleanup();
    }
}

void Projectile::checkCollisions() {
    if (!scene()) return;
    
    // 获取碰撞的物体
    QList<QGraphicsItem*> collisions = scene()->collidingItems(this);
    
    for (QGraphicsItem* item : collisions) {
        if (item != this && item != parentItem()) {
            // 只检测是否撞击到角色
            if (auto character = dynamic_cast<Character*>(item)) {
                onCharacterHit(character);
                return; // 碰撞后立即返回
            }
            // 忽略其他物品的碰撞
        }
    }
}

void Projectile::onCharacterHit(Character* character) {
    if (character && character->isAlive()) {
        // 对角色造成伤害，传递武器名称
        character->takeDamage(damage, weaponName);
        qDebug() << "Projectile from" << weaponName << "hit character! Damage:" << damage 
                << "Target health:" << character->getCurrentHealth();
    }
    
    // 撞击后销毁投射物
    cleanup();
}

void Projectile::cleanup() {
    stopFlight();
    if (scene()) {
        scene()->removeItem(this);
    }
    deleteLater();
}

bool Projectile::isOutOfBounds() const {
    if (!scene()) return true;
    
    QRectF sceneRect = scene()->sceneRect();
    return (position.x() < sceneRect.left() - 50 || 
            position.x() > sceneRect.right() + 50 || 
            position.y() < sceneRect.top() - 50 ||
            position.y() > sceneRect.bottom() + 50);
}
