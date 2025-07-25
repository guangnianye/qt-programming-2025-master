//
// Created by gerw on 8/20/24.
//
#include "MyGame.h"

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
    
    
    // 开始时进入开始菜单场景
    switchToStartMenuScene();
}

void MyGame::onStartGame() {
    switchToGameModeSelectionScene();
}

void MyGame::onGameModeSelected(GameMode mode) {
    currentGameMode = mode;
    switchToChooseMapScene();
}

void MyGame::onMapSelected(int mapId) {
    currentMapId = mapId; // 记录当前地图ID
    switchToBattleScene(mapId);
}

void MyGame::switchToStartMenuScene() {
    // 停止当前场景的循环
    if (currentScene) {
        currentScene->stopLoop();
    }
    
    // 创建开始菜单场景
    if (!startMenuScene) {
        startMenuScene = new StartMenuScene(this);
        // 连接信号
        connect(startMenuScene, &StartMenuScene::playButtonClicked, 
                this, &MyGame::onStartGame);
    }
    
    // 切换场景
    currentScene = startMenuScene;
    view->setScene(startMenuScene);
    
    // 开始场景循环
    startMenuScene->startLoop();
}

void MyGame::switchToGameModeSelectionScene() {
    // 停止当前场景的循环
    if (currentScene) {
        currentScene->stopLoop();
    }
    
    // 创建游戏模式选择场景
    if (!gameModeSelectionScene) {
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
    
}

void MyGame::switchToChooseMapScene() {
    // 停止当前场景的循环
    if (currentScene) {
        currentScene->stopLoop();
    }
    
    // 创建地图选择场景
    if (!chooseMapScene) {
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
    
    chooseMapScene->startLoop();
}

void MyGame::switchToBattleScene(int mapId) {
    
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
    switchToGameOverScene(winner);
}

void MyGame::onRestartBattle() {
    switchToBattleScene(currentMapId);
}

void MyGame::onReturnToModeSelection() {
    switchToGameModeSelectionScene();
}

void MyGame::onReturnToStartMenu() {
    switchToStartMenuScene();
}

void MyGame::switchToGameOverScene(const QString& winner) {
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
    connect(gameOverScene, &GameOverScene::returnToStartMenu,
            this, &MyGame::onReturnToStartMenu);
    
    // 切换场景
    currentScene = gameOverScene;
    view->setScene(gameOverScene);
    gameOverScene->startLoop();
}
