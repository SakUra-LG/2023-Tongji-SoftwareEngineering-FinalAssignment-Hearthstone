#pragma execution_character_set("utf-8")
#pragma execution_character_set("utf-8")
#ifndef __SPELL_CARD_H__
#define __SPELL_CARD_H__

#include "Card.h"

class SpellCard : public Card {
public:
    static SpellCard* create(int id, const std::string& name);
    void initUI2();  // 新的UI初始化方法

protected:
    virtual bool init(int id, const std::string& name) override;
    //virtual void initUI() override;
    virtual void updateUI();

private:
    // 法术牌特有的属性
    bool _hasBattlecry;
    bool _hasOutcast;
};

#endif // __SPELL_CARD_H__