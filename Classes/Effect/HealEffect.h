#ifndef __HEAL_EFFECT_H__
#define __HEAL_EFFECT_H__

#include "Effect/IEffect.h"

class HealEffect : public IEffect {
public:
    HealEffect(int healing, bool targetAll = false);

    virtual void onActivate() override;
    virtual void onDeactivate() override;
    virtual bool canActivate() override;
    virtual TriggerType getTriggerType() const override { return TriggerType::ON_PLAY; }

    void setHealing(int healing) { _healing = healing; }
    int getHealing() const { return _healing; }

private:
    int _healing;
    bool _targetAll;

    void healTarget(Card* target);
    void healAllTargets();
};

#endif // __HEAL_EFFECT_H__
