#ifndef __SPELL_EFFECT_H__
#define __SPELL_EFFECT_H__

#include "IEffect.h"
#include <functional>

/**
 * @class SpellEffect
 * @brief 法术效果类
 * 
 * 实现了法术卡牌的效果系统。
 * 法术效果在卡牌被打出时触发一次。
 */
class SpellEffect : public IEffect {
public:
    /**
     * @brief 构造函数
     * @param effect 法术效果的具体实现函数
     */
    SpellEffect(std::function<void(Card*)> effect) : _effect(effect) {}
    
    virtual void onActivate() override { if (_effect && _owner) _effect(_owner); }
    virtual void onDeactivate() override {}
    virtual bool canActivate() override { return true; }
    virtual TriggerType getTriggerType() const override { return TriggerType::ON_PLAY; }
    virtual void setOwner(Card* owner) override { _owner = owner; }

private:
    std::function<void(Card*)> _effect;  // 存储法术效果的函数对象
};

#endif // __SPELL_EFFECT_H__ 