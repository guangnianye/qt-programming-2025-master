//
// Created by guangnianye on 7/20/25
//

#include "ChooseBattlefieldScene.h"
#include "BattleScene.h"
#include <QGraphicsSceneMouseEvent>
#include <QFont>
#include <QFontInfo>
#include <QFontDatabase>
#include <QGraphicsDropShadowEffect>
#include <QPixmap>
#include <QBrush>
#include <QPen>
#include <QMainWindow>
#include <QCoreApplication>
#include <QLinearGradient>
#include <QGraphicsRectItem>

ChooseBattlefieldScene::ChooseBattlefieldScene(QObject *parent) : Scene(parent) {
    // 设置场景尺寸
    setSceneRect(0, 0, 960, 640);
    
    // 加载背景图片
    QPixmap backgroundPixmap(":/Backgrounds/ChooseBattlefieldSceneBackground.png");
    if (!backgroundPixmap.isNull()) {
        // 如果图片加载成功，设置为背景
        backgroundImage = new QGraphicsPixmapItem(backgroundPixmap);
        backgroundImage->setPos(0, 0);
        addItem(backgroundImage);
    } else {
        // 如果图片加载失败，使用纯色背景作为备用
        setBackgroundBrush(QBrush(QColor(240, 248, 255)));
    }
    
    // 初始化选中索引
    selectedMapIndex = 0;
    
    // 打印可用的字体信息
    QFontDatabase fontDb;
    QStringList fontFamilies = fontDb.families();
    
    // 初始化UI
    setupUI();
}

void ChooseBattlefieldScene::setupUI() {
    // 艺术字体标题
    titleText = new QGraphicsTextItem("Choose Map !");
    titleText->setDefaultTextColor(QColor(255, 105, 180)); // 可爱的粉色
    
    // 尝试多种艺术字体，优先级从高到低
    QStringList fontFamilies = {
        "Papyrus",       // 艺术字体
        "Comic Sans MS", // 可爱字体
        "Chalkduster",   // 粉笔字体
        "Bradley Hand ITC", // 手写体
        "Segoe Print",   // 印刷体
        "Arial"          // 备用字体
    };
    
    QFont titleFont;
    for (const QString &family : fontFamilies) {
        titleFont.setFamily(family);
        if (QFontInfo(titleFont).family() == family) {
            break; // 找到可用字体
        }
    }
    titleFont.setPointSize(32);
    titleFont.setBold(true);
    titleText->setFont(titleFont);
    
    titleText->setPos((sceneRect().width() - titleText->boundingRect().width()) / 2, 50);
    
    // 添加艺术效果：阴影
    QGraphicsDropShadowEffect *titleShadow = new QGraphicsDropShadowEffect();
    titleShadow->setBlurRadius(15);
    titleShadow->setColor(QColor(0, 0, 0, 100));
    titleShadow->setOffset(3, 3);
    titleText->setGraphicsEffect(titleShadow);
    
    addItem(titleText);
    
    // 创建5个地图选择按钮
    qreal buttonWidth = 120;
    qreal buttonHeight = 100;
    qreal spacing = 30;
    qreal totalWidth = 5 * buttonWidth + 4 * spacing;
    qreal startX = (sceneRect().width() - totalWidth) / 2;
    qreal buttonY = 380; // 调整到提示文字上方一点点 (原来是250，提示文字在500)
    
    for (int i = 0; i < 5; i++) {
        // 计算按钮位置
        qreal buttonX = startX + i * (buttonWidth + spacing);
        
        // 创建圆角矩形按钮
        QGraphicsRectItem *button = new QGraphicsRectItem(buttonX, buttonY, buttonWidth, buttonHeight);
        
        // 设置按钮样式
        updateButtonStyle(button, i);
        
        mapButtons.append(button);
        addItem(button);
        
        // 创建数字文本，使用艺术字体
        QGraphicsTextItem *numberText = new QGraphicsTextItem(QString::number(i + 1));
        numberText->setDefaultTextColor(QColor(255, 255, 255)); // 白色数字
        
        // 为数字也使用艺术字体
        QFont numberFont;
        QStringList numberFontFamilies = {
            "Impact",        // 粗体艺术字
            "Arial Black",   // 粗黑体
            "Courier New",   // 等宽字体
            "Arial"          // 备用
        };
        
        for (const QString &family : numberFontFamilies) {
            numberFont.setFamily(family);
            if (QFontInfo(numberFont).family() == family) {
                break;
            }
        }
        numberFont.setPointSize(36);
        numberFont.setBold(true);
        numberText->setFont(numberFont);
        
        // 居中数字文本
        qreal textX = buttonX + (buttonWidth - numberText->boundingRect().width()) / 2;
        qreal textY = buttonY + (buttonHeight - numberText->boundingRect().height()) / 2;
        numberText->setPos(textX, textY);
        
        // 为数字添加发光效果
        QGraphicsDropShadowEffect *numberGlow = new QGraphicsDropShadowEffect();
        numberGlow->setBlurRadius(8);
        numberGlow->setColor(QColor(255, 255, 255, 150));
        numberGlow->setOffset(0, 0);
        numberText->setGraphicsEffect(numberGlow);
        
        mapButtonTexts.append(numberText);
        addItem(numberText);
    }
    
    // 添加操作提示
    QGraphicsTextItem *hintText = new QGraphicsTextItem("使用方向键或鼠标选择地图 | Enter确认 | ESC退出");
    hintText->setDefaultTextColor(QColor(180, 150, 200, 180)); // 更淡的紫色，增加透明度
    hintText->setFont(QFont("Arial", 14));
    hintText->setPos((sceneRect().width() - hintText->boundingRect().width()) / 2, 500);
    addItem(hintText);
}

