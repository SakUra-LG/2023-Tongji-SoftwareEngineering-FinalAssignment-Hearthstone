#include "GameManager.h"
#include "Player/Player.h"
#include "Card/Card.h"
#include "Network/NetworkManager.h"
#include "Utils/Constants.h"
#include "Utils/GameLogger.h"
#include "Game/TurnSystem.h"
#include <vector>
#include <queue>
#include "cocos2d.h"
#include"Card/MinionCard.h"
GameManager* GameManager::_instance = nullptr;

GameManager* GameManager::getInstance() {
    if (!_instance) {
        _instance = new GameManager();
    }
    return _instance;
}

GameManager::GameManager()
    : _gameState(GameState::MENU)
    , _currentPlayer(nullptr)
    , _opponentPlayer(nullptr)
    , _selectedTarget(nullptr) {
}
GameManager::~GameManager() {
    delete _currentPlayer;
    delete _opponentPlayer;
}

void GameManager::startGame() {
    _gameState = GameState::PLAYING;

    // 初始化玩家
    _currentPlayer = new Player();
    _opponentPlayer = new Player();

    // 初始化游戏
    initGame();

    // 发初始手牌
    for (int i = 0; i < GameConstants::STARTING_HAND_SIZE; ++i) {
        drawCard(_currentPlayer);
        drawCard(_opponentPlayer);
    }

    // 通知网络管理器
    GameAction startAction;
    startAction.type = ActionType::GAME_START;
    startAction.sourceId = 0;
    startAction.targetId = 0;
    NetworkManager::getInstance()->sendGameAction(startAction);

    GameLogger::getInstance()->log(LogLevel::INFO, "Game started");
}
void GameManager::processGameAction(const GameAction& action) {
    _actionQueue.push(action);
    if (_actionQueue.size() == 1) {
        processActionQueue();
    }
}

void GameManager::processActionQueue() {
    while (!_actionQueue.empty()) {
        const GameAction& action = _actionQueue.front();

        switch (action.type) {
        case ActionType::PLAY_CARD:
            // 处理打出卡牌
            break;
        case ActionType::ATTACK:
            // 处理攻击
            break;
        case ActionType::END_TURN:
            // 处理回合结束
            break;
        default:
            GameLogger::getInstance()->log(LogLevel::WARNING,
                "Unknown action type: " + std::to_string(static_cast<int>(action.type)));
            break;
        }

        _actionQueue.pop();
    }
}

void GameManager::checkGameState() {
    // 检查游戏是否结束
    if (_currentPlayer->getHealth() <= 0 || _opponentPlayer->getHealth() <= 0) {
        _gameState = GameState::GAME_OVER;
        endGame();
    }
}
void GameManager::initGame() {
    // 初始化游戏状态
    _gameState = GameState::PLAYING;
    _selectedTarget = nullptr;

    // 初始化玩家
    if (_currentPlayer) {
        _currentPlayer->setHealth(30);
        _currentPlayer->setMana(0);
        _currentPlayer->setMaxMana(0);
    }

    if (_opponentPlayer) {
        _opponentPlayer->setHealth(30);
        _opponentPlayer->setMana(0);
        _opponentPlayer->setMaxMana(0);
    }

    // 记录游戏开始
    GameLogger::getInstance()->log(LogLevel::INFO, "Game initialized");
}

void GameManager::drawCard(Player* player) {
    if (!player) return;

    // 检查是否还有牌可抽
    if (!player->canDrawCard()) {
        // 疲劳伤害
        player->takeDamage(player->getFatigueDamage());
        player->increaseFatigueDamage();
        GameLogger::getInstance()->log(LogLevel::INFO,
            "Player took fatigue damage: " + std::to_string(player->getFatigueDamage()));
        return;
    }

    // 从牌库抽一张牌
    auto& deck = player->getDeck();
    if (!deck.empty()) {
        Card* card = deck.back();
        deck.pop_back();

        // 添加到手牌
        player->getHand().push_back(card);

        // 触发抽牌效果
        card->setOwner(player);
        card->onDraw();

        GameLogger::getInstance()->log(LogLevel::INFO,
            "Player drew a card: " + card->getCardName());
    }
}
bool GameManager::useHeroPower(Player* player) {
    if (!player || !canUseHeroPower(player)) {
        return false;
    }

    // 扣除法力值
    player->setMana(player->getMana() - HERO_POWER_COST);

    // 对目标造成伤害
    if (_selectedTarget) {
        // 如果是随从卡牌，确保它有生命值属性
        if (auto minionCard = dynamic_cast<MinionCard*>(_selectedTarget)) {
            minionCard->takeDamage(2);
            GameLogger::getInstance()->log(LogLevel::INFO,
                "Hero power used on minion: " + _selectedTarget->getCardName());
        }
    }
    else {
        // 直接对敌方英雄造成伤害
        _opponentPlayer->takeDamage(2);
        GameLogger::getInstance()->log(LogLevel::INFO,
            "Hero power used on opponent hero");
    }

    return true;
}

bool GameManager::canUseHeroPower(Player* player) const {
    if (!player) return false;

    // 检查是否是玩家的回合
    if (player != _currentPlayer) return false;

    // 检查法力值是否足够
    if (player->getMana() < HERO_POWER_COST) return false;

    // 检查本回合是否已经使用过英雄技能
    // TODO: 添加使用次数检查

    return true;
}
void GameManager::endGame() {
    _gameState = GameState::GAME_OVER;
    // 确定获胜者
    std::string winner;
    if (_currentPlayer->isDead()) {
        winner = "Opponent";
    }
    else if (_opponentPlayer->isDead()) {
        winner = "Current Player";
    }
    else {
        winner = "Draw";
    }

    // 记录游戏结束
    GameLogger::getInstance()->log(LogLevel::INFO,
        "Game ended. Winner: " + winner);

    // 可以在这里添加游戏结束的UI显示
    // TODO: 显示游戏结束界面
}
Card* GameManager::getSelectedTarget() const {
    return _selectedTarget;
}
void GameManager::selectTarget(Card* target) {
    _selectedTarget = target;
    
    // 可以添加日志
    if (target) {
        GameLogger::getInstance()->log(LogLevel::INFO,
            "Selected target: " + target->getCardName());
    } else {
        GameLogger::getInstance()->log(LogLevel::INFO,
            "Cleared target selection");
    }
}
void GameManager::discardCard(Card* card) {
    if (!card) return;

    Player* owner = card->getOwner();
    if (!owner) return;

    auto& hand = owner->getHand();
    auto it = std::find(hand.begin(), hand.end(), card);
    if (it != hand.end()) {
        hand.erase(it);
    }

    auto& field = owner->getField();
    it = std::find(field.begin(), field.end(), card);
    if (it != field.end()) {
        field.erase(it);
    }

    owner->getDiscardPile().push_back(card);

    // 使用 GameLogger
    GameLogger::getInstance()->log(LogLevel::INFO,
        "Card discarded: " + card->getCardName());
}
