//
// Created by gerw on 8/20/24.
//
#include "MyGame.h"
#include <QDebug>

MyGame::MyGame(QWidget *parent) : QMainWindow(parent) {
    // 创建图形视图
    view = new QGraphicsView(this);
    view->setFixedSize(960, 640);  // 设置固定大小
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    // 设置视图的一些属性以确保正确显示
    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view->setRenderHint(QPainter::Antialiasing);
    
    setCentralWidget(view);
    setFixedSize(960, 640);  // 直接设置窗口大小而不是依赖view->sizeHint()
    
    
    // 开始时进入地图选择场景
    switchToChooseMapScene();
}

void MyGame::onMapSelected(int mapId) {
    qDebug() << "MyGame received map selection:" << mapId;
    switchToBattleScene(mapId);
}

void MyGame::switchToChooseMapScene() {
    qDebug() << "Switching to choose map scene";
    
    // 停止当前场景的循环
    if (currentScene) {
        currentScene->stopLoop();
    }
    
    // 创建地图选择场景
    if (!chooseMapScene) {
        qDebug() << "Creating new ChooseBattlefieldScene";
        chooseMapScene = new ChooseBattlefieldScene(this);
        // 连接信号
        connect(chooseMapScene, &ChooseBattlefieldScene::mapSelected, 
                this, &MyGame::onMapSelected);
    }
    
    // 切换场景
    currentScene = chooseMapScene;
    view->setScene(chooseMapScene);
    qDebug() << "Scene set to view, scene rect:" << chooseMapScene->sceneRect();
    qDebug() << "View viewport rect:" << view->viewport()->rect();
    
    chooseMapScene->startLoop();
    qDebug() << "Choose map scene loop started";
}

void MyGame::switchToBattleScene(int mapId) {
    qDebug() << "Switching to battle scene with map ID:" << mapId;
    
    // 停止当前场景的循环
    if (currentScene) {
        currentScene->stopLoop();
    }
    
    // 创建战斗场景（每次重新创建以确保状态重置）
    if (battleScene) {
        delete battleScene;
    }
    battleScene = new BattleScene(this);
    
    // 连接返回信号
    connect(battleScene, &BattleScene::returnToMapSelection,
            this, &MyGame::switchToChooseMapScene);
    
    // 切换场景
    currentScene = battleScene;
    view->setScene(battleScene);
    battleScene->startLoop();
}
