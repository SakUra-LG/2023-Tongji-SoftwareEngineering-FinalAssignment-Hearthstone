#ifndef __GAME_LOGGER_H__
#define __GAME_LOGGER_H__

#include <string>

// ��־����ö��
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
    GameLogger() {}  // ˽�й��캯��
    static GameLogger* _instance;
    std::string getLogLevelString(LogLevel level);
};

#endif // __GAME_LOGGER_H__