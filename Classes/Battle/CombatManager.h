#ifndef __COMBAT_MANAGER_H__
#define __COMBAT_MANAGER_H__

#include "cocos2d.h"

class Card;
class Player;

class CombatManager {
public:
    static CombatManager* getInstance();

    // ս�����
    bool canAttack(Card* attacker, Card* target);
    void performAttack(Card* attacker, Card* target);
    void dealDamage(Card* target, int amount);
    void healTarget(Card* target, int amount);

    // ״̬���
    void checkDeaths();
    bool isValidTarget(Card* target);

private:
    CombatManager();
    static CombatManager* _instance;

    void resolveEffects(Card* attacker, Card* target);
    void handleDeath(Card* card);

    std::vector<Card*> _deathQueue;// ����׷�������Ŀ���
};

#endif // __COMBAT_MANAGER_H__