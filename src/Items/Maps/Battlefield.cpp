//
// Created by gerw on 8/21/24.
//

#include "Battlefield.h"

Battlefield::Battlefield(QGraphicsItem *parent) : Map(parent, ":/Items/Maps/Battlefield/1.png") {
    initialize(); // 初始化平台和墙体
}

qreal Battlefield::getFloorHeight() {
    auto sceneRect = sceneBoundingRect();
    return (sceneRect.top() + sceneRect.bottom()) * 0.63;
}

void Battlefield::initializePlatforms() {
    platforms.clear();

    auto sceneRect = sceneBoundingRect();
    platforms.push_back(Platform(
        QRectF(sceneRect.left(), sceneRect.bottom() - 192, 320, 192), 
                    sceneRect.bottom() - 192,
                    0,
                    Platform::Type::Grass));
    platforms.push_back(Platform(
        QRectF(sceneRect.left() + 320, sceneRect.bottom() - 128, 320 , 128), 
                    sceneRect.bottom() - 128,
                    0,
                    Platform::Type::Grass));
    platforms.push_back(Platform(
        QRectF(sceneRect.left() + 640, sceneRect.bottom() - 128, 320, 128), 
                    sceneRect.bottom() - 128,
                    0,
                    Platform::Type::Normal));
}

void Battlefield::initializeWalls() {
    walls.clear();
    
    auto sceneRect = sceneBoundingRect();
    walls.push_back(Wall(QRectF(sceneRect.left() +319 , sceneRect.bottom() - 182, 1, 63)));
    walls.push_back(Wall(QRectF(sceneRect.left() + 220, sceneRect.bottom() - 138, 99, 64)));
}