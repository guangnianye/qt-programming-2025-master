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

class MyGame : public QMainWindow {
Q_OBJECT

public:
    explicit MyGame(QWidget *parent = nullptr);

private slots:
    void onMapSelected(int mapId);

private:
    void switchToChooseMapScene();
    void switchToBattleScene(int mapId);
    
    Scene *currentScene{nullptr};
    ChooseBattlefieldScene *chooseMapScene{nullptr};
    BattleScene *battleScene{nullptr};
    QGraphicsView *view{nullptr};
};


#endif //QT_PROGRAMMING_2024_MYGAME_H
