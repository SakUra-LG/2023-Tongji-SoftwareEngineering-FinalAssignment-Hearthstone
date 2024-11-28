#include "Utils/GameLogger.h"
#include <iostream>
#include <ctime>
#include <string.h>

GameLogger* GameLogger::_instance = nullptr;

GameLogger* GameLogger::getInstance() {
    if (!_instance) {
        _instance = new GameLogger();
    }
    return _instance;
}

void GameLogger::log(LogLevel level, const std::string& message) {
    time_t now = time(0);
    char* dt = ctime(&now);
    if (dt) {
        dt[strlen(dt) - 1] = '\0';  // 移除换行符
    }

    std::string levelStr = getLogLevelString(level);
    std::cout << "[" << (dt ? dt : "Unknown time") << "] [" << levelStr << "] " << message << std::endl;
}

std::string GameLogger::getLogLevelString(LogLevel level) {
    switch (level) {
    case LogLevel::DEBUG:   return "DEBUG";
    case LogLevel::INFO:    return "INFO";
    case LogLevel::WARNING: return "WARNING";
    case LogLevel::ERR:   return "ERROR";
    default:               return "UNKNOWN";
    }
}