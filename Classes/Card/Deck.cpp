#include "Deck.h"
#include "CardFactory.h"

Deck* Deck::create(const std::string& name) {
    auto deck = new Deck();
    if (deck && deck->init(name)) {
        deck->autorelease();
        return deck;
    }
    CC_SAFE_DELETE(deck);
    return nullptr;
}

bool Deck::init(const std::string& name) {
    _name = name;
    return true;
}

void Deck::addCard(int cardId, int count) {
    _cardCounts[cardId] += count;
}

void Deck::removeCard(int cardId, int count) {
    if (_cardCounts.find(cardId) != _cardCounts.end()) {
        _cardCounts[cardId] = std::max(0, _cardCounts[cardId] - count);
        if (_cardCounts[cardId] == 0) {
            _cardCounts.erase(cardId);
        }
    }
}

int Deck::getCardCount(int cardId) const {
    auto it = _cardCounts.find(cardId);
    return it != _cardCounts.end() ? it->second : 0;
}

std::vector<Card*> Deck::getCards() const {
    std::vector<Card*> cards;
    auto factory = CardFactory::getInstance();
    
    for (const auto& pair : _cardCounts) {
        for (int i = 0; i < pair.second; ++i) {
            auto card = factory->createCardById(pair.first);
            if (card) {
                cards.push_back(card);
            }
        }
    }
    return cards;
}

int Deck::getTotalCards() const {
    int total = 0;
    for (const auto& pair : _cardCounts) {
        total += pair.second;
    }
    return total;
} 