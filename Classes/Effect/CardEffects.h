#ifndef __CARD_EFFECTS_H__
#define __CARD_EFFECTS_H__
#pragma execution_character_set("utf-8")
#include "IEffect.h"
#include <functional>

/**
 * @class BattlecryEffect
 * @brief 战吼效果类
 * 
 * 实现了随从卡牌的战吼效果。
 * 战吼效果在随从被打出时触发一次。
 */
class BattlecryEffect : public IEffect {
public:
    /**
     * @brief 构造函数
     * @param effect 战吼效果的具体实现函数
     */
    BattlecryEffect(std::function<void()> effect) : _effect(effect) {}
    
    virtual void onActivate() override { if (_effect) _effect(); }
    virtual void onDeactivate() override {}
    virtual bool canActivate() override { return true; }
    virtual TriggerType getTriggerType() const override { return TriggerType::BATTLECRY; }
    virtual void setOwner(Card* owner) override { _owner = owner; }

private:
    std::function<void()> _effect;  // 存储战吼效果的函数对象
};

/**
 * @class DivineShieldEffect
 * @brief 圣盾效果类
 * 
 * 实现了随从的圣盾效果。
 * 圣盾可以抵消一次伤害。
 */
class DivineShieldEffect : public IEffect {
public:
    virtual void onActivate() override {}
    virtual void onDeactivate() override {}
    virtual bool canActivate() override { return true; }
    virtual TriggerType getTriggerType() const override { return TriggerType::ON_DAMAGE; }
    virtual void setOwner(Card* owner) override { _owner = owner; }
};

/**
 * @class TauntEffect
 * @brief 嘲讽效果类
 * 
 * 实现了随从的嘲讽效果。
 * 具有嘲讽的随从必须优先被攻击。
 */
class TauntEffect : public IEffect {
public:
    virtual void onActivate() override {}
    virtual void onDeactivate() override {}
    virtual bool canActivate() override { return true; }
    virtual TriggerType getTriggerType() const override { return TriggerType::ON_PLAY; }
    virtual void setOwner(Card* owner) override { _owner = owner; }
};

/**
 * @class OutcastEffect
 * @brief 流放效果类
 * 
 * 实现了恶魔猎手的流放效果。
 * 当卡牌在手牌最左或最右侧时触发特殊效果。
 */
class OutcastEffect : public IEffect {
public:
    OutcastEffect(std::function<void()> effect) : _effect(effect) {}
    
    virtual void onActivate() override { if (_effect) _effect(); }
    virtual void onDeactivate() override {}
    virtual bool canActivate() override { return true; }
    virtual TriggerType getTriggerType() const override { return TriggerType::ON_PLAY; }
    virtual void setOwner(Card* owner) override { _owner = owner; }

private:
    std::function<void()> _effect;  // 存储流放效果的函数对象
};

/**
 * @class RushEffect
 * @brief 突袭效果类
 * 
 * 实现了随从的突袭效果。
 * 具有突袭的随从在打出的回合可以攻击其他随从。
 */
class RushEffect : public IEffect {
public:
    virtual void onActivate() override {}
    virtual void onDeactivate() override {}
    virtual bool canActivate() override { return true; }
    virtual TriggerType getTriggerType() const override { return TriggerType::ON_PLAY; }
    virtual void setOwner(Card* owner) override { _owner = owner; }
};

/**
 * @class ChargeEffect
 * @brief 冲锋效果类
 * 
 * 实现了随从的冲锋效果。
 * 具有冲锋的随从在打出的回合就可以攻击。
 */
class ChargeEffect : public IEffect {
public:
    virtual void onActivate() override {}
    virtual void onDeactivate() override {}
    virtual bool canActivate() override { return true; }
    virtual TriggerType getTriggerType() const override { return TriggerType::ON_PLAY; }
    virtual void setOwner(Card* owner) override { _owner = owner; }
};

/**
 * @class PoisonousEffect
 * @brief 剧毒效果类
 * 
 * 实现了随从的剧毒效果。
 * 具有剧毒的随从无论造成多少伤害都会消灭目标。
 */
class PoisonousEffect : public IEffect {
public:
    virtual void onActivate() override {}
    virtual void onDeactivate() override {}
    virtual bool canActivate() override { return true; }
    virtual TriggerType getTriggerType() const override { return TriggerType::ON_ATTACK; }
    virtual void setOwner(Card* owner) override { _owner = owner; }
};

#endif // __CARD_EFFECTS_H__ 