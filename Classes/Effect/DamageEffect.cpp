#include "DamageEffect.h"
#include "Card/Card.h"
#include "Manager/GameManager.h"
#include "Animation/AnimationManager.h"
#include "Player/Player.h"
#include"Card/MinionCard.h"
DamageEffect::DamageEffect(int damage, bool targetAll)
    : _damage(damage)
    , _targetAll(targetAll) {
}

void DamageEffect::onActivate() {
    if (_targetAll) {
        damageAllTargets();
    }
    else {
        Card* target = GameManager::getInstance()->getSelectedTarget();
        if (target) {
            damageTarget(target);
        }
    }
}

void DamageEffect::onDeactivate() {
    // ����Ч�������Դ
}

bool DamageEffect::canActivate() {
    if (!_owner) return false;

    if (_targetAll) {
        return true;
    }
    else {
        return GameManager::getInstance()->getSelectedTarget() != nullptr;
    }
}

void DamageEffect::damageTarget(Card* target) {
    if (!target) return;

    // �����˺�����
    AnimationManager::getInstance()->playDamageAnimation(target, _damage);

    // ����˺�
    if (target->getCardType() == CardType::MINION) {
        dynamic_cast<MinionCard*>(target)->takeDamage(_damage);
    }
}

void DamageEffect::damageAllTargets() {
    auto gameManager = GameManager::getInstance();
    auto& playerField = gameManager->getCurrentPlayer()->getField();
    auto& opponentField = gameManager->getOpponentPlayer()->getField();

    // �������������˺�
    for (auto minion : playerField) {
        damageTarget(minion);
    }

    for (auto minion : opponentField) {
        damageTarget(minion);
    }
}