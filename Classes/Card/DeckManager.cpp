#include "CardFactory.h"
#include "DeckManager.h"
#include "Utils/GameLogger.h"
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
    // 检查模板是否为空
    if (templateDeck.empty()) {
        return nullptr;
    }

    // 判断是哪种卡组（1开头是任务瞎，2开头是彩虹DK）
    bool isQuestDH = (templateDeck[0]->getId() / 1000 == 1);
    auto deck = Deck::create(isQuestDH ? "Quest Demon Hunter" : "Rainbow DK");
    
    if (deck) {
        auto factory = CardFactory::getInstance();
        auto logger = GameLogger::getInstance();
        
        // 遍历所有卡牌模板
        for (const auto& templateCard : templateDeck) {
            if (!templateCard) continue;
            
            int cardId = templateCard->getId();
            // 确保卡牌ID与卡组类型匹配
            if ((isQuestDH && cardId/1000 == 1) || (!isQuestDH && cardId/1000 == 2)) {
                // 使用CardFactory创建新的卡牌实例
                Card* newCard = factory->createCardById(cardId);
                if (newCard) {
                    // 设置卡牌数量（从模板中获取）
                    newCard->setCount(templateCard->getCount());
                    // 添加到卡组
                    deck->addCardInstance(newCard);
                    
                    logger->log(LogLevel::DEBUG, 
                        "Added card to deck: " + newCard->getName() + 
                        " (ID: " + std::to_string(cardId) + 
                        ", Count: " + std::to_string(templateCard->getCount()) + ")");
                }
            }
        }
        
        logger->log(LogLevel::INFO, 
            "Created deck: " + deck->getName() + 
            " with " + std::to_string(deck->getTotalCards()) + " cards");
    }
    
    return deck;
} 