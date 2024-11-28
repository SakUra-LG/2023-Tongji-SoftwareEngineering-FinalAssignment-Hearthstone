#ifndef __CONFIG_MANAGER_H__
#define __CONFIG_MANAGER_H__

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
    // ... ∆‰À˚≈‰÷√œÓ
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