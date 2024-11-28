#ifndef __GAME_LOGGER_H__
#define __GAME_LOGGER_H__

#include <string>

// 日志级别枚举
enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERR
};

class GameLogger {
public:
    static GameLogger* getInstance();
    void log(LogLevel level, const std::string& message);

private:
    GameLogger() {}  // 私有构造函数
    static GameLogger* _instance;
    std::string getLogLevelString(LogLevel level);
};

#endif // __GAME_LOGGER_H__