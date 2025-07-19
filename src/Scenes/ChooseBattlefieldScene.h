#ifndef QT_PROGRAMMING_2024_CHOOSEBATTLEFIELD_SCENE_H
#define QT_PROGRAMMING_2024_CHOOSEBATTLEFIELD_SCENE_H

#include <QKeyEvent>
#include <QMouseEvent>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include "Scene.h"

class ChooseBattlefieldScene : public Scene {
Q_OBJECT

public:
    explicit ChooseBattlefieldScene(QObject *parent);

    void processInput() override;

protected slots:
    void update() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    void setupUI();
    void updateButtonStyle(QGraphicsRectItem *button, int index);
    void updateSelection(int newIndex);
    void handleMapSelection(int mapId);
    void startBattle(int mapId);
    
    // UI元素
    QGraphicsTextItem *titleText{nullptr};
    QList<QGraphicsRectItem*> mapButtons;
    QList<QGraphicsTextItem*> mapButtonTexts;
    QGraphicsPixmapItem *backgroundImage{nullptr}; // 为将来的背景图片准备
    
    // 状态
    int selectedMapIndex{0}; // 当前选中的地图索引

signals:
    void mapSelected(int mapId);

};

#endif //QT_PROGRAMMING_2024_CHOOSEBATTLEFIELD_SCENE_H