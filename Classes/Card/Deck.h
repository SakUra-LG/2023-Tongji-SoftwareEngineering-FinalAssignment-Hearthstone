#ifndef __DECK_H__
#define __DECK_H__
#pragma execution_character_set("utf-8")
#include "cocos2d.h"
#include "Card.h"
#include <vector>
#include <map>

class Deck : public cocos2d::Ref {
public:
    static Deck* create(const std::string& name);
    
    void addCard(int cardId, int count = 1);
    void removeCard(int cardId, int count = 1);
    int getCardCount(int cardId) const;
    const std::string& getName() const { return _name; }
    
    // 添加卡牌实例到卡组
    void addCardInstance(Card* card);
    
    // 获取卡组中的卡牌总数
    int getTotalCards() const;
    
    // 获取卡组中所有卡牌
    const std::vector<Card*>& getCards() const;
    
    // 清空卡组
    void clear();
    
private:
    bool init(const std::string& name);
    std::string _name;
    std::map<int, int> _cardCounts;  // cardId -> count
    std::vector<Card*> _cards;
};

#endif 