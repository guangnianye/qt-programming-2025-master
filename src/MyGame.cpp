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
    
    
    // 开始时进入游戏模式选择场景
    switchToGameModeSelectionScene();
}

void MyGame::onGameModeSelected(GameMode mode) {
    qDebug() << "MyGame received game mode selection:" << static_cast<int>(mode);
    currentGameMode = mode;
    switchToChooseMapScene();
}

void MyGame::onMapSelected(int mapId) {
    qDebug() << "MyGame received map selection:" << mapId;
    currentMapId = mapId; // 记录当前地图ID
    switchToBattleScene(mapId);
}

void MyGame::switchToGameModeSelectionScene() {
    qDebug() << "Switching to game mode selection scene";
    
    // 停止当前场景的循环
    if (currentScene) {
        currentScene->stopLoop();
    }
    
    // 创建游戏模式选择场景
    if (!gameModeSelectionScene) {
        qDebug() << "Creating new GameModeSelectionScene";
        gameModeSelectionScene = new GameModeSelectionScene(this);
        // 连接信号
        connect(gameModeSelectionScene, &GameModeSelectionScene::gameModeSelected, 
                this, &MyGame::onGameModeSelected);
    }
    
    // 切换场景
    currentScene = gameModeSelectionScene;
    view->setScene(gameModeSelectionScene);
    
    // 开始场景循环
    gameModeSelectionScene->startLoop();
    
    qDebug() << "Switched to game mode selection scene";
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
        connect(chooseMapScene, &ChooseBattlefieldScene::returnToModeSelection,
                this, &MyGame::onReturnToModeSelection);
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
    battleScene = new BattleScene(this,mapId,currentGameMode); // 传递游戏模式和地图ID
    
    // 连接返回信号
    connect(battleScene, &BattleScene::returnToMapSelection,
            this, &MyGame::switchToChooseMapScene);
    
    // 连接游戏结束信号
    connect(battleScene, &BattleScene::gameOver,
            this, &MyGame::onGameOver);
    
    // 切换场景
    currentScene = battleScene;
    view->setScene(battleScene);
    battleScene->startLoop();
}

void MyGame::onGameOver(const QString& winner) {
    qDebug() << "MyGame received game over signal, winner:" << winner;
    switchToGameOverScene(winner);
}

void MyGame::onRestartBattle() {
    qDebug() << "MyGame received restart battle signal";
    switchToBattleScene(currentMapId);
}

void MyGame::onReturnToModeSelection() {
    qDebug() << "MyGame received return to mode selection signal";
    switchToGameModeSelectionScene();
}

void MyGame::switchToGameOverScene(const QString& winner) {
    qDebug() << "Switching to game over scene, winner:" << winner;
    
    // 停止当前场景的循环
    if (currentScene) {
        currentScene->stopLoop();
    }
    
    // 创建游戏结束场景
    if (gameOverScene) {
        delete gameOverScene;
    }
    gameOverScene = new GameOverScene(this, winner);
    
    // 连接信号
    connect(gameOverScene, &GameOverScene::returnToMapSelection,
            this, &MyGame::switchToChooseMapScene);
    connect(gameOverScene, &GameOverScene::returnToModeSelection,
            this, &MyGame::onReturnToModeSelection);
    connect(gameOverScene, &GameOverScene::restartBattle,
            this, &MyGame::onRestartBattle);
    
    // 切换场景
    currentScene = gameOverScene;
    view->setScene(gameOverScene);
    gameOverScene->startLoop();
}
