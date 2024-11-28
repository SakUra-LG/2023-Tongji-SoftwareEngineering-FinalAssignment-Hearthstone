#include "Utils/Constants.h"  // �������
#include "GameManager.h"
#include "Player/Player.h"
#include "Card/Card.h"
#include "Network/NetworkManager.h"
#include <vector>
#include <queue>
#include "Utils/GameLogger.h"
#include "cocos2d.h"
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

    // ��ʼ�����
    _currentPlayer = new Player();
    _opponentPlayer = new Player();

    // ��ʼ����Ϸ
    initGame();

    // ����ʼ����
    for (int i = 0; i < GameConstants::STARTING_HAND_SIZE; ++i) {
        drawCard(_currentPlayer);
        drawCard(_opponentPlayer);
    }

    // ֪ͨ���������
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
            // ����������
            break;
        case ActionType::ATTACK:
            // ������
            break;
        case ActionType::END_TURN:
            // ����غϽ���
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
    // �����Ϸ�Ƿ����
    if (_currentPlayer->getHealth() <= 0 || _opponentPlayer->getHealth() <= 0) {
        _gameState = GameState::GAME_OVER;
        endGame();
    }
}
void GameManager::initGame() {
    // ��ʼ����Ϸ״̬
    _gameState = GameState::PLAYING;
    _selectedTarget = nullptr;

    // ��ʼ�����
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

    // ��¼��Ϸ��ʼ
    GameLogger::getInstance()->log(LogLevel::INFO, "Game initialized");
}

void GameManager::drawCard(Player* player) {
    if (!player) return;

    // ����Ƿ����ƿɳ�
    if (!player->canDrawCard()) {
        // ƣ���˺�
        player->takeDamage(player->getFatigueDamage());
        player->increaseFatigueDamage();
        GameLogger::getInstance()->log(LogLevel::INFO,
            "Player took fatigue damage: " + std::to_string(player->getFatigueDamage()));
        return;
    }

    // ���ƿ��һ����
    auto& deck = player->getDeck();
    if (!deck.empty()) {
        Card* card = deck.back();
        deck.pop_back();

        // ��ӵ�����
        player->getHand().push_back(card);

        // ��������Ч��
        card->setOwner(player);
        card->onDraw();

        GameLogger::getInstance()->log(LogLevel::INFO,
            "Player drew a card: " + card->getCardName());
    }
}

void GameManager::endGame() {
    _gameState = GameState::GAME_OVER;
    // ȷ����ʤ��
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

    // ��¼��Ϸ����
    GameLogger::getInstance()->log(LogLevel::INFO,
        "Game ended. Winner: " + winner);

    // ���������������Ϸ������UI��ʾ
    // TODO: ��ʾ��Ϸ��������
}
Card* GameManager::getSelectedTarget() const {
    // ʵ�ֻ�ȡ��ǰѡ��Ŀ����߼�
    // ������漰�������Ϸ״̬����
    // ���磺
    return _selectedTarget; // ��������һ�� _selectedTarget ��Ա����
}
void GameManager::selectTarget(Card* target) {
    _selectedTarget = target;
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

    // ʹ�� GameLogger
    GameLogger::getInstance()->log(LogLevel::INFO,
        "Card discarded: " + card->getCardName());
}