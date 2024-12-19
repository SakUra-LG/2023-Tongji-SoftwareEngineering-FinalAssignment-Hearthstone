#include "CardFactory.h"
#include "DeckManager.h"
#pragma execution_character_set("utf-8")
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
        // 从 CardFactory 获取卡牌信息
        auto factory = CardFactory::getInstance();
        const auto& deck1 = factory->getDeck1();
        
        // 根据 CardFactory 中设置的数量添加卡牌
        for (const auto& card : deck1) {
            if (card->getCount() > 0) {
                deck->addCard(card->getId(), card->getCount());
            }
        }
    }
    return deck;
}

Deck* DeckManager::createRainbowDKDeck() {
    auto deck = Deck::create("Rainbow DK");
    if (deck) {
        // 从 CardFactory 获取卡牌信息
        auto factory = CardFactory::getInstance();
        const auto& deck2 = factory->getDeck2();
        
        // 根据 CardFactory 中设置的数量添加卡牌
        for (const auto& card : deck2) {
            if (card->getCount() > 0) {
                deck->addCard(card->getId(), card->getCount());
            }
        }
    }
    return deck;
}

Deck* DeckManager::createDeckFromTemplate(const std::vector<Card*>& templateDeck) {
    auto deck = Deck::create(templateDeck[0]->getId() / 1000 == 1 ? 
        "Quest Demon Hunter" : "Rainbow DK");
    
    if (deck) {
        // 直接根据模板卡组添加卡牌
        for (const auto& card : templateDeck) {
            if (card && card->getCount() > 0) {
                deck->addCard(card->getId(), card->getCount());
            }
        }
    }
    return deck;
} 