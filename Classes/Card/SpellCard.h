#pragma execution_character_set("utf-8")

#ifndef __SPELL_CARD_H__
#define __SPELL_CARD_H__

#include "Card.h"

enum class SpellTarget {
    NONE,           // Ŀ
    SINGLE_TARGET,  // Ŀ
    ALL_MINIONS,    // 
    ALL_CHARACTERS, // нɫ
    FRIENDLY,       // ѷ
    ENEMY          //з
};

class SpellCard : public Card {
public:
    static SpellCard* create(int id, const std::string& name);

    CC_SYNTHESIZE(SpellTarget, _targetType, TargetType);
    CC_SYNTHESIZE(bool, _needsTarget, NeedsTarget);

    // 
    virtual void castSpell(Card* target = nullptr);
    virtual bool canTargetCard(Card* target) const;

protected:
    virtual bool init(int id, const std::string& name) override;
    virtual void initUI() override;

    // 
    virtual void playSpellEffect();
    virtual void onSpellComplete();
};

#endif // __SPELL_CARD_H__