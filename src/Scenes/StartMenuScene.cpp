//
// Created by AI Assistant on 7/25/25
//

#include "StartMenuScene.h"
#include <QGraphicsSceneMouseEvent>
#include <QFont>
#include <QBrush>
#include <QPen>
#include <QDebug>

StartMenuScene::StartMenuScene(QObject *parent)
    : Scene(parent)
    , backgroundItem(nullptr)
    , titleText(nullptr)
    , subtitleText(nullptr)
    , playButtonBg(nullptr)
    , playButtonText(nullptr)
    , instructionText(nullptr)
    , isPlayButtonHovered(false)
    , animationTimer(new QTimer(this))
    , titleOpacity(1.0)
    , titleFadingIn(false)
{
    setSceneRect(0, 0, 960, 640);
    setupUI();
    
    // 设置动画定时器
    connect(animationTimer, &QTimer::timeout, this, &StartMenuScene::updateButtonState);
    animationTimer->start(50); // 20 FPS for smooth animation
}

StartMenuScene::~StartMenuScene() {
    if (animationTimer) {
        animationTimer->stop();
    }
}

void StartMenuScene::setupUI() {
    createBackground();
    createTitle();
    createPlayButton();
}

void StartMenuScene::createBackground() {
    // 创建背景矩形，使用深色背景
    QGraphicsRectItem *bg = addRect(0, 0, 960, 640, QPen(Qt::NoPen), QBrush(QColor(20, 20, 30)));
    bg->setZValue(-100); // 确保背景在最底层
    
    // 尝试加载背景图片（如果有的话）
    QPixmap backgroundPixmap(":/Backgrounds/ChooseBattlefieldSceneBackground.png");
    if (!backgroundPixmap.isNull()) {
        backgroundItem = addPixmap(backgroundPixmap);
        backgroundItem->setZValue(-50);
        // 缩放背景图片以适应场景
        qreal scaleX = 960.0 / backgroundPixmap.width();
        qreal scaleY = 640.0 / backgroundPixmap.height();
        qreal scale = qMax(scaleX, scaleY);
        backgroundItem->setScale(scale);
        
        // 居中背景
        qreal x = (960 - backgroundPixmap.width() * scale) / 2;
        qreal y = (640 - backgroundPixmap.height() * scale) / 2;
        backgroundItem->setPos(x, y);
        
        // 添加半透明遮罩
        QGraphicsRectItem *overlay = addRect(0, 0, 960, 640, QPen(Qt::NoPen), QBrush(QColor(0, 0, 0, 150)));
        overlay->setZValue(-25);
    }
}

void StartMenuScene::createTitle() {
    // 创建主标题
    titleText = addText("The Battle of Spirits", QFont("Agency FB", 48, QFont::Bold));
    titleText->setDefaultTextColor(QColor(255, 255, 255));
    titleText->setZValue(10);
    
    // 居中标题
    QRectF titleBounds = titleText->boundingRect();
    titleText->setPos((960 - titleBounds.width()) / 2, 120);
    
    // 创建副标题
    subtitleText = addText("2D Battle Game", QFont("Agency FB", 24, QFont::Normal));
    subtitleText->setDefaultTextColor(QColor(200, 200, 200));
    subtitleText->setZValue(10);
    
    // 居中副标题
    QRectF subtitleBounds = subtitleText->boundingRect();
    subtitleText->setPos((960 - subtitleBounds.width()) / 2, 180);
}

void StartMenuScene::createPlayButton() {
    // Play按钮的位置和大小
    qreal buttonWidth = 200;
    qreal buttonHeight = 60;
    qreal buttonX = (960 - buttonWidth) / 2;
    qreal buttonY = 320;
    
    playButtonRect = QRectF(buttonX, buttonY, buttonWidth, buttonHeight);
    
    // 创建按钮背景
    playButtonBg = addRect(playButtonRect, QPen(QColor(255, 255, 255), 3), QBrush(QColor(50, 50, 70, 200)));
    playButtonBg->setZValue(5);
    
    // 创建按钮文字
    playButtonText = addText("PLAY", QFont("Agency FB", 28, QFont::Bold));
    playButtonText->setDefaultTextColor(QColor(255, 255, 255));
    playButtonText->setZValue(10);
    
    // 居中按钮文字
    QRectF textBounds = playButtonText->boundingRect();
    playButtonText->setPos(buttonX + (buttonWidth - textBounds.width()) / 2,
                          buttonY + (buttonHeight - textBounds.height()) / 2);
    
    // 创建操作说明
    instructionText = addText("Click PLAY to start or press ENTER", QFont("Agency FB", 16));
    instructionText->setDefaultTextColor(QColor(150, 150, 150));
    instructionText->setZValue(10);
    
    // 居中操作说明
    QRectF instructionBounds = instructionText->boundingRect();
    instructionText->setPos((960 - instructionBounds.width()) / 2, 450);
}

void StartMenuScene::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
        case Qt::Key_Space:
            emit playButtonClicked();
            break;
        default:
            Scene::keyPressEvent(event);
            break;
    }
}

void StartMenuScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (isPointInPlayButton(event->scenePos())) {
            emit playButtonClicked();
        }
    }
    Scene::mousePressEvent(event);
}

bool StartMenuScene::isPointInPlayButton(const QPointF &point) const {
    return playButtonRect.contains(point);
}

void StartMenuScene::update() {
    Scene::update();
    // 可以在这里添加其他更新逻辑
}

void StartMenuScene::updateButtonState() {
    // 标题呼吸效果
    if (titleFadingIn) {
        titleOpacity += 0.02;
        if (titleOpacity >= 1.0) {
            titleOpacity = 1.0;
            titleFadingIn = false;
        }
    } else {
        titleOpacity -= 0.02;
        if (titleOpacity <= 0.6) {
            titleOpacity = 0.6;
            titleFadingIn = true;
        }
    }
    
    if (titleText) {
        titleText->setOpacity(titleOpacity);
    }
}
