 
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
    // ��ʼ�����
    _currentPlayer = new Player();
    _opponentPlayer = new Player();

    // ��ʼ������
    // TODO: �������ļ����ؿ���

    // ����ʼ����
    for (int i = 0; i < 3; ++i) {
        drawCard(_currentPlayer);                    // ��ǰ��ҳ���
        drawCard(_opponentPlayer);                   // ������ҳ���
    }
}

void GameManager::drawCard(Player* player) {         // ��ҳ��Ƶķ���
    if (player->getDeck().empty()) {                // ����ƿ�Ϊ��
        player->takeDamage(player->getFatigueDamage()); // ���ƣ���˺�
        player->increaseFatigueDamage();            // �����´�ƣ���˺�ֵ
        return;                                     // ��������
    }
    Card* card = player->getDeck().back();          // ��ȡ�ƿⶥ���Ŀ���
    player->getDeck().pop_back();                   // ���ƿ��Ƴ��ÿ���
    player->getHand().push_back(card);              // ��������ӵ�����
}
void GameManager::discardCard(Card* card) {
    if (!card) return;

    // ��ȡ�������������
    Player* owner = card->getOwner();
    if (!owner) return;

    // ���������Ƴ�
    auto& hand = owner->getHand();
    auto it = std::find(hand.begin(), hand.end(), card);
    if (it != hand.end()) {
        hand.erase(it);
    }

    // �ӳ����Ƴ�
    auto& field = owner->getField();
    it = std::find(field.begin(), field.end(), card);
    if (it != field.end()) {
        field.erase(it);
    }

    // ��ӵ����ƶ�
    owner->getDiscardPile().push_back(card);

    // ��¼��־
    GameLogger::getInstance()->log(LogLevel::INFO,
        "Card discarded: " + card->getCardName());
}
Card* GameManager::getSelectedTarget() const {
    return _selectedTarget;
}
