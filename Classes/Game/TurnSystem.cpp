#include "TurnSystem.h"
#include "Manager/GameManager.h"
#include "Effect/EffectManager.h"

TurnSystem* TurnSystem::_instance = nullptr;

TurnSystem* TurnSystem::getInstance() {
    if (!_instance) {
        _instance = new TurnSystem();
    }
    return _instance;
}

TurnSystem::TurnSystem()
    : _isPlayerTurn(true)
    , _currentMana(0)
    , _maxMana(0)
    , _turnCount(0) {
}

void TurnSystem::startTurn() {
    _turnCount++;

    // ���ӷ���ˮ��
    if (_maxMana < 10) {
        addManaSlot();
    }
    refillMana();

    // ��һ����
    auto player = GameManager::getInstance()->getCurrentPlayer();
    GameManager::getInstance()->drawCard(player);

    // �����غϿ�ʼЧ��
    EffectManager::getInstance()->triggerEffects(TriggerType::START_OF_TURN);
}

void TurnSystem::endTurn() {
    // �����غϽ���Ч��
    EffectManager::getInstance()->triggerEffects(TriggerType::END_OF_TURN);

    // �л����
    _isPlayerTurn = !_isPlayerTurn;

    // ��ʼ�»غ�
    startTurn();
}

void TurnSystem::addManaSlot() {
    _maxMana = std::min(10, _maxMana + 1);
}

void TurnSystem::refillMana() {
    _currentMana = _maxMana;
}

bool TurnSystem::useMana(int cost) {
    if (_currentMana >= cost) {
        _currentMana -= cost;
        return true;
    }
    return false;
}