//
// Created by AI Assistant on 7/24/25
//

#include "GameModeSelectionScene.h"
#include <QBrush>
#include <QPen>

GameModeSelectionScene::GameModeSelectionScene(QObject *parent)
    : Scene(parent)
    , titleText(nullptr)
    , instructionText(nullptr)
    , backgroundRect(nullptr)
    , currentSelection(0)
    , blinkTimer(new QTimer(this))
    , blinkState(false)
    , blinkCounter(0)
{
    setSceneRect(0, 0, 960, 640);
    
    // 初始化UI
    initializeUI();
    
    // 设置闪烁定时器
    connect(blinkTimer, &QTimer::timeout, this, &GameModeSelectionScene::updateSelection);
    blinkTimer->start(500); // 每500ms闪烁一次
}

GameModeSelectionScene::~GameModeSelectionScene() {
}

void GameModeSelectionScene::initializeUI() {
    setupBackground();
    setupModeOptions();
    setupInstructions();
    updateSelectionHighlight();
}

void GameModeSelectionScene::setupBackground() {
    // 创建背景图片
    QPixmap backgroundPixmap(":/Backgrounds/GameModeSelectionSceneBackground.png");
    if (!backgroundPixmap.isNull()) {
        // 缩放背景图片到场景大小
        backgroundPixmap = backgroundPixmap.scaled(sceneRect().size().toSize(), 
                                                 Qt::IgnoreAspectRatio, 
                                                 Qt::SmoothTransformation);
        QGraphicsPixmapItem* backgroundImage = new QGraphicsPixmapItem(backgroundPixmap);
        backgroundImage->setPos(0, 0);
        addItem(backgroundImage);
    } else {
        // 如果图片加载失败，使用纯色背景作为后备
        backgroundRect = new QGraphicsRectItem(sceneRect());
        backgroundRect->setBrush(QBrush(QColor(20, 20, 40))); // 深蓝色背景
        backgroundRect->setPen(QPen(Qt::NoPen));
        addItem(backgroundRect);
    }
    
    // 创建标题
    titleText = new QGraphicsTextItem("Select Game Mode");
    titleText->setFont(getTitleFont());
    titleText->setDefaultTextColor(QColor(0, 255, 255));
    
    // 居中标题
    QRectF titleBounds = titleText->boundingRect();
    titleText->setPos((sceneRect().width() - titleBounds.width()) / 2, 80);
    addItem(titleText);
}

void GameModeSelectionScene::setupModeOptions() {
    // 创建模式选项
    modeOptions.clear();
    modeOptions.append(ModeOption(GameMode::PvP));
    modeOptions.append(ModeOption(GameMode::PvE));
    modeOptions.append(ModeOption(GameMode::Single));
    
    qreal startY = 200;
    qreal optionHeight = 120;
    qreal optionWidth = 700;
    qreal startX = (sceneRect().width() - optionWidth) / 2;
    
    for (int i = 0; i < modeOptions.size(); ++i) {
        ModeOption& option = modeOptions[i];
        qreal optionY = startY + i * optionHeight;
        
        // 创建选项背景
        option.backgroundItem = new QGraphicsRectItem(startX, optionY, optionWidth, optionHeight - 10);
        option.backgroundItem->setBrush(QBrush(QColor(40, 40, 60, 128)));
        option.backgroundItem->setPen(QPen(QColor(100, 100, 150), 2));
        addItem(option.backgroundItem);
        
        // 创建模式标题
        QString modeTitle;
        switch (option.mode) {
            case GameMode::PvP:
                modeTitle = "🥊 PvP Mode - Player vs Player";
                break;
            case GameMode::PvE:
                modeTitle = "🤖 PvE Mode - Player vs AI";
                break;
            case GameMode::Single:
                modeTitle = "🏃 Practice Mode - Single Player";
                break;
        }
        
        option.titleItem = new QGraphicsTextItem(modeTitle);
        option.titleItem->setFont(getDescriptionFont());
        option.titleItem->setDefaultTextColor(QColor(255, 255, 255));
        option.titleItem->setPos(startX + 20, optionY + 10);
        addItem(option.titleItem);
        
        // 创建模式描述
        option.descItem = new QGraphicsTextItem(GameModeUtils::getGameModeDescription(option.mode));
        option.descItem->setFont(getInstructionFont());
        option.descItem->setDefaultTextColor(QColor(200, 200, 200));
        option.descItem->setPos(startX + 20, optionY + 50);
        addItem(option.descItem);
        
        // 设置边界
        option.bounds = QRectF(startX, optionY, optionWidth, optionHeight - 10);
    }
}

