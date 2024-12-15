#ifndef __DECK_MANAGER_H__
#define __DECK_MANAGER_H__

#include "Deck.h"

class DeckManager {
public:
    static DeckManager* getInstance();
    
    Deck* createQuestDemonHunterDeck();  // 任务瞎卡组
    Deck* createRainbowDKDeck();         // 彩虹DK卡组
    
private:
    static DeckManager* _instance;
};

#endif 