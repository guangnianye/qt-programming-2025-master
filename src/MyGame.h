//
// Created by gerw on 8/20/24.
//

#ifndef QT_PROGRAMMING_2024_MYGAME_H
#define QT_PROGRAMMING_2024_MYGAME_H

#include <QGraphicsView>
#include <QMainWindow>
#include "Scenes/Scene.h"
#include "Scenes/StartMenuScene.h"
#include "Scenes/GameModeSelectionScene.h"
#include "Scenes/ChooseBattlefieldScene.h"
#include "Scenes/BattleScene.h"
#include "Scenes/GameOverScene.h"
#include "GameMode.h"

class MyGame : public QMainWindow {
Q_OBJECT

public:
    explicit MyGame(QWidget *parent = nullptr);

private slots:
    void onStartGame();
    void onGameModeSelected(GameMode mode);
    void onMapSelected(int mapId);
    void onGameOver(const QString& winner);
    void onRestartBattle();
    void onReturnToModeSelection();
    void onReturnToStartMenu();

private:
    void switchToStartMenuScene();
    void switchToGameModeSelectionScene();
    void switchToChooseMapScene();
    void switchToBattleScene(int mapId);
    void switchToGameOverScene(const QString& winner);
    
    Scene *currentScene{nullptr};
    StartMenuScene *startMenuScene{nullptr};
    GameModeSelectionScene *gameModeSelectionScene{nullptr};
    ChooseBattlefieldScene *chooseMapScene{nullptr};
    BattleScene *battleScene{nullptr};
    GameOverScene *gameOverScene{nullptr};
    QGraphicsView *view{nullptr};
    GameMode currentGameMode{GameMode::PvP}; // 当前游戏模式
    int currentMapId{1}; // 记录当前地图ID，用于重新开始战斗
};


#endif //QT_PROGRAMMING_2024_MYGAME_H
