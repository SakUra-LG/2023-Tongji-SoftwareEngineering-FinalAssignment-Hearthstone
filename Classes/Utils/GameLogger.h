#ifndef __GAME_LOGGER_H__
#define __GAME_LOGGER_H__

#include <string>
#include <fstream>
#include "cocos2d.h"
#pragma execution_character_set("utf-8")

// 日志级别枚举
enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERR
};

class GameLogger {
private:
    static GameLogger* _instance;
    GameLogger() {}  // 私有构造函数
    std::string getLogLevelString(LogLevel level);

public:
    static GameLogger* getInstance();
    void log(LogLevel level, const std::string& message);
};

#endif // __GAME_LOGGER_H__