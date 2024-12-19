 
#ifndef __UTILS_H__
#define __UTILS_H__
#pragma execution_character_set("utf-8")
#include "cocos2d.h"
#include <string>
#include <vector>
#include <random>

class Utils {
public:
    // 随机数生成
    static int random(int min, int max);
    static float randomFloat(float min, float max);
    static bool randomBool(float probability = 0.5f);

    // 字符串处理
    static std::vector<std::string> split(const std::string& str, char delimiter);
    static std::string trim(const std::string& str);
    static std::string toLowerCase(const std::string& str);

    // 文件操作
    static std::string readFile(const std::string& filename);
    static bool writeFile(const std::string& filename, const std::string& content);
    static bool fileExists(const std::string& filename);

    // UI辅助
    static cocos2d::Vec2 centerOfNode(cocos2d::Node* node);
    static bool isPointInNode(const cocos2d::Vec2& point, cocos2d::Node* node);
    static float distanceBetweenNodes(cocos2d::Node* node1, cocos2d::Node* node2);

    // 动画辅助
    static cocos2d::Action* createCardFlipAnimation();
    static cocos2d::Action* createFloatingAnimation();
    static cocos2d::Action* createShakeAnimation();

    // 数据转换
    static std::string intToString(int value, int width = 0);
    static cocos2d::Color3B hexToColor(const std::string& hexColor);

    // 游戏辅助
    static bool isValidCardTarget(Card* source, Card* target);
    static std::vector<Card*> getValidTargets(Card* card);
    static void shuffleDeck(std::vector<Card*>& deck);

private:
    static std::random_device _rd;
    static std::mt19937 _gen;
};

#endif // __UTILS_H__