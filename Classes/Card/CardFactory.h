#ifndef __CARD_FACTORY_H__
#define __CARD_FACTORY_H__
#pragma execution_character_set("utf-8")
#include "Card/Card.h"
#include "Card/MinionCard.h"
#include "Card/SpellCard.h"
#include "Utils/GameLogger.h"
#include <map>
#include <vector>
#include <string>

/**
 * @struct CardData
 * @brief 卡牌数据结构
 * 
 * 存储卡牌的基本属性数据，用于卡牌的创建和初始化。
 */
struct CardData {
    int id;                     // 卡牌ID
    std::string name;           // 卡牌名称
    CardType type;              // 卡牌类型（随从/法术）
    CardRarity rarity;          // 稀有度
    int cost;                   // 法力消耗
    int attack;                 // 攻击力（仅随从卡）
    int health;                 // 生命值（仅随从卡）
    std::string description;    // 卡牌描述
    std::string framePath;      // 卡牌框架图片路径
    std::string portraitPath;   // 卡牌立绘图片路径

    // 添加效果标记
    bool hasBattlecry = false;      // 是否有战吼效果
    bool hasDeathrattle = false;    // 是否有亡语效果
    bool hasDivineShield = false;   // 是否有圣盾
    bool hasTaunt = false;          // 是否有嘲讽
    bool hasPoison = false;         // 是否有剧毒
    bool hasOutcast = false;        // 是否有流放效果
    bool hasRush = false;           // 是否有突袭
    bool hasCharge = false;         // 是否有冲锋
};

/**
 * @struct CardTemplate
 * @brief 卡牌模板结构
 * 
 * 存储卡牌模板的基本属性数据，用于卡牌的创建和初始化。
 */
struct CardTemplate {
    int id;
    std::string name;
    int cost;
    std::string description;
    CardType type;
    CardRarity rarity;
    // ... 其他需要的属性
};

/**
 * @class CardFactory
 * @brief 卡牌工厂类
 * 
 * 负责创建和管理所有卡牌实例。
 * 使用单例模式确保全局只有一个卡牌工厂实例。
 */
class CardFactory {
private:
    CardFactory();
    static CardFactory* _instance;
    
    std::map<int, CardData> _cardTemplates;  // 只保留一个模板存储
    std::vector<Card*> allCards;
    std::vector<Card*> deck1;
    std::vector<Card*> deck2;
    
public:
    /**
     * @brief 获取卡牌工厂的单例实例
     * @return 返回卡牌工厂的单例指针
     */
    static CardFactory* getInstance();
    
    /**
     * @brief 通过ID创建卡牌
     * @param cardId 要创建的卡牌ID
     * @return 返回创建的卡牌指针，如果ID不存在则返回nullptr
     */
    Card* createCardById(int cardId);
    
    // 新增方法
    void initializeDecks();                    // 初始化卡组
    const std::vector<Card*>& getDeck1() const;  // 获取卡组1
    const std::vector<Card*>& getDeck2() const;  // 获取卡组2
    void setCardCount(int cardId, int count);    // 设置卡牌数量
    
    /**
     * @brief 创建卡牌
     * @param id 卡牌ID
     * @param name 卡牌名称
     * @return 返回创建的卡牌指针
     */
    Card* createCard(int id, const std::string& name);
    
    /**
     * @brief 初始化卡牌模板
     */
    void initCardTemplates();
    
    /**
     * @brief 创建随从卡牌
     * @param data 随从卡牌的数据
     * @return 返回创建的随从卡牌指针
     */
    MinionCard* createMinionCard(const CardData& data);
    
    /**
     * @brief 创建法术卡牌
     * @param data 法术卡牌的数据
     * @return 返回创建的法术卡牌指针
     */
    SpellCard* createSpellCard(const CardData& data);
};

#endif // __CARD_FACTORY_H__ 