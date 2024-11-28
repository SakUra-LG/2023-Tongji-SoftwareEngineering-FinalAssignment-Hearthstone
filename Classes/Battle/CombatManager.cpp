#include "CombatManager.h"
#include "Card/Card.h"
#include "Player/Player.h"
#include "Effect/EffectManager.h"
#include "Effect/IEffect.h"
#include "Animation/AnimationManager.h"
#include"Utils/GameLogger.h"
#include <algorithm>  // 为 std::min 添加
CombatManager* CombatManager::_instance = nullptr;

CombatManager* CombatManager::getInstance() {
    if (!_instance) {
        _instance = new CombatManager();
    }
    return _instance;
}
// 构造函数实现
CombatManager::CombatManager()
    : _deathQueue() {  // 初始化死亡队列
}
bool CombatManager::canAttack(Card* attacker, Card* target) {
    if (!attacker || !target) return false;
    if (!attacker->canPlay()) return false;
    if (!isValidTarget(target)) return false;
    return true;
}

void CombatManager::performAttack(Card* attacker, Card* target) {
    if (!canAttack(attacker, target)) return;

    // 播放攻击动画
    AnimationManager::getInstance()->playAttackAnimation(attacker, target);

    // 造成伤害
    dealDamage(target, attacker->getAttack());
    dealDamage(attacker, target->getAttack());

    // 设置攻击状态
    attacker->setHasAttacked(true);

    // 检查死亡
    checkDeaths();
}

void CombatManager::dealDamage(Card* target, int amount) {
    if (!target) return;
    // 触发受伤效果
    EffectManager::getInstance()->triggerEffects(TriggerType::ON_DAMAGE);
    // 计算实际伤害
    int actualDamage = amount;
    // 造成伤害
    target->setHealth(target->getHealth() - actualDamage);
    // 检查是否需要死亡
    if (target->getHealth() <= 0) {
        _deathQueue.push_back(target);
    }
}
bool CombatManager::isValidTarget(Card* target) {
    if (!target) return false;

    // 检查目标是否存活
    if (target->getHealth() <= 0) return false;

    // 检查目标是否可以被攻击（例如：潜行状态）
    return true;
}
void CombatManager::checkDeaths() {
    // 处理死亡队列中的卡牌
    for (auto card : _deathQueue) {
        handleDeath(card);
    }

    // 清空死亡队列
    _deathQueue.clear();
}
void CombatManager::handleDeath(Card* card) {
    if (!card) return;

    // 触发死亡效果
    EffectManager::getInstance()->triggerEffects(TriggerType::ON_DEATH);

    // 从场上移除
    card->getOwner()->removeFromField(card);

    // 记录日志
    GameLogger::getInstance()->log(LogLevel::INFO,
        "Card died: " + card->getCardName());
}
void CombatManager::healTarget(Card* target, int amount) {
    if (!target) return;

    int currentHealth = target->getHealth();
    int maxHealth = target->getMaxHealth();

    // 计算实际治疗量
    int actualHeal = std::min(amount, maxHealth - currentHealth);

    if (actualHeal > 0) {
        target->setHealth(currentHealth + actualHeal);

        // 触发治疗效果
        EffectManager::getInstance()->triggerEffects(TriggerType::ON_HEAL);

        // 记录日志
        GameLogger::getInstance()->log(LogLevel::INFO,
            "Healed " + target->getCardName() + " for " + std::to_string(actualHeal));
    }
}
void CombatManager::resolveEffects(Card* attacker, Card* target) {
    if (!attacker || !target) return;

    // 触发攻击效果
    EffectManager::getInstance()->triggerEffects(TriggerType::ON_ATTACK);    // 使用作用域运算符

    // 触发被攻击效果
    EffectManager::getInstance()->triggerEffects(TriggerType::ON_ATTACKED);  // 使用作用域运算符
}