//
// Created by gerw on 8/20/24.
//

#ifndef QT_PROGRAMMING_2024_MYGAME_H
#define QT_PROGRAMMING_2024_MYGAME_H

#include <QGraphicsView>
#include <QMainWindow>
#include "Scenes/Scene.h"
#include "Scenes/ChooseBattlefieldScene.h"
#include "Scenes/BattleScene.h"
#include "Scenes/GameOverScene.h"

class MyGame : public QMainWindow {
Q_OBJECT

public:
    explicit MyGame(QWidget *parent = nullptr);

private slots:
    void onMapSelected(int mapId);
    void onGameOver(const QString& winner);
    void onRestartBattle();

private:
    void switchToChooseMapScene();
    void switchToBattleScene(int mapId);
    void switchToGameOverScene(const QString& winner);
    
    Scene *currentScene{nullptr};
    ChooseBattlefieldScene *chooseMapScene{nullptr};
    BattleScene *battleScene{nullptr};
    GameOverScene *gameOverScene{nullptr};
    QGraphicsView *view{nullptr};
    int currentMapId{1}; // 记录当前地图ID，用于重新开始战斗
};


#endif //QT_PROGRAMMING_2024_MYGAME_H
