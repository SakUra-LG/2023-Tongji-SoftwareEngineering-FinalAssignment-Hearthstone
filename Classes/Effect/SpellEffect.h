#ifndef __SPELL_EFFECT_H__
#define __SPELL_EFFECT_H__
#pragma execution_character_set("utf-8")
#include "IEffect.h"
#include <functional>

/**
 * @class SpellEffect
 * @brief 法术效果类
 * 
 * 实现了法术卡牌的效果系v统。
 * 法术效果在卡牌被打出时触发一次。
 */
class SpellEffect : public IEffect {
public:
    // 使用 std::function 定义回调函数类型
    using SpellCallback = std::function<void(Card*)>;
    
    // 构造函数接受一个回调函数
    explicit SpellEffect(SpellCallback callback) 
        : _callback(callback)
        , _owner(nullptr) 
    {}
    
    // 实现基类的所有虚函数
    virtual void onActivate() override {
        if (_callback && _owner) {
            _callback(_owner);
        }
    }
    
    virtual void onDeactivate() override {
        // 法术效果通常是一次性的，不需要特殊的清理
    }
    
    virtual bool canActivate() override {
        return _owner != nullptr;
    }
    
    virtual TriggerType getTriggerType() const override {
        return TriggerType::ON_PLAY;
    }
    
    virtual void setOwner(Card* owner) override {
        _owner = owner;
    }
    
private:
    SpellCallback _callback;
    Card* _owner;
};

#endif // __SPELL_EFFECT_H__ 