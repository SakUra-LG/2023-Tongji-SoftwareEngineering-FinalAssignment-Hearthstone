#ifndef __BATTLECRY_EFFECT_H__
#define __BATTLECRY_EFFECT_H__

#include "IEffect.h"
#include <functional>

class BattlecryEffect : public IEffect {
public:
    // 使用函数对象来定义战吼效果
    BattlecryEffect(std::function<void()> effect) : _effect(effect) {}
    
    virtual bool canActivate() const override {
        return true;  // 战吼总是可以触发
    }
    
    virtual void onActivate() override {
        if (_effect) {
            _effect();  // 执行战吼效果
        }
    }

private:
    std::function<void()> _effect;  // 存储战吼效果的函数对象
};

#endif // __BATTLECRY_EFFECT_H__ 