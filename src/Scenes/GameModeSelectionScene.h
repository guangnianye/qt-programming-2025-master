//
// Created by AI Assistant on 7/24/25
//

#ifndef QT_PROGRAMMING_2024_GAMEMODESELECTIONSCENE_H
#define QT_PROGRAMMING_2024_GAMEMODESELECTIONSCENE_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QKeyEvent>
#include <QTimer>
#include "Scene.h"
#include "../GameMode.h"

/**
 * @brief 游戏模式选择场景
 * 
 * 允许玩家选择不同的游戏模式：PvP、PvE或单人练习
 */
class GameModeSelectionScene : public Scene {
    Q_OBJECT

public:
    explicit GameModeSelectionScene(QObject *parent = nullptr);
    ~GameModeSelectionScene();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void update() override;
    void updateSelection();

private:
    // UI元素
    QGraphicsTextItem *titleText;           // 标题文本
    QGraphicsTextItem *instructionText;     // 说明文本
    QGraphicsRectItem *backgroundRect;      // 背景矩形
    
    // 模式选项
    struct ModeOption {
        GameMode mode;                      // 游戏模式
        QGraphicsTextItem *titleItem;      // 模式标题
        QGraphicsTextItem *descItem;       // 模式描述
        QGraphicsRectItem *backgroundItem; // 选项背景
        QRectF bounds;                      // 选项边界
        
        ModeOption(GameMode m) : mode(m), titleItem(nullptr), descItem(nullptr), backgroundItem(nullptr) {}
    };
    
    QVector<ModeOption> modeOptions;        // 所有模式选项
    int currentSelection;                   // 当前选择的索引
    
    // 动画效果
    QTimer *blinkTimer;                     // 闪烁定时器
    bool blinkState;                        // 闪烁状态
    int blinkCounter;                       // 闪烁计数器
    
    // 初始化方法
    void initializeUI();
    void setupModeOptions();
    void setupBackground();
    void setupInstructions();
    
    // 更新方法
    void updateSelectionHighlight();
    void updateModeDescription();
    
    // 输入处理
    void handleSelectionChange(int direction);
    void handleModeSelection();
    
    // 样式设置
    void setOptionHighlighted(int index, bool highlighted);
    QColor getSelectionColor() const;
    QFont getTitleFont() const;
    QFont getDescriptionFont() const;
    QFont getInstructionFont() const;

signals:
    void gameModeSelected(GameMode mode);   // 游戏模式选择信号
    void returnToMainMenu();                // 返回主菜单信号

};

#endif //QT_PROGRAMMING_2024_GAMEMODESELECTIONSCENE_H
