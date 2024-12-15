#include "DeckManager.h"

DeckManager* DeckManager::_instance = nullptr;

DeckManager* DeckManager::getInstance() {
    if (!_instance) {
        _instance = new DeckManager();
    }
    return _instance;
}

Deck* DeckManager::createQuestDemonHunterDeck() {
    auto deck = Deck::create("Quest Demon Hunter");
    if (deck) {
        // 添加任务瞎卡组的卡牌
        deck->addCard(1001, 2);  // 二段跳 x2
        deck->addCard(1002, 2);  // 伊利达雷研习 x2
        deck->addCard(1003, 2);  // 凶猛的外来者 x2
        // ... 继续添加其他卡牌
    }
    return deck;
}

Deck* DeckManager::createRainbowDKDeck() {
    auto deck = Deck::create("Rainbow DK");
    if (deck) {
        // 添加彩虹DK卡组的卡牌
        deck->addCard(2001, 2);  // 奇迹推销员 x2
        deck->addCard(2002, 2);  // 黑暗符文 x2
        deck->addCard(2003, 2);  // 冰霜打击 x2
        // ... 继续添加其他卡牌
    }
    return deck;
} 