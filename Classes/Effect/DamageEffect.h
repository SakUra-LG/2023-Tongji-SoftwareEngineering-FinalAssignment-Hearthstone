#ifndef __DAMAGE_EFFECT_H__
#define __DAMAGE_EFFECT_H__
#pragma execution_character_set("utf-8")
#include "Effect/IEffect.h"

class DamageEffect : public IEffect {
public:
    DamageEffect(int damage, bool targetAll = false);

    virtual void onActivate() override;
    virtual void onDeactivate() override;
    virtual bool canActivate() override;
    bool canActivate(Card* target) const;
    virtual TriggerType getTriggerType() const override { return TriggerType::ON_PLAY; }

    void setDamage(int damage) { _damage = damage; }
    int getDamage() const { return _damage; }

private:
    int _damage;
    bool _targetAll;

    void damageTarget(Card* target);
    void damageAllTargets();
};

#endif // __DAMAGE_EFFECT_H__