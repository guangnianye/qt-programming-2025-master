//
// Created by AI Assistant on 7/25/25
//

#ifndef QT_PROGRAMMING_2024_STARTMENUSCENE_H
#define QT_PROGRAMMING_2024_STARTMENUSCENE_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QKeyEvent>
#include <QTimer>
#include <QMouseEvent>
#include "Scene.h"

/**
 * @brief 开始菜单场景
 * 
 * 游戏的主菜单界面，包含游戏标题和开始按钮
 */
class StartMenuScene : public Scene {
    Q_OBJECT

public:
    explicit StartMenuScene(QObject *parent = nullptr);
    ~StartMenuScene();

signals:
    void playButtonClicked();  // 当点击Play按钮时发出的信号

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private slots:
    void update() override;
    void updateButtonState();

private:
    void setupUI();
    void createBackground();
    void createTitle();
    void createPlayButton();
    bool isPointInPlayButton(const QPointF &point) const;
    
    // UI元素
    QGraphicsPixmapItem *backgroundItem;    // 背景图片
    QGraphicsTextItem *titleText;          // 游戏标题
    QGraphicsTextItem *subtitleText;       // 副标题
    QGraphicsRectItem *playButtonBg;       // Play按钮背景
    QGraphicsTextItem *playButtonText;     // Play按钮文字
    QGraphicsTextItem *instructionText;    // 操作说明文字
    
    // 按钮状态
    bool isPlayButtonHovered;              // Play按钮是否被悬停
    QRectF playButtonRect;                 // Play按钮的矩形区域
    
    // 动画效果
    QTimer *animationTimer;                // 动画定时器
    qreal titleOpacity;                    // 标题透明度（用于呼吸效果）
    bool titleFadingIn;                    // 标题是否正在淡入
};

#endif //QT_PROGRAMMING_2024_STARTMENUSCENE_H
