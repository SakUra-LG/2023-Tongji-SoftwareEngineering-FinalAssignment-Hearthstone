#include "GameLogger.h"
#include <fstream>
#include <ctime>
#include "cocos2d.h"

GameLogger* GameLogger::_instance = nullptr;

GameLogger* GameLogger::getInstance() {
    if (!_instance) {
        _instance = new GameLogger();
    }
    return _instance;
}

void GameLogger::log(LogLevel level, const std::string& message) {
    // 使用项目目录作为日志文件位置
    std::string logPath = "H:/FinalProjectForHearthStone/Projects/Projects/HearthStone/game_log.txt";
    std::ofstream logFile(logPath, std::ios::app);
    
    if (logFile.is_open()) {
        // 获取当前时间
        time_t now = time(0);
        char* dt = ctime(&now);
        dt[strlen(dt) - 1] = '\0';

        // 写入日志
        logFile << "[" << dt << "] [" << getLogLevelString(level) << "] " 
               << message << std::endl;
        logFile.close();
        
        // 同时输出到控制台，方便调试
        cocos2d::log("[%s] [%s] %s", 
            dt, 
            getLogLevelString(level).c_str(), 
            message.c_str());
    }
    else {
        // 如果还是打不开，尝试在当前目录创建
        logFile.open("./game_log.txt", std::ios::app);
        if (!logFile.is_open()) {
            cocos2d::log("Failed to open log file at both paths");
            cocos2d::log("[%s] %s", getLogLevelString(level).c_str(), message.c_str());
            return;
        }
    }
}

std::string GameLogger::getLogLevelString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG:   return "DEBUG";
        case LogLevel::INFO:    return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERR:     return "ERROR";
        default:               return "UNKNOWN";
    }
}