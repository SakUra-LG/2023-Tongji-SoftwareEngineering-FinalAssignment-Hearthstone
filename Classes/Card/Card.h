#ifndef __CARD_H__
#define __CARD_H__

#include "cocos2d.h"
#include "Effect/IEffect.h"
#include <vector>
#include <memory>

USING_NS_CC;

class Player;  // ǰ������
class IEffect; // ǰ������

// ��������ö��
enum class CardType {
    MINION,     // ���
    SPELL,      // ����
    WEAPON,     // ����
    HERO        // Ӣ��
};

// ����ϡ�ж�
enum class CardRarity {
    NORMAL,     // ��ͨ
    RARE,       // ϡ��
    EPIC,       // ʷʫ
    LEGENDARY   // ��˵
};

class Card : public cocos2d::Sprite {
public:
    // �����ͳ�ʼ��
    static Card* create(int id, const std::string& name);
    virtual bool init(int id, const std::string& name);

    // �������� (ʹ�� CC_SYNTHESIZE ��)
    CC_SYNTHESIZE(int, _id, Id);                       // ����ID
    CC_SYNTHESIZE(std::string, _cardName, CardName);   // ��������
    CC_SYNTHESIZE(int, _cost, Cost);                   // ����ֵ����
    CC_SYNTHESIZE(std::string, _description, Description); // ��������
    CC_SYNTHESIZE(CardType, _cardType, CardType);      // ��������
    CC_SYNTHESIZE(CardRarity, _rarity, Rarity);        // ����ϡ�ж�
    CC_SYNTHESIZE(int, _health, Health);               // ��ǰ����ֵ
    CC_SYNTHESIZE(int, _maxHealth, MaxHealth);         // �������ֵ
    CC_SYNTHESIZE(int, _attack, Attack);               // ������

    // ״̬����
    CC_SYNTHESIZE(bool, _isPlayable, IsPlayable);      // �Ƿ���Դ��
    CC_SYNTHESIZE(bool, _isSelected, IsSelected);      // �Ƿ�ѡ��
    CC_SYNTHESIZE(bool, _hasEffect, HasEffect);        // �Ƿ�����Ч
    CC_SYNTHESIZE(bool, _isActive, IsActive);          // �Ƿ��ڻ�Ծ״̬
    CC_SYNTHESIZE(bool, _isSilenced, IsSilenced);      // �Ƿ񱻳�Ĭ
    CC_SYNTHESIZE(bool, _isFrozen, IsFrozen);         // �Ƿ񱻶���
    CC_SYNTHESIZE(bool, _hasProvoke, HasProvoke);      // �Ƿ���г���
    CC_SYNTHESIZE(bool, _hasDivineShield, HasDivineShield); // �Ƿ����ʥ��
    CC_SYNTHESIZE(bool, _hasAttacked, HasAttacked);    // �Ƿ��Ѿ�����
    CC_SYNTHESIZE(Player*, _owner, Owner);             // �����������

    // ��Ϸ��Ϊ
    virtual bool canPlay() const;                      // ����Ƿ���Դ��
    virtual void onPlay();                             // ���ʱ����
    virtual void onDraw();                             // �鵽ʱ����
    virtual void onDiscard();                          // ����ʱ����

    // ս�����
    virtual void heal(int amount);                     // ����
    virtual bool isActive() const;                     // ����Ծ״̬

    // Ч��ϵͳ
    void addEffect(std::shared_ptr<IEffect> effect);
    void removeEffect(std::shared_ptr<IEffect> effect);
    void triggerEffects();

    // UI���
    void showPlayableHighlight();
    void returnToOriginalPosition();
    bool isInPlayableArea(const cocos2d::Vec2& position);

protected:
    // UI���
    Label* _nameLabel;
    Label* _costLabel;
    Label* _descriptionLabel;
    Sprite* _cardFrame;
    Sprite* _cardImage;
    Sprite* _highlightSprite;

    // λ�����
    cocos2d::Vec2 _originalPosition;

    // Ч���б�
    std::vector<std::shared_ptr<IEffect>> _effects;

    // ��ʼ��UI
    virtual void initUI();

    // �����¼�
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
};

#endif // __CARD_H__