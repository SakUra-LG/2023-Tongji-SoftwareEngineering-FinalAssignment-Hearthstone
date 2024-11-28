 
#ifndef __MINION_CARD_H__
#define __MINION_CARD_H__

#include "Card.h"

class MinionCard : public Card {
public:
    static MinionCard* create(int id, const std::string& name);

    // 随从特有属性
    CC_SYNTHESIZE(int, _attack, Attack);               // 攻击力
    CC_SYNTHESIZE(int, _health, Health);               // 生命值
    CC_SYNTHESIZE(int, _maxHealth, MaxHealth);         // 最大生命值
    CC_SYNTHESIZE(bool, _canAttack, CanAttack);        // 是否可以攻击
    CC_SYNTHESIZE(bool, _hasTaunt, HasTaunt);          // 是否有嘲讽
    CC_SYNTHESIZE(bool, _hasDivineShield, HasDivineShield); // 是否有圣盾

    // 战斗相关
    virtual bool canAttackTarget(Card* target) const;
    virtual void attackTarget(Card* target);
    virtual void takeDamage(int amount);
    virtual void heal(int amount);
    // 添加 playCard 方法声明
    virtual void playCard();
    // 状态效果
    void addBuff(int attack, int health);
    void silence();                                    // 沉默

protected:
    virtual bool init(int id, const std::string& name) override;
    virtual void initUI() override;
    virtual void updateUI();

    // 战吼和亡语
    virtual void onBattlecry();
    virtual void onDeathrattle();

private:
    void checkDeath();
    void updateStats();

    Label* _attackLabel;
    Label* _healthLabel;
};

#endif // __MINION_CARD_H__