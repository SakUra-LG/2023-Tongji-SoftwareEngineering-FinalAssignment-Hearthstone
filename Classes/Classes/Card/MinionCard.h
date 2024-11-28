 
#ifndef __MINION_CARD_H__
#define __MINION_CARD_H__

#include "Card.h"

class MinionCard : public Card {
public:
    static MinionCard* create(int id, const std::string& name);

    // �����������
    CC_SYNTHESIZE(int, _attack, Attack);               // ������
    CC_SYNTHESIZE(int, _health, Health);               // ����ֵ
    CC_SYNTHESIZE(int, _maxHealth, MaxHealth);         // �������ֵ
    CC_SYNTHESIZE(bool, _canAttack, CanAttack);        // �Ƿ���Թ���
    CC_SYNTHESIZE(bool, _hasTaunt, HasTaunt);          // �Ƿ��г���
    CC_SYNTHESIZE(bool, _hasDivineShield, HasDivineShield); // �Ƿ���ʥ��

    // ս�����
    virtual bool canAttackTarget(Card* target) const;
    virtual void attackTarget(Card* target);
    virtual void takeDamage(int amount);
    virtual void heal(int amount);
    // ��� playCard ��������
    virtual void playCard();
    // ״̬Ч��
    void addBuff(int attack, int health);
    void silence();                                    // ��Ĭ

protected:
    virtual bool init(int id, const std::string& name) override;
    virtual void initUI() override;
    virtual void updateUI();

    // ս�������
    virtual void onBattlecry();
    virtual void onDeathrattle();

private:
    void checkDeath();
    void updateStats();

    Label* _attackLabel;
    Label* _healthLabel;
};

#endif // __MINION_CARD_H__