 
#ifndef __UTILS_H__
#define __UTILS_H__

#include "cocos2d.h"
#include <string>
#include <vector>
#include <random>

class Utils {
public:
    // ���������
    static int random(int min, int max);
    static float randomFloat(float min, float max);
    static bool randomBool(float probability = 0.5f);

    // �ַ�������
    static std::vector<std::string> split(const std::string& str, char delimiter);
    static std::string trim(const std::string& str);
    static std::string toLowerCase(const std::string& str);

    // �ļ�����
    static std::string readFile(const std::string& filename);
    static bool writeFile(const std::string& filename, const std::string& content);
    static bool fileExists(const std::string& filename);

    // UI����
    static cocos2d::Vec2 centerOfNode(cocos2d::Node* node);
    static bool isPointInNode(const cocos2d::Vec2& point, cocos2d::Node* node);
    static float distanceBetweenNodes(cocos2d::Node* node1, cocos2d::Node* node2);

    // ��������
    static cocos2d::Action* createCardFlipAnimation();
    static cocos2d::Action* createFloatingAnimation();
    static cocos2d::Action* createShakeAnimation();

    // ����ת��
    static std::string intToString(int value, int width = 0);
    static cocos2d::Color3B hexToColor(const std::string& hexColor);

    // ��Ϸ����
    static bool isValidCardTarget(Card* source, Card* target);
    static std::vector<Card*> getValidTargets(Card* card);
    static void shuffleDeck(std::vector<Card*>& deck);

private:
    static std::random_device _rd;
    static std::mt19937 _gen;
};

#endif // __UTILS_H__