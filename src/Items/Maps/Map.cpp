//
// Created by gerw on 8/21/24.
//

#include "Map.h"
#include <algorithm>
#include <limits>
#include <vector>
Map::Map(QGraphicsItem *parent, const QString &pixmapPath) : Item(parent, pixmapPath) {}

void Map::initialize() {
    // 默认实现 - 子类可以重写这个方法来定义具体的平台
    // 添加一个覆盖整个地图底部的平台作为默认地面
    initializePlatforms();
    initializeWalls();
}

void Map::scaleToFitScene(QGraphicsScene *scene) {
    // Calculate scale factors to scale the item to fit the entire scene
    QRectF sceneRect = scene->sceneRect();
    QRectF itemRect = boundingRect();

    qreal scaleX = sceneRect.width() / itemRect.width();
    qreal scaleY = sceneRect.height() / itemRect.height();

    // Choose the smaller scale factor to maintain aspect ratio
    qreal scaleFactor = qMin(scaleX, scaleY);

    // Apply the scale to the item
    setTransform(QTransform::fromScale(scaleFactor, scaleFactor), true);

    // Center the item in the scene (optional)
    // setPos((sceneRect.width() - boundingRect().width() * scaleFactor) / 2,
    //        (sceneRect.height() - boundingRect().height() * scaleFactor) / 2);

}

QPointF Map::getSpawnPos() {
    auto boundingRect = sceneBoundingRect();
    auto midX = (boundingRect.left() + boundingRect.right()) * 0.5;
    return {midX, boundingRect.top()}; // 假设生成点在顶部
}

qreal Map::getFloorHeight() {
    auto sceneRect = sceneBoundingRect();
    return sceneRect.top() + (sceneRect.top() - sceneRect.bottom()) * 0.5;
}

qreal Map::getFloorHeightAt(qreal x) {
    qreal highestFloor = boundingRect().bottom();
    
    for (const Platform& platform : platforms) {
        if (platform.containsX(x)) {
            highestFloor = qMin(highestFloor, platform.height);
        }
    }
    
    return highestFloor;
}

QList<qreal> Map::getAllFloorHeightsAt(qreal x) {
    QList<qreal> heights;
    
    for (const Platform& platform : platforms) {
        if (platform.containsX(x)) {
            heights.append(platform.height);
        }
    }
    
    // 按高度排序（从高到低）
    std::sort(heights.begin(), heights.end());
    
    return heights;
}

Platform* Map::getNearestPlatform(QPointF position, qreal maxDistance) {
    Platform* nearest = nullptr;
    qreal minDistance = maxDistance;

    for (Platform& platform : platforms) {
        if (platform.containsX(position.x())) {
            qreal distance = qAbs(position.y() - platform.height);
            if (distance < minDistance) {
                minDistance = distance;
                nearest = &platform;
            }
        }
    }

    return nearest;
}

Platform* Map::getNearestPlatform(QPointF position1, QPointF position2, qreal maxDistance) {
    Platform* nearest = nullptr;
    qreal minDistance = maxDistance;

    for (Platform& platform : platforms) {
        if (platform.containsX(position1.x()) || platform.containsX(position2.x())) {
            qreal distance1 = qAbs(position1.y() - platform.height);
            qreal distance2 = qAbs(position2.y() - platform.height);
            qreal distance = std::min(distance1, distance2);
            if (distance < minDistance) {
                minDistance = distance;
                nearest = &platform;
            }
        }
    }

    return nearest;
}

Platform* Map::getPlatformAt(QPointF position) {
    for (Platform& platform : platforms) {
        if (platform.containsPoint(QPointF(position.x(), position.y()))) {
            return &platform;
        }
    }
    return nullptr;
}

QList<Platform *> Map::getPlatformsInRange(qreal xMin, qreal xMax) {
    QList<Platform *> result;

    for (Platform& platform : platforms) {
        // 检查平台的X范围是否与查询范围重叠
        if (platform.rect.right() >= xMin && platform.rect.left() <= xMax) {
            result.append(&platform);
        }
    }

    return result;
}

QVector<Wall> Map::getWalls(){
    return walls;
}

void Map::initializePlatforms() {
    // 默认实现 - 可以为空或提供基础平台
}

void Map::initializeWalls() {
    // 默认实现 - 可以为空或提供基础墙体
}