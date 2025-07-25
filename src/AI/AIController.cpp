//
// Created by linye on 7/25/25.
//

#include "AIController.h"
#include <QDebug>

AIController::AIController(Character* controlledCharacter, QObject* parent)
    : QObject(parent), character(controlledCharacter), aiEnabled(false) {
    if (!character) {
        qWarning() << "AIController: Controlled character is null";
    }
}

AIController::~AIController() {
    qDebug() << "AIController destroyed";
}

void AIController::setEnabled(bool enabled) {
    aiEnabled = enabled;
    if (character) {
        character->setAttacking(enabled); // 示例：启用时角色开始攻击
        qDebug() << "AIController: AI" << (enabled ? "enabled" : "disabled");
    } else {
        qWarning() << "AIController: No character to control";
    }
}

#pragma region Foudation Action Control

void AIController::moveLeft() {
    if (character) {
        character->setLeftDown(true);
        qDebug() << "AIController: Moving left";
    }
}

void AIController::moveRight() {
    if (character) {
        character->setRightDown(true);
        qDebug() << "AIController: Moving right";
    }
}

void AIController::jump() {
    if (character) {
        character->setUpDown(true);
        qDebug() << "AIController: Jumping";
    }
}

void AIController::attack() {
    if (character) {
        character->setAttacking(true);
        qDebug() << "AIController: Attacking";
    }
}

void AIController::pickup() {
    if (character) {
        character->setPickDown(true);
        qDebug() << "AIController: Picking up item";
    }
}

void AIController::squat() {
    if (character) {
        character->setSquatting(true);
        qDebug() << "AIController: Squatting";
    }
}

void AIController::stopMovement() {
    if (character) {
        character->setLeftDown(false);
        character->setRightDown(false);
        character->setUpDown(false);
        character->setSquatting(false);
        character->setPickDown(false);
        qDebug() << "AIController: Stopping movement";
    }
}

void AIController::universalAction(bool left, bool right, bool up, bool down, bool attack, bool pickup) {
    if (character) {
        character->setLeftDown(left);
        character->setRightDown(right);
        character->setUpDown(up);
        character->setSquatting(down);
        character->setAttacking(attack);
        character->setPickDown(pickup);
        qDebug() << "AIController: Universal action executed";
    } else {
        qWarning() << "AIController: No character to perform universal action";
    }
}

#pragma endregion