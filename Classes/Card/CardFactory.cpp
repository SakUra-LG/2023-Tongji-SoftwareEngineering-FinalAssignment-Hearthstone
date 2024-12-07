#include "CardFactory.h"
#include "Manager/GameManager.h"
#include "Effect/CardEffects.h"
#include "Effect/SpellEffect.h"

CardFactory* CardFactory::_instance = nullptr;

CardFactory::CardFactory() {
    initCardTemplates();
}

CardFactory* CardFactory::getInstance() {
    if (!_instance) {
        _instance = new CardFactory();
    }
    return _instance;
}

void CardFactory::initCardTemplates() {
    // 新手战士
    {
        CardData warrior;
        warrior.id = 1001;
        warrior.name = "新手战士";
        warrior.type = CardType::MINION;
        warrior.rarity = CardRarity::NORMAL;
        warrior.cost = 1;
        warrior.attack = 1;
        warrior.health = 2;
        warrior.description = "A soldier who has just joined the army.";
        warrior.framePath = "cards/frames/normal.png";
        warrior.portraitPath = "cards/portraits/rookie_warrior.png";
        _cardTemplates[1001] = warrior;
    }
    
    // 老练士兵
    {
        CardData veteran;
        veteran.id = 1002;
        veteran.name = "老练士兵";
        veteran.type = CardType::MINION;
        veteran.rarity = CardRarity::RARE;
        veteran.cost = 2;
        veteran.attack = 2;
        veteran.health = 3;
        veteran.description = "Battlecry: Gain +1 Attack";
        veteran.framePath = "cards/frames/rare.png";
        veteran.portraitPath = "cards/portraits/veteran_soldier.png";
        _cardTemplates[1002] = veteran;
    }
    
    // 圣盾战士
    {
        CardData shield;
        shield.id = 1003;
        shield.name = "圣盾战士";
        shield.type = CardType::MINION;
        shield.rarity = CardRarity::RARE;
        shield.cost = 3;
        shield.attack = 2;
        shield.health = 2;
        shield.description = "圣盾";
        shield.framePath = "cards/frames/rare.png";
        shield.portraitPath = "cards/portraits/divine_shield_warrior.png";
        _cardTemplates[1003] = shield;
    }

    // 守护者
    {
        CardData guard;
        guard.id = 1004;
        guard.name = "Guardian";
        guard.type = CardType::MINION;
        guard.rarity = CardRarity::RARE;
        guard.cost = 4;
        guard.attack = 3;
        guard.health = 5;
        guard.description = "Taunt, Battlecry: Gain +1/+1";
        guard.framePath = "cards/frames/rare.png";
        guard.portraitPath = "cards/portraits/guardian.png";
        _cardTemplates[1004] = guard;
    }
    
    // 火球术
    {
        CardData spell;
        spell.id = 2001;
        spell.name = "Fireball";
        spell.type = CardType::SPELL;
        spell.rarity = CardRarity::NORMAL;
        spell.cost = 4;
        spell.attack = 0;
        spell.health = 0;
        spell.description = "Deals 6 damage to a target";
        spell.framePath = "cards/frames/normal_spell.png";
        spell.portraitPath = "cards/portraits/fireball.png";
        _cardTemplates[2001] = spell;
    }
}

Card* CardFactory::createCardById(int cardId) {
    auto it = _cardTemplates.find(cardId);
    if (it == _cardTemplates.end()) {
        return nullptr;
    }
    
    const CardData& data = it->second;
    
    switch (data.type) {
        case CardType::MINION:
            return createMinionCard(data);
        case CardType::SPELL:
            return createSpellCard(data);
        default:
            return nullptr;
    }
}

MinionCard* CardFactory::createMinionCard(const CardData& data) {
    auto minion = MinionCard::create(data.id, data.name);
    if (minion) {
        // 设置基本属性
        minion->setCardType(data.type);
        minion->setRarity(data.rarity);
        minion->setCost(data.cost);
        minion->setAttack(data.attack);
        minion->setHealth(data.health);
        minion->setMaxHealth(data.health);
        minion->setDescription(data.description);
        
        // 添加框架
        auto frame = Sprite::create(data.framePath);
        if (frame) {
            minion->addChild(frame, 1);  // z-order=1，确保在立绘上层
        }
        
        // 添加立绘
        auto portrait = Sprite::create(data.portraitPath);
        if (portrait) {
            minion->addChild(portrait, 0);  // z-order=0，确保在框架下层
        }
        
        // 根据卡牌ID添加特殊效果
        switch (data.id) {
            case 1002: // 老练士兵
                minion->addEffect(std::make_shared<BattlecryEffect>([=]() {
                    minion->setAttack(minion->getAttack() + 1);
                }));
                break;
                
            case 1003: // 圣盾战士
                minion->addEffect(std::make_shared<DivineShieldEffect>());
                break;
                
            case 1004: // 守护者
                minion->addEffect(std::make_shared<TauntEffect>());
                minion->addEffect(std::make_shared<BattlecryEffect>([=]() {
                    minion->setAttack(minion->getAttack() + 1);
                    minion->setHealth(minion->getHealth() + 1);
                    minion->setMaxHealth(minion->getMaxHealth() + 1);
                }));
                break;
        }
    }
    return minion;
}

SpellCard* CardFactory::createSpellCard(const CardData& data) {
    auto spell = SpellCard::create(data.id, data.name);
    if (spell) {
        // 设置基本属性
        spell->setCardType(data.type);
        spell->setRarity(data.rarity);
        spell->setCost(data.cost);
        spell->setDescription(data.description);
        
        // 添��框架
        auto frame = Sprite::create(data.framePath);
        if (frame) {
            spell->addChild(frame, 1);
        }
        
        // 添加立绘
        auto portrait = Sprite::create(data.portraitPath);
        if (portrait) {
            spell->addChild(portrait, 0);
        }
        
        // 添加特殊效果
        if (data.id == 2001) { // 火球术的效果
            spell->addEffect(std::make_shared<SpellEffect>([=](Card* target) {
                if (target) {
                    target->takeDamage(6);
                }
            }));
        }
    }
    return spell;
} 