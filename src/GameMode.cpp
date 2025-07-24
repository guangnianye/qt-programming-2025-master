//
// Created by AI Assistant on 7/24/25
//

#include "GameMode.h"

QString GameModeUtils::gameModeToString(GameMode mode) {
    switch (mode) {
        case GameMode::PvP:
            return "PvP";
        case GameMode::PvE:
            return "PvE";
        case GameMode::Single:
            return "Single";
        default:
            return "Unknown";
    }
}

GameMode GameModeUtils::stringToGameMode(const QString& str) {
    if (str == "PvP") {
        return GameMode::PvP;
    } else if (str == "PvE") {
        return GameMode::PvE;
    } else if (str == "Single") {
        return GameMode::Single;
    } else {
        return GameMode::PvP; // 默认模式
    }
}

QString GameModeUtils::getGameModeDescription(GameMode mode) {
    switch (mode) {
        case GameMode::PvP:
            return "Player vs Player - Two players control characters with keyboard";
        case GameMode::PvE:
            return "Player vs AI - Player battles against intelligent AI character";
        case GameMode::Single:
            return "Practice Mode - Single player practice movement and weapons";
        default:
            return "Unknown Game Mode";
    }
}

bool GameModeUtils::needsAI(GameMode mode) {
    return mode == GameMode::PvE;
}

bool GameModeUtils::supportsDualControl(GameMode mode) {
    return mode == GameMode::PvP;
}
