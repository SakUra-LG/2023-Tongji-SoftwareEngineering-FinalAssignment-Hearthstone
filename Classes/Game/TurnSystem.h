#ifndef __TURN_SYSTEM_H__
#define __TURN_SYSTEM_H__

#include "cocos2d.h"

class TurnSystem {
public:
    static TurnSystem* getInstance();

    // �غϿ���
    void startTurn();
    void endTurn();
    bool isPlayerTurn() const { return _isPlayerTurn; }

    // ����ˮ��
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

    // ��ֹ����
    TurnSystem(const TurnSystem&) = delete;
    TurnSystem& operator=(const TurnSystem&) = delete;
};

#endif // __TURN_SYSTEM_H__