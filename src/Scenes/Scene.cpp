//
// Created by gerw on 8/20/24.
//

#include <QDateTime>
#include "Scene.h"

Scene::Scene(QObject *parent) : QGraphicsScene(parent), timer(new QTimer(this)) {
    connect(timer, &QTimer::timeout, this, &Scene::update);
}

void Scene::update() {
    auto currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    if (lastTime == -1) { // first frame
        deltaTime = 0;
    } else {
        deltaTime = currentTime - lastTime;
    }
    lastTime = currentTime;

    processInput();
    processMovement();
}

void Scene::startLoop() {
    timer->start(1000 / 90); // 90FPS
}

void Scene::stopLoop() {
    timer->stop();
    lastTime = -1; // 重置时间，为下次启动做准备
}

void Scene::processInput() {

}

void Scene::processMovement() {

}