void ChooseBattlefieldScene::updateButtonStyle(QGraphicsRectItem *button, int index) {
    if (!button) return;
    
    if (index == selectedMapIndex) {
        // 选中状态：更淡的粉色渐变
        QLinearGradient selectedGradient(0, 0, 0, button->rect().height());
        selectedGradient.setColorAt(0, QColor(255, 182, 193, 200));   // 淡粉色，增加透明度
        selectedGradient.setColorAt(1, QColor(255, 105, 180, 200));   // 热粉色，增加透明度
        
        button->setBrush(QBrush(selectedGradient));
        button->setPen(QPen(QColor(255, 20, 147, 150), 3));  // 深粉色边框，更淡
    } else {
        // 普通状态：更淡的紫色渐变
        QLinearGradient normalGradient(0, 0, 0, button->rect().height());
        normalGradient.setColorAt(0, QColor(186, 85, 211, 160));   // 兰花紫，更淡
        normalGradient.setColorAt(1, QColor(147, 112, 219, 160));  // 中紫罗兰色，更淡
        
        button->setBrush(QBrush(normalGradient));
        button->setPen(QPen(QColor(138, 43, 226, 120), 2));  // 蓝紫色边框，更淡
    }
}

void ChooseBattlefieldScene::updateSelection(int newIndex) {
    if (newIndex < 0 || newIndex >= mapButtons.size()) return;
    
    int oldIndex = selectedMapIndex;
    selectedMapIndex = newIndex;
    
    // 更新按钮样式
    if (oldIndex >= 0 && oldIndex < mapButtons.size()) {
        updateButtonStyle(mapButtons[oldIndex], oldIndex);
    }
    updateButtonStyle(mapButtons[selectedMapIndex], selectedMapIndex);
}

void ChooseBattlefieldScene::processInput() {
    Scene::processInput();
    // 主要通过键盘和鼠标事件处理输入
}

void ChooseBattlefieldScene::update() {
    Scene::update();
    // 简化的更新函数，移除了复杂动画
}

void ChooseBattlefieldScene::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Escape:
            emit returnToModeSelection();
            break;
        case Qt::Key_Left:
        case Qt::Key_A:
            // 向左选择
            if (selectedMapIndex > 0) {
                updateSelection(selectedMapIndex - 1);
            }
            break;
        case Qt::Key_Right:
        case Qt::Key_D:
            // 向右选择
            if (selectedMapIndex < mapButtons.size() - 1) {
                updateSelection(selectedMapIndex + 1);
            }
            break;
        case Qt::Key_Return:
        case Qt::Key_Enter:
        case Qt::Key_Space:
            // 确认选择当前地图
            handleMapSelection(selectedMapIndex + 1);
            break;
        case Qt::Key_1:
        case Qt::Key_2:
        case Qt::Key_3:
        case Qt::Key_4:
        case Qt::Key_5:
            // 数字键直接选择
            {
                int mapIndex = event->key() - Qt::Key_1;
                if (mapIndex >= 0 && mapIndex < mapButtons.size()) {
                    updateSelection(mapIndex);
                    handleMapSelection(mapIndex + 1);
                }
            }
            break;
        default:
            Scene::keyPressEvent(event);
    }
}

void ChooseBattlefieldScene::keyReleaseEvent(QKeyEvent *event) {
    Scene::keyReleaseEvent(event);
}

void ChooseBattlefieldScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        QPointF clickPos = event->scenePos();
        
        // 检查点击了哪个按钮
        for (int i = 0; i < mapButtons.size(); i++) {
            QGraphicsRectItem *button = mapButtons[i];
            if (button && button->contains(button->mapFromScene(clickPos))) {
                updateSelection(i);
                // 所有按钮都可以选择
                handleMapSelection(i + 1);
                break;
            }
        }
    }
    
    Scene::mousePressEvent(event);
}

void ChooseBattlefieldScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QPointF mousePos = event->scenePos();
    
    // 检查鼠标悬停在哪个按钮上
    for (int i = 0; i < mapButtons.size(); i++) {
        QGraphicsRectItem *button = mapButtons[i];
        if (button && button->contains(button->mapFromScene(mousePos))) {
            // 鼠标悬停时更新选中状态
            if (i != selectedMapIndex) {
                updateSelection(i);
            }
            
            // 简单的悬停效果：稍微提高亮度
            QLinearGradient hoverGradient(0, 0, 0, button->rect().height());
            if (i == selectedMapIndex) {
                hoverGradient.setColorAt(0, QColor(255, 192, 203, 220));  // 更亮的粉色，但保持淡色
                hoverGradient.setColorAt(1, QColor(255, 130, 200, 220));
            } else {
                hoverGradient.setColorAt(0, QColor(206, 105, 231, 180)); // 更亮的紫色，但保持淡色
                hoverGradient.setColorAt(1, QColor(167, 132, 239, 180));
            }
            button->setBrush(QBrush(hoverGradient));
            break;
        }
    }
    
    Scene::mouseMoveEvent(event);
}

void ChooseBattlefieldScene::handleMapSelection(int mapId) {   
    // 发送信号
    emit mapSelected(mapId);
    
    // 开始战斗
    startBattle(mapId);
}

void ChooseBattlefieldScene::startBattle(int mapId) {   
    // 这里可以添加切换到战斗场景的逻辑
    // 比如创建BattleScene实例并传递mapId
}
