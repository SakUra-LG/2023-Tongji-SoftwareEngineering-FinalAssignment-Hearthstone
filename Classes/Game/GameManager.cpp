 
#include "GameManager.h"
#include "Player/Player.h"
#include "Card/Card.h"
#include "TurnSystem.h"
#include"Utils/GameLogger.h"
GameManager* GameManager::_instance = nullptr;

GameManager* GameManager::getInstance() {
    if (!_instance) {
        _instance = new GameManager();
    }
    return _instance;
}

GameManager::GameManager()
    : _currentPlayer(nullptr)
    , _opponentPlayer(nullptr)
    , _isGameOver(false)
, _selectedTarget(nullptr){
}

void GameManager::initGame() {
    // 初始化玩家
    _currentPlayer = new Player();
    _opponentPlayer = new Player();

    // 初始化卡组
    // TODO: 从配置文件加载卡组

    // 发初始手牌
    for (int i = 0; i < 3; ++i) {
        drawCard(_currentPlayer);                    // 当前玩家抽牌
        drawCard(_opponentPlayer);                   // 对手玩家抽牌
    }
}

void GameManager::drawCard(Player* player) {         // 玩家抽牌的方法
    if (player->getDeck().empty()) {                // 如果牌库为空
        player->takeDamage(player->getFatigueDamage()); // 造成疲劳伤害
        player->increaseFatigueDamage();            // 增加下次疲劳伤害值
        return;                                     // 结束方法
    }
    Card* card = player->getDeck().back();          // 获取牌库顶部的卡牌
    player->getDeck().pop_back();                   // 从牌库移除该卡牌
    player->getHand().push_back(card);              // 将卡牌添加到手牌
}
void GameManager::discardCard(Card* card) {
    if (!card) return;

    // 获取卡牌所属的玩家
    Player* owner = card->getOwner();
    if (!owner) return;

    // 从手牌中移除
    auto& hand = owner->getHand();
    auto it = std::find(hand.begin(), hand.end(), card);
    if (it != hand.end()) {
        hand.erase(it);
    }

    // 从场上移除
    auto& field = owner->getField();
    it = std::find(field.begin(), field.end(), card);
    if (it != field.end()) {
        field.erase(it);
    }

    // 添加到弃牌堆
    owner->getDiscardPile().push_back(card);

    // 记录日志
    GameLogger::getInstance()->log(LogLevel::INFO,
        "Card discarded: " + card->getCardName());
}
Card* GameManager::getSelectedTarget() const {
    return _selectedTarget;
}
