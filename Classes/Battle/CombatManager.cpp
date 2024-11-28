#include "CombatManager.h"
#include "Card/Card.h"
#include "Player/Player.h"
#include "Effect/EffectManager.h"
#include "Effect/IEffect.h"
#include "Animation/AnimationManager.h"
#include"Utils/GameLogger.h"
#include <algorithm>  // Ϊ std::min ���
CombatManager* CombatManager::_instance = nullptr;

CombatManager* CombatManager::getInstance() {
    if (!_instance) {
        _instance = new CombatManager();
    }
    return _instance;
}
// ���캯��ʵ��
CombatManager::CombatManager()
    : _deathQueue() {  // ��ʼ����������
}
bool CombatManager::canAttack(Card* attacker, Card* target) {
    if (!attacker || !target) return false;
    if (!attacker->canPlay()) return false;
    if (!isValidTarget(target)) return false;
    return true;
}

void CombatManager::performAttack(Card* attacker, Card* target) {
    if (!canAttack(attacker, target)) return;

    // ���Ź�������
    AnimationManager::getInstance()->playAttackAnimation(attacker, target);

    // ����˺�
    dealDamage(target, attacker->getAttack());
    dealDamage(attacker, target->getAttack());

    // ���ù���״̬
    attacker->setHasAttacked(true);

    // �������
    checkDeaths();
}

void CombatManager::dealDamage(Card* target, int amount) {
    if (!target) return;
    // ��������Ч��
    EffectManager::getInstance()->triggerEffects(TriggerType::ON_DAMAGE);
    // ����ʵ���˺�
    int actualDamage = amount;
    // ����˺�
    target->setHealth(target->getHealth() - actualDamage);
    // ����Ƿ���Ҫ����
    if (target->getHealth() <= 0) {
        _deathQueue.push_back(target);
    }
}
bool CombatManager::isValidTarget(Card* target) {
    if (!target) return false;

    // ���Ŀ���Ƿ���
    if (target->getHealth() <= 0) return false;

    // ���Ŀ���Ƿ���Ա����������磺Ǳ��״̬��
    return true;
}
void CombatManager::checkDeaths() {
    // �������������еĿ���
    for (auto card : _deathQueue) {
        handleDeath(card);
    }

    // �����������
    _deathQueue.clear();
}
void CombatManager::handleDeath(Card* card) {
    if (!card) return;

    // ��������Ч��
    EffectManager::getInstance()->triggerEffects(TriggerType::ON_DEATH);

    // �ӳ����Ƴ�
    card->getOwner()->removeFromField(card);

    // ��¼��־
    GameLogger::getInstance()->log(LogLevel::INFO,
        "Card died: " + card->getCardName());
}
void CombatManager::healTarget(Card* target, int amount) {
    if (!target) return;

    int currentHealth = target->getHealth();
    int maxHealth = target->getMaxHealth();

    // ����ʵ��������
    int actualHeal = std::min(amount, maxHealth - currentHealth);

    if (actualHeal > 0) {
        target->setHealth(currentHealth + actualHeal);

        // ��������Ч��
        EffectManager::getInstance()->triggerEffects(TriggerType::ON_HEAL);

        // ��¼��־
        GameLogger::getInstance()->log(LogLevel::INFO,
            "Healed " + target->getCardName() + " for " + std::to_string(actualHeal));
    }
}
void CombatManager::resolveEffects(Card* attacker, Card* target) {
    if (!attacker || !target) return;

    // ��������Ч��
    EffectManager::getInstance()->triggerEffects(TriggerType::ON_ATTACK);    // ʹ�������������

    // ����������Ч��
    EffectManager::getInstance()->triggerEffects(TriggerType::ON_ATTACKED);  // ʹ�������������
}