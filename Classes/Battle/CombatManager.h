#ifndef __COMBAT_MANAGER_H__
#define __COMBAT_MANAGER_H__
#pragma execution_character_set("utf-8")
#include "cocos2d.h"

class Card;
class Player;

class CombatManager {
public:
    static CombatManager* getInstance();

    // 战斗相关
    bool canAttack(Card* attacker, Card* target);
    void performAttack(Card* attacker, Card* target);
    void dealDamage(Card* target, int amount);
    void healTarget(Card* target, int amount);

    // 状态检查
    void checkDeaths();
    bool isValidTarget(Card* target);

private:
    CombatManager();
    static CombatManager* _instance;

    void resolveEffects(Card* attacker, Card* target);
    void handleDeath(Card* card);

    std::vector<Card*> _deathQueue;// 用于追踪死亡的卡牌
};

#endif // __COMBAT_MANAGER_H__