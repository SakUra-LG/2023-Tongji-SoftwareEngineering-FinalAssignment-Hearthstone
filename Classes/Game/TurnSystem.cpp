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

    // 增加法力水晶
    if (_maxMana < 10) {
        addManaSlot();
    }
    refillMana();

    // 抽一张牌
    auto player = GameManager::getInstance()->getCurrentPlayer();
    GameManager::getInstance()->drawCard(player);

    // 触发回合开始效果
    EffectManager::getInstance()->triggerEffects(TriggerType::START_OF_TURN);
}

void TurnSystem::endTurn() {
    // 触发回合结束效果
    EffectManager::getInstance()->triggerEffects(TriggerType::END_OF_TURN);

    // 切换玩家
    _isPlayerTurn = !_isPlayerTurn;

    // 开始新回合
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