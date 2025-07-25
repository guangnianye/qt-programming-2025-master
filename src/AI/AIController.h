//
// Created by AI Assistant on 7/24/25
//

#ifndef QT_PROGRAMMING_2024_AICONTROLLER_H
#define QT_PROGRAMMING_2024_AICONTROLLER_H

#include <QObject>
#include "../Items/Characters/Character.h"

/**
 * @brief AI控制器类
 */
class AIController : public QObject {
    Q_OBJECT

public:
    explicit AIController(Character* controlledCharacter, QObject* parent = nullptr);
    ~AIController();

    // 基本控制接口
    void setEnabled(bool enabled);
    bool isEnabled() const { return aiEnabled; }
    
    // 直接角色操作接口
    void moveLeft();
    void moveRight();
    void jump();
    void attack();
    void pickup();
    void squat();
    void stopMovement();

    void universalAction(bool left, bool right, bool up, bool down, bool attack, bool pickup);

private:
    // 核心组件
    Character* character;           // 控制的角色
    bool aiEnabled;                 // AI是否启用
};

#endif //QT_PROGRAMMING_2024_AICONTROLLER_H

