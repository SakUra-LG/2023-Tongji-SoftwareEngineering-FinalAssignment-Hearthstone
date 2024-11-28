#include "ConfigManager.h"
#include "json/document.h"
#include "Utils/GameLogger.h"

ConfigManager* ConfigManager::_instance = nullptr;

ConfigManager* ConfigManager::getInstance() {
    if (!_instance) {
        _instance = new ConfigManager();
    }
    return _instance;
}

ConfigManager::ConfigManager() {
    // ��ʼ����Ա����
    _cardConfigs.clear();
}

bool ConfigManager::loadConfigs() {
    // ������������
    bool success = true;

    // ���ؿ�������
    if (!loadCardConfigs()) {
        GameLogger::getInstance()->log(LogLevel::ERR, "Failed to load card configs");
        success = false;
    }

    return success;
}

bool ConfigManager::loadCardConfigs() {
    // ��ȡ���������ļ�
    std::string filePath = "configs/cards.json";
    std::string jsonString = cocos2d::FileUtils::getInstance()->getStringFromFile(filePath);

    if (jsonString.empty()) {
        GameLogger::getInstance()->log(LogLevel::ERR, "Card config file not found: " + filePath);
        return false;
    }

    return parseCardJson(jsonString);
}

bool ConfigManager::parseCardJson(const std::string& jsonString) {
    rapidjson::Document doc;
    if (doc.Parse(jsonString.c_str()).HasParseError()) {
        GameLogger::getInstance()->log(LogLevel::ERR, "Failed to parse card config JSON");
        return false;
    }

    if (!doc.IsArray()) {
        GameLogger::getInstance()->log(LogLevel::ERR, "Card config JSON should be an array");
        return false;
    }

    // �����������
    _cardConfigs.clear();

    // ����ÿ����������
    for (const auto& cardJson : doc.GetArray()) {
        if (!cardJson.IsObject()) continue;

        CardConfig config;

        // ������Ҫ�ֶ�
        if (cardJson.HasMember("id") && cardJson["id"].IsInt()) {
            config.id = cardJson["id"].GetInt();
        }
        else continue;

        if (cardJson.HasMember("name") && cardJson["name"].IsString()) {
            config.name = cardJson["name"].GetString();
        }

        if (cardJson.HasMember("cost") && cardJson["cost"].IsInt()) {
            config.cost = cardJson["cost"].GetInt();
        }

        if (cardJson.HasMember("attack") && cardJson["attack"].IsInt()) {
            config.attack = cardJson["attack"].GetInt();
        }

        if (cardJson.HasMember("health") && cardJson["health"].IsInt()) {
            config.health = cardJson["health"].GetInt();
        }

        if (cardJson.HasMember("description") && cardJson["description"].IsString()) {
            config.description = cardJson["description"].GetString();
        }

        if (cardJson.HasMember("imagePath") && cardJson["imagePath"].IsString()) {
            config.imagePath = cardJson["imagePath"].GetString();
        }

        // �洢����
        _cardConfigs[config.id] = config;
    }

    GameLogger::getInstance()->log(LogLevel::INFO,
        "Successfully loaded " + std::to_string(_cardConfigs.size()) + " card configs");

    return true;
}

const CardConfig* ConfigManager::getCardConfig(int id) const {
    auto it = _cardConfigs.find(id);
    if (it != _cardConfigs.end()) {
        return &(it->second);
    }
    return nullptr;
}