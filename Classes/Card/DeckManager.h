#ifndef __DECK_MANAGER_H__
#define __DECK_MANAGER_H__
#pragma execution_character_set("utf-8")
#include "Deck.h"

class DeckManager {
public:
    static DeckManager* getInstance();
    
    Deck* createQuestDemonHunterDeck();  // 任务瞎卡组
    Deck* createRainbowDKDeck();         // 彩虹DK卡组
    Deck* createDeckFromTemplate(const std::vector<Card*>& templateDeck);
    
private:
    static DeckManager* _instance;
};

#endif 