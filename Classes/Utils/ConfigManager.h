#ifndef __CONFIG_MANAGER_H__
#define __CONFIG_MANAGER_H__
#pragma execution_character_set("utf-8")
#include "cocos2d.h"
#include <map>

struct CardConfig {
    int id;
    std::string name;
    int cost;
    int attack;
    int health;
    std::string description;
    std::string imagePath;
    // ... 其他配置项
};

class ConfigManager {
public:
    static ConfigManager* getInstance();

    bool loadConfigs();
    const CardConfig* getCardConfig(int id) const;

private:
    ConfigManager();
    static ConfigManager* _instance;

    std::map<int, CardConfig> _cardConfigs;

    bool loadCardConfigs();
    bool parseCardJson(const std::string& jsonString);
};

#endif // __CONFIG_MANAGER_H__