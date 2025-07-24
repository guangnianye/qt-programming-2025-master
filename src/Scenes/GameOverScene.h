//
// Created by AI Assistant on 7/24/25.
//

#ifndef QT_PROGRAMMING_2024_GAMEOVERSCENE_H
#define QT_PROGRAMMING_2024_GAMEOVERSCENE_H

#include "Scene.h"
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QKeyEvent>

class GameOverScene : public Scene {
Q_OBJECT

public:
    explicit GameOverScene(QObject *parent, const QString& winner = "");

    void processInput() override;
    void processMovement() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void setupUI();
    void updateDisplay();
    
    QString winnerName;
    QGraphicsTextItem *titleText;
    QGraphicsTextItem *winnerText;
    QGraphicsTextItem *instructionText;
    QGraphicsRectItem *backgroundRect;

signals:
    void returnToMapSelection();
    void returnToModeSelection();
    void restartBattle();
};

#endif //QT_PROGRAMMING_2024_GAMEOVERSCENE_H
