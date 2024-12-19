#pragma execution_character_set("utf-8")
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
    // 清理效果相关资源
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

bool DamageEffect::canActivate(Card* target) const {
    return target && target->getType() == CardType::MINION;
}

void DamageEffect::damageTarget(Card* target) {
    if (!target) return;

    // 播放伤害动画
    AnimationManager::getInstance()->playDamageAnimation(target, _damage);

    // 造成伤害
    if (target->getCardType() == CardType::MINION) {
        dynamic_cast<MinionCard*>(target)->takeDamage(_damage);
    }
}

void DamageEffect::damageAllTargets() {
    auto gameManager = GameManager::getInstance();
    auto& playerField = gameManager->getCurrentPlayer()->getField();
    auto& opponentField = gameManager->getOpponentPlayer()->getField();

    // 对所有随从造成伤害
    for (auto minion : playerField) {
        damageTarget(minion);
    }

    for (auto minion : opponentField) {
        damageTarget(minion);
    }
}