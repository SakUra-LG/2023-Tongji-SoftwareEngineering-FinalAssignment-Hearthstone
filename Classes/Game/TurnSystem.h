#ifndef __TURN_SYSTEM_H__
#define __TURN_SYSTEM_H__
#pragma execution_character_set("utf-8")
#include "cocos2d.h"

class TurnSystem {
public:
    static TurnSystem* getInstance();

    // 回合控制
    void startTurn();
    void endTurn();
    bool isPlayerTurn() const { return _isPlayerTurn; }

    // 法力水晶
    void addManaSlot();
    void refillMana();
    bool useMana(int cost);
    int getCurrentMana() const { return _currentMana; }
    int getMaxMana() const { return _maxMana; }

private:
    TurnSystem();
    static TurnSystem* _instance;

    bool _isPlayerTurn;
    int _currentMana;
    int _maxMana;
    int _turnCount;

    // 禁止拷贝
    TurnSystem(const TurnSystem&) = delete;
    TurnSystem& operator=(const TurnSystem&) = delete;
};

#endif // __TURN_SYSTEM_H__