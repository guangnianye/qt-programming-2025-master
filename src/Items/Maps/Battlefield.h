//
// Created by gerw on 8/21/24.
//

#ifndef QT_PROGRAMMING_2024_BATTLEFIELD_H
#define QT_PROGRAMMING_2024_BATTLEFIELD_H


#include "Map.h"

class Battlefield: public Map {
public:
    explicit Battlefield(int mapId = 1, QGraphicsItem *parent= nullptr);

    qreal getFloorHeight() override;

protected:
    void initializePlatforms() override;
    void initializeWalls() override;
    
private:
    int mapId; // 地图ID，用于区分不同的战场图片
};


#endif //QT_PROGRAMMING_2024_BATTLEFIELD_H
