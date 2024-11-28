#ifndef __CARD_H__
#define __CARD_H__

#include "cocos2d.h"
#include "Effect/IEffect.h"
#include <vector>
#include <memory>

USING_NS_CC;

class Player;  // 前向声明
class IEffect; // 前向声明

// 卡牌类型枚举
enum class CardType {
    MINION,     // 随从
    SPELL,      // 法术
    WEAPON,     // 武器
    HERO        // 英雄
};

// 卡牌稀有度
enum class CardRarity {
    NORMAL,     // 普通
    RARE,       // 稀有
    EPIC,       // 史诗
    LEGENDARY   // 传说
};

class Card : public cocos2d::Sprite {
public:
    // 创建和初始化
    static Card* create(int id, const std::string& name);
    virtual bool init(int id, const std::string& name);

    // 基础属性 (使用 CC_SYNTHESIZE 宏)
    CC_SYNTHESIZE(int, _id, Id);                       // 卡牌ID
    CC_SYNTHESIZE(std::string, _cardName, CardName);   // 卡牌名称
    CC_SYNTHESIZE(int, _cost, Cost);                   // 法力值消耗
    CC_SYNTHESIZE(std::string, _description, Description); // 卡牌描述
    CC_SYNTHESIZE(CardType, _cardType, CardType);      // 卡牌类型
    CC_SYNTHESIZE(CardRarity, _rarity, Rarity);        // 卡牌稀有度
    CC_SYNTHESIZE(int, _health, Health);               // 当前生命值
    CC_SYNTHESIZE(int, _maxHealth, MaxHealth);         // 最大生命值
    CC_SYNTHESIZE(int, _attack, Attack);               // 攻击力

    // 状态属性
    CC_SYNTHESIZE(bool, _isPlayable, IsPlayable);      // 是否可以打出
    CC_SYNTHESIZE(bool, _isSelected, IsSelected);      // 是否被选中
    CC_SYNTHESIZE(bool, _hasEffect, HasEffect);        // 是否有特效
    CC_SYNTHESIZE(bool, _isActive, IsActive);          // 是否处于活跃状态
    CC_SYNTHESIZE(bool, _isSilenced, IsSilenced);      // 是否被沉默
    CC_SYNTHESIZE(bool, _isFrozen, IsFrozen);         // 是否被冻结
    CC_SYNTHESIZE(bool, _hasProvoke, HasProvoke);      // 是否具有嘲讽
    CC_SYNTHESIZE(bool, _hasDivineShield, HasDivineShield); // 是否具有圣盾
    CC_SYNTHESIZE(bool, _hasAttacked, HasAttacked);    // 是否已经攻击
    CC_SYNTHESIZE(Player*, _owner, Owner);             // 卡牌所属玩家

    // 游戏行为
    virtual bool canPlay() const;                      // 检查是否可以打出
    virtual void onPlay();                             // 打出时触发
    virtual void onDraw();                             // 抽到时触发
    virtual void onDiscard();                          // 弃牌时触发

    // 战斗相关
    virtual void heal(int amount);                     // 治疗
    virtual bool isActive() const;                     // 检查活跃状态

    // 效果系统
    void addEffect(std::shared_ptr<IEffect> effect);
    void removeEffect(std::shared_ptr<IEffect> effect);
    void triggerEffects();

    // UI相关
    void showPlayableHighlight();
    void returnToOriginalPosition();
    bool isInPlayableArea(const cocos2d::Vec2& position);

protected:
    // UI组件
    Label* _nameLabel;
    Label* _costLabel;
    Label* _descriptionLabel;
    Sprite* _cardFrame;
    Sprite* _cardImage;
    Sprite* _highlightSprite;

    // 位置相关
    cocos2d::Vec2 _originalPosition;

    // 效果列表
    std::vector<std::shared_ptr<IEffect>> _effects;

    // 初始化UI
    virtual void initUI();

    // 触摸事件
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
};

#endif // __CARD_H__