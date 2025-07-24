//
// Created by AI Assistant on 7/24/25
//

#ifndef QT_PROGRAMMING_2024_GAMEMODE_H
#define QT_PROGRAMMING_2024_GAMEMODE_H

#include <QString>
/**
 * @brief 游戏模式枚举
 * 
 * 定义不同的游戏模式类型
 */
enum class GameMode {
    PvP,    // 玩家对玩家模式 (Player vs Player)
    PvE,    // 玩家对环境/AI模式 (Player vs Environment/AI)
    Single  // 单人练习模式
};

/**
 * @brief 游戏模式工具类
 * 
 * 提供游戏模式相关的工具方法
 */
class GameModeUtils {
public:
    // 将游戏模式转换为字符串
    static QString gameModeToString(GameMode mode);
    
    // 将字符串转换为游戏模式
    static GameMode stringToGameMode(const QString& str);
    
    // 获取游戏模式描述
    static QString getGameModeDescription(GameMode mode);
    
    // 检查游戏模式是否需要AI
    static bool needsAI(GameMode mode);
    
    // 检查游戏模式是否支持双人控制
    static bool supportsDualControl(GameMode mode);
};

#endif //QT_PROGRAMMING_2024_GAMEMODE_H