void GameModeSelectionScene::setupInstructions() {
    // 创建操作说明
    QString instructions = "↑↓ Select Mode    ENTER Confirm";
    instructionText = new QGraphicsTextItem(instructions);
    instructionText->setFont(getInstructionFont());
    instructionText->setDefaultTextColor(QColor(150, 150, 150));
    
    // 居中说明文本
    QRectF instructBounds = instructionText->boundingRect();
    instructionText->setPos((sceneRect().width() - instructBounds.width()) / 2, 
                           sceneRect().height() - 80);
    addItem(instructionText);
}

void GameModeSelectionScene::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Up:
            handleSelectionChange(-1);
            break;
        case Qt::Key_Down:
            handleSelectionChange(1);
            break;
        case Qt::Key_Return:
        case Qt::Key_Enter:
            handleModeSelection();
            break;
        case Qt::Key_Escape:
            emit returnToMainMenu();
            break;
        default:
            Scene::keyPressEvent(event);
            break;
    }
}

void GameModeSelectionScene::update() {
    Scene::update();
}

void GameModeSelectionScene::updateSelection() {
    blinkState = !blinkState;
    blinkCounter++;
    updateSelectionHighlight();
}

void GameModeSelectionScene::handleSelectionChange(int direction) {
    int newSelection = currentSelection + direction;
    
    // 循环选择
    if (newSelection < 0) {
        newSelection = modeOptions.size() - 1;
    } else if (newSelection >= modeOptions.size()) {
        newSelection = 0;
    }
    
    if (newSelection != currentSelection) {
        // 取消高亮当前选择
        setOptionHighlighted(currentSelection, false);
        
        // 更新选择
        currentSelection = newSelection;
        
        // 高亮新选择
        updateSelectionHighlight();
        
        // 重置闪烁
        blinkCounter = 0;
        blinkState = true;
    }
}

void GameModeSelectionScene::handleModeSelection() {
    if (currentSelection >= 0 && currentSelection < modeOptions.size()) {
        GameMode selectedMode = modeOptions[currentSelection].mode;
        emit gameModeSelected(selectedMode);
    }
}

void GameModeSelectionScene::updateSelectionHighlight() {
    // 更新所有选项的高亮状态
    for (int i = 0; i < modeOptions.size(); ++i) {
        bool isSelected = (i == currentSelection);
        bool shouldHighlight = isSelected && (blinkState || blinkCounter % 4 < 2);
        setOptionHighlighted(i, shouldHighlight);
    }
}

void GameModeSelectionScene::setOptionHighlighted(int index, bool highlighted) {
    if (index < 0 || index >= modeOptions.size()) return;
    
    ModeOption& option = modeOptions[index];
    
    if (highlighted) {
        // 高亮状态
        option.backgroundItem->setBrush(QBrush(QColor(80, 120, 200, 180)));
        option.backgroundItem->setPen(QPen(QColor(120, 160, 255), 3));
        option.titleItem->setDefaultTextColor(QColor(255, 255, 100));
        option.descItem->setDefaultTextColor(QColor(255, 255, 255));
    } else {
        // 普通状态
        option.backgroundItem->setBrush(QBrush(QColor(40, 40, 60, 128)));
        option.backgroundItem->setPen(QPen(QColor(100, 100, 150), 2));
        option.titleItem->setDefaultTextColor(QColor(255, 255, 255));
        option.descItem->setDefaultTextColor(QColor(200, 200, 200));
    }
}

QColor GameModeSelectionScene::getSelectionColor() const {
    return blinkState ? QColor(255, 255, 100) : QColor(255, 255, 255);
}

QFont GameModeSelectionScene::getTitleFont() const {
    QFont font("Agency FB", 32, QFont::Bold);
    return font;
}

QFont GameModeSelectionScene::getDescriptionFont() const {
    QFont font("Agency FB", 18, QFont::Bold);
    return font;
}

QFont GameModeSelectionScene::getInstructionFont() const {
    QFont font("Agency FB", 14);
    return font;
}

#include "GameModeSelectionScene.moc"
