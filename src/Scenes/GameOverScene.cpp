//
// Created by AI Assistant on 7/24/25.
//

#include "GameOverScene.h"
#include <QBrush>
#include <QPen>
#include <QFont>
#include <QGraphicsDropShadowEffect>

GameOverScene::GameOverScene(QObject *parent, const QString& winner)
    : Scene(parent), winnerName(winner) {
    
    // 设置场景大小
    setSceneRect(0, 0, 960, 640);
    
    // 设置背景图片
    QPixmap backgroundPixmap(":/Backgrounds/GameOverBackground.png");
    if (!backgroundPixmap.isNull()) {
        // 缩放背景图片以适应场景大小
        backgroundPixmap = backgroundPixmap.scaled(960, 640, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        setBackgroundBrush(QBrush(backgroundPixmap));
    } else {
        // 如果背景图片加载失败，使用默认的半透明黑色背景
        setBackgroundBrush(QBrush(QColor(0, 0, 0, 150)));
    }
    
    setupUI();
    updateDisplay();
}

void GameOverScene::setupUI() {   
    // 创建标题文本 "GAME OVER"
    titleText = new QGraphicsTextItem();
    titleText->setPlainText("GAME OVER");
    titleText->setFont(QFont("Agency FB", 56, QFont::Bold)); // 增大字体
    titleText->setDefaultTextColor(QColor(220, 20, 20)); // 深红色，更有冲击力
    // 添加文字阴影效果
    QGraphicsDropShadowEffect* titleShadow = new QGraphicsDropShadowEffect();
    titleShadow->setBlurRadius(8);
    titleShadow->setColor(QColor(0, 0, 0, 180));
    titleShadow->setOffset(3, 3);
    titleText->setGraphicsEffect(titleShadow);
    addItem(titleText);
    
    // 创建获胜者文本
    winnerText = new QGraphicsTextItem();
    winnerText->setFont(QFont("Agency FB", 28, QFont::Bold)); // 增大字体
    // 添加文字阴影效果
    QGraphicsDropShadowEffect* winnerShadow = new QGraphicsDropShadowEffect();
    winnerShadow->setBlurRadius(6);
    winnerShadow->setColor(QColor(0, 0, 0, 160));
    winnerShadow->setOffset(2, 2);
    winnerText->setGraphicsEffect(winnerShadow);
    addItem(winnerText);
    
    // 创建操作指示文本
    instructionText = new QGraphicsTextItem();
    instructionText->setPlainText("Press R to Restart Battle\nPress M to Return to Map Selection\nPress ESC to Return to Mode Selection");
    instructionText->setFont(QFont("Agency FB", 18, QFont::Normal)); // 稍微增大字体
    instructionText->setDefaultTextColor(QColor(255, 255, 0)); // 白色
    // 添加文字阴影效果
    QGraphicsDropShadowEffect* instructionShadow = new QGraphicsDropShadowEffect();
    instructionShadow->setBlurRadius(4);
    instructionShadow->setColor(QColor(0, 0, 0, 140));
    instructionShadow->setOffset(1, 1);
    instructionText->setGraphicsEffect(instructionShadow);
    addItem(instructionText);
}

void GameOverScene::updateDisplay() {
    // 设置标题位置
    QRectF titleRect = titleText->boundingRect();
    titleText->setPos((960 - titleRect.width()) / 2, 120); // 稍微上移
    
    // 设置获胜者文本
    QString winnerMessage;
    if (winnerName.isEmpty()) {
        winnerMessage = "Draw Game!";
        winnerText->setDefaultTextColor(QColor(200, 200, 200)); // 浅灰色表示平局
    } else {
        winnerMessage = QString("%1 Wins!").arg(winnerName);
        winnerText->setDefaultTextColor(QColor(255, 215, 0)); // 金色表示获胜
    }
    winnerText->setPlainText(winnerMessage);
    
    // 设置获胜者文本位置
    QRectF winnerRect = winnerText->boundingRect();
    winnerText->setPos((960 - winnerRect.width()) / 2, 240); // 调整位置
    
    // 设置指示文本位置
    QRectF instructionRect = instructionText->boundingRect();
    instructionText->setPos((960 - instructionRect.width()) / 2, 450); // 稍微下移
}

void GameOverScene::processInput() {
    // 游戏结束场景不需要持续的输入处理
    // 输入主要通过按键事件处理
}

void GameOverScene::processMovement() {
    // 游戏结束场景不需要移动处理
}

void GameOverScene::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_R:
            // 重新开始战斗
            emit restartBattle();
            break;
        case Qt::Key_M:
            // 返回地图选择
            emit returnToMapSelection();
            break;
        case Qt::Key_Escape:
            // 返回游戏模式选择
            emit returnToModeSelection();
            break;
        default:
            Scene::keyPressEvent(event);
            break;
    }
}
