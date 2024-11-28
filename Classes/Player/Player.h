#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include <vector>

class Card;

class Player {
public:
    Player();
    ~Player();
    // ��������
    CC_SYNTHESIZE(int, _health, Health);
    CC_SYNTHESIZE(int, _maxHealth, MaxHealth);
    CC_SYNTHESIZE(int, _armor, Armor);
    CC_SYNTHESIZE(int, _fatigueDamage, FatigueDamage);

    // ����ֵ���
    CC_SYNTHESIZE(int, _mana, Mana);              // ��ǰ����ֵ
    CC_SYNTHESIZE(int, _maxMana, MaxMana);        // �����ֵ
    CC_SYNTHESIZE(int, _tempMana, TempMana);      // ��ʱ����ֵ�����غϿ��ã�

    // ����ֵ����
    void spendMana(int amount);                   // ���ķ���ֵ
    void restoreMana();                           // �ָ�����ֵ
    void gainMana(int amount);                    // �����ʱ����ֵ
    void incrementMaxMana();                      // ���������ֵ
    bool hasEnoughMana(int cost) const;           // ��鷨��ֵ�Ƿ��㹻

    // ���ƹ���
    std::vector<Card*>& getDeck() { return _deck; }
    std::vector<Card*>& getHand() { return _hand; }
    std::vector<Card*>& getField() { return _field; }
    std::vector<Card*>& getDiscardPile() { return _discardPile; }
    // ��Ϸ��Ϊ
    void takeDamage(int amount);
    void heal(int amount);
    void addArmor(int amount);
    void increaseFatigueDamage();
    void removeCardFromHand(Card* card);
    bool hasValidTaunt() const;
    void removeFromField(Card* card);
    // ״̬���
    bool isDead() const { return _health <= 0; }
    bool canDrawCard() const { return !_deck.empty(); }

private:
    std::vector<Card*> _deck;   // �ƿ�
    std::vector<Card*> _hand;   // ����
    std::vector<Card*> _field;  // �������
    std::vector<Card*> _discardPile;//����
};

#endif // __PLAYER_H__