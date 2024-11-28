 
#ifndef __SPELL_CARD_H__
#define __SPELL_CARD_H__

#include "Card.h"

enum class SpellTarget {
    NONE,           // 无目标
    SINGLE_TARGET,  // 单体目标
    ALL_MINIONS,    // 所有随从
    ALL_CHARACTERS, // 所有角色
    FRIENDLY,       // 友方
    ENEMY          // 敌方
};

class SpellCard : public Card {
public:
    static SpellCard* create(int id, const std::string& name);

    CC_SYNTHESIZE(SpellTarget, _targetType, TargetType);
    CC_SYNTHESIZE(bool, _needsTarget, NeedsTarget);

    // 法术效果
    virtual void castSpell(Card* target = nullptr);
    virtual bool canTargetCard(Card* target) const;

protected:
    virtual bool init(int id, const std::string& name) override;
    virtual void initUI() override;

    // 法术特效
    virtual void playSpellEffect();
    virtual void onSpellComplete();
};

#endif // __SPELL_CARD_H__