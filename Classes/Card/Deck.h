#ifndef __DECK_H__
#define __DECK_H__

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
    std::vector<Card*> getCards() const;
    
    const std::string& getName() const { return _name; }
    int getTotalCards() const;
    
private:
    bool init(const std::string& name);
    std::string _name;
    std::map<int, int> _cardCounts;  // cardId -> count
};

#endif 