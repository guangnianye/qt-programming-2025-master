//
// Created by gerw on 8/20/24.
//

#include <memory>
#include "Green.h"

Green::Green(QGraphicsItem *parent) : Character(parent) {
    edgeTolerance = 45; // 设置边界容忍度
    characterPixmapPaths = {
        { "normal", ":/Items/Characters/green/character_green_front.png" },
        { "duck", ":/Items/Characters/green/character_green_duck.png" },
        { "attack", ":/Items/Characters/green/character_green_jump.png" }
    };
    updatePixmapItem("normal");
}
