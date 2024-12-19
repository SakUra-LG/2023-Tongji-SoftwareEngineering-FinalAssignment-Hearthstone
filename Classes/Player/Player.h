#ifndef __PLAYER_H__
#define __PLAYER_H__
#pragma execution_character_set("utf-8")
#include "cocos2d.h"
#include <vector>

class Card;

class Player {
public:
    Player();
    ~Player();
    // 基础属性
    CC_SYNTHESIZE(int, _health, Health);
    CC_SYNTHESIZE(int, _maxHealth, MaxHealth);
    CC_SYNTHESIZE(int, _armor, Armor);
    CC_SYNTHESIZE(int, _fatigueDamage, FatigueDamage);

    // 法力值相关
    CC_SYNTHESIZE(int, _mana, Mana);              // 当前法力值
    CC_SYNTHESIZE(int, _maxMana, MaxMana);        // 最大法力值
    CC_SYNTHESIZE(int, _tempMana, TempMana);      // 临时法力值（本回合可用）

    // 法力值操作
    void spendMana(int amount);                   // 消耗法力值
    void restoreMana();                           // 恢复法力值
    void gainMana(int amount);                    // 获得临时法力值
    void incrementMaxMana();                      // 增加最大法力值
    bool hasEnoughMana(int cost) const;           // 检查法力值是否足够

    // 卡牌管理
    std::vector<Card*>& getDeck() { return _deck; }
    std::vector<Card*>& getHand() { return _hand; }
    std::vector<Card*>& getField() { return _field; }
    std::vector<Card*>& getDiscardPile() { return _discardPile; }
    // 游戏行为
    void takeDamage(int amount);
    void heal(int amount);
    void addArmor(int amount);
    void increaseFatigueDamage();
    void removeCardFromHand(Card* card);
    bool hasValidTaunt() const;
    
    // 场上随从管理
    void addToField(Card* card);
    void removeFromField(Card* card);
    const std::vector<Card*>& getField() const { return _field; }

    // 状态检查
    bool isDead() const { return _health <= 0; }
    bool canDrawCard() const { return !_deck.empty(); }

    void setDeck(const std::vector<Card*>& deck) { _deck = deck; }

private:
    std::vector<Card*> _deck;   // 牌库
    std::vector<Card*> _hand;   // 手牌
    std::vector<Card*> _field;  // 场上随从
    std::vector<Card*> _discardPile;  // 弃牌堆
};

#endif // __PLAYER_H__