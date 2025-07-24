//
// Created by gerw on 8/21/24.
//

#include "Battlefield.h"

Battlefield::Battlefield(int mapId, QGraphicsItem *parent) 
    : Map(parent, QString(":/Items/Maps/Battlefield/%1.png").arg(mapId)), mapId(mapId) {
    initialize(); // 初始化平台和墙体
}

qreal Battlefield::getFloorHeight() {
    auto sceneRect = sceneBoundingRect();
    return (sceneRect.top() + sceneRect.bottom()) * 0.63;
}

void Battlefield::initializePlatforms() {
    platforms.clear();

    auto sceneRect = sceneBoundingRect();
    
    // 根据不同的地图ID设置不同的平台布局
    switch(mapId) {
        case 1:
            // 第一张地图的平台布局
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
            break;
            
        case 2:
            // 第二张地图的平台布局（可以根据实际需要调整）
            platforms.push_back(Platform(
                QRectF(sceneRect.left(), sceneRect.bottom() - 64, 192, 64), 
                            sceneRect.bottom() - 64,
                            0,
                            Platform::Type::Normal));
            platforms.push_back(Platform(
                QRectF(sceneRect.left() + 192, sceneRect.bottom() - 64, 128, 64), 
                            sceneRect.bottom() - 64,
                            0,
                            Platform::Type::Grass));
            platforms.push_back(Platform(
                QRectF(sceneRect.left() + 320, sceneRect.bottom() - 64, 512, 64), 
                            sceneRect.bottom() - 64,
                            0,
                            Platform::Type::Normal));
            platforms.push_back(Platform(
                QRectF(sceneRect.left() + 832, sceneRect.bottom() - 64, 128, 64), 
                            sceneRect.bottom() - 64,
                            0,
                            Platform::Type::Ice));
            platforms.push_back(Platform(
                QRectF(sceneRect.left() + 32*18, sceneRect.bottom() - 192, 320, 64), 
                            sceneRect.bottom() - 192,
                            0,
                            Platform::Type::Ice));
            platforms.push_back(Platform(
                QRectF(sceneRect.left() + 32, sceneRect.bottom() - 160, 192, 64), 
                            sceneRect.bottom() - 160,
                            0,
                            Platform::Type::Normal)); 
            platforms.push_back(Platform(
                QRectF(sceneRect.left() + 192, sceneRect.bottom() - 480, 128, 64), 
                            sceneRect.bottom() - 480,
                            0,
                            Platform::Type::Grass));     
            platforms.push_back(Platform(
                QRectF(sceneRect.left() + 704, sceneRect.bottom() - 480, 256, 64), 
                            sceneRect.bottom() - 480,
                            0,
                            Platform::Type::Grass));
            platforms.push_back(Platform(
                QRectF(sceneRect.left() + 160, sceneRect.bottom() - 320, 448, 64), 
                            sceneRect.bottom() - 320,
                            0,
                            Platform::Type::Normal));
            break;
        
        case 3:
            // 第三张地图的平台布局（可以根据实际需要调整）
            platforms.push_back(Platform(
                QRectF(sceneRect.left(), sceneRect.bottom() - 160, 960, 64), 
                            sceneRect.bottom() - 160,
                            0,
                            Platform::Type::Ice));
            platforms.push_back(Platform(
                QRectF(sceneRect.left() , sceneRect.bottom() - 384, 448, 64), 
                            sceneRect.bottom() - 384,
                            0,
                            Platform::Type::Normal));          
            platforms.push_back(Platform(
                QRectF(sceneRect.left() +512, sceneRect.bottom() - 384, 448, 64), 
                            sceneRect.bottom() - 384,
                            0,
                            Platform::Type::Normal));
            break;
        
        case 4:
            platforms.push_back(Platform(
                QRectF(sceneRect.left(), sceneRect.bottom() - 352, 128, 64), 
                            sceneRect.bottom() - 352,
                            0,
                            Platform::Type::Grass));
            platforms.push_back(Platform(
                QRectF(sceneRect.left() + 192, sceneRect.bottom() - 288, 128, 64), 
                            sceneRect.bottom() - 288,
                            0,
                            Platform::Type::Grass));
            platforms.push_back(Platform(
                QRectF(sceneRect.left() + 384, sceneRect.bottom() - 320, 128, 64), 
                            sceneRect.bottom() - 320,
                            0,
                            Platform::Type::Grass));                            
            platforms.push_back(Platform(
                QRectF(sceneRect.left() + 512, sceneRect.bottom() - 480, 128, 64), 
                            sceneRect.bottom() - 480,
                            0,
                            Platform::Type::Grass));
            platforms.push_back(Platform(
                QRectF(sceneRect.right() -320, sceneRect.bottom() - 384, 128, 64), 
                            sceneRect.bottom() - 384,
                            0,
                            Platform::Type::Grass));                            
            platforms.push_back(Platform(
                QRectF(sceneRect.right() - 128, sceneRect.bottom() - 352, 128, 64), 
                            sceneRect.bottom() - 352,
                            0,
                            Platform::Type::Grass));   
            break;  
        
        case 5:
            // 第五张地图的平台布局（可以根据实际需要调整）
            platforms.push_back(Platform(
                QRectF(sceneRect.left(), sceneRect.bottom() - 256, 960, 64), 
                            sceneRect.bottom() - 256,
                            0,
                            Platform::Type::Poison));
            platforms.push_back(Platform(
                QRectF(sceneRect.left() + 320, sceneRect.bottom() - 384, 320, 64), 
                            sceneRect.bottom() - 384,
                            0,
                            Platform::Type::Grass));
            break;
        default:
            // 默认使用第一张地图的布局
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
            break;
    }
}

void Battlefield::initializeWalls() {
    walls.clear();
    
    auto sceneRect = sceneBoundingRect();
    
    // 根据不同的地图ID设置不同的墙体布局
    switch(mapId) {
        case 1:
            // 第一张地图的墙体布局
            walls.push_back(Wall(QRectF(sceneRect.left() +319 , sceneRect.bottom() - 182, 1, 63)));
            walls.push_back(Wall(QRectF(sceneRect.left() + 220, sceneRect.bottom() - 138, 99, 64)));
            break;
            
        case 2:
            break;
        
        case 3:
            break;

        case 4:
            break;
        
        case 5:
            break;  
        
        default:
            // 默认使用第一张地图的墙体布局
            walls.push_back(Wall(QRectF(sceneRect.left() +319 , sceneRect.bottom() - 182, 1, 63)));
            walls.push_back(Wall(QRectF(sceneRect.left() + 220, sceneRect.bottom() - 138, 99, 64)));
            break;
    }
}