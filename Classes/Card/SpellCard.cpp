// SpellCard.cpp
// 法术卡牌类的实现文件
// 处理法术卡牌的创建、初始化、施放等功能

#include "SpellCard.h"
#include "Manager/GameManager.h"
#include "Animation/AnimationManager.h"
#include "Effect/EffectManager.h"

// 创建法术卡牌的静态工厂方法
// @param id: 卡牌ID
// @param name: 卡牌名称
// @return: 返回创建的法术卡牌指针，创建失败返回nullptr
SpellCard* SpellCard::create(int id, const std::string& name) {
    SpellCard* spell = new (std::nothrow) SpellCard();
    if (spell && spell->init(id, name)) {
        spell->autorelease();  // 加入自动释放池
        return spell;
    }
    CC_SAFE_DELETE(spell);    // 创建失败时安全删除
    return nullptr;
}

// 初始化法术卡牌
// @param id: 卡牌ID
// @param name: 卡牌名称
// @return: 初始化成功返回true，失败返回false
bool SpellCard::init(int id, const std::string& name) {
    // 调用父类初始化
    if (!Card::init(id, name)) {
        return false;
    }

    // 设置卡牌类型为法术
    _cardType = CardType::SPELL;
    // 默认不需要目标
    _targetType = SpellTarget::NONE;
    _needsTarget = false;

    return true;
}

// 初始化法术卡牌的UI元素
void SpellCard::initUI() {
    // 首先初始化基础卡牌UI
    Card::initUI();

    // 添加法术特有的图标
    auto spellIcon = Sprite::create("cards/spell_icon.png");
    spellIcon->setPosition(Vec2(0, 30));
    this->addChild(spellIcon);
}

// 施放法术
// @param target: 法术目标，可以为nullptr（无目标法术）
void SpellCard::castSpell(Card* target) {
    // 检查目标要求
    if (_needsTarget && !target) {
        // 需要目标但没有提供目标，直接返回
        return;
    }

    // 检查目标合法性
    if (_needsTarget && !canTargetCard(target)) {
        // 目标不合法，直接返回
        return;
    }

    // 播放法术施放动画
    playSpellEffect();

    // 触发所有法术效果
    for (auto& effect : _effects) {
        effect->setOwner(this);  // 设置效果的所有者
        effect->onActivate();    // 激活效果
    }

    // 法术施放完成的处理
    onSpellComplete();
}

// 检查是否可以将法术指向目标
// @param target: 要检查的目标
// @return: 如果可以指向返回true，否则返回false
bool SpellCard::canTargetCard(Card* target) const {
    // 空指针检查
    if (!target) return false;

    // 根据法术目标类型判断
    switch (_targetType) {
    case SpellTarget::SINGLE_TARGET:
        return true;  // 单体目标，任何有效目标都可以
    case SpellTarget::FRIENDLY:
        // 检查目标是否为友方随从
        return target->getOwner() == GameManager::getInstance()->getCurrentPlayer();
    case SpellTarget::ENEMY:
        // 检查目标是否为敌方随从
        return target->getOwner() == GameManager::getInstance()->getOpponentPlayer();
    default:
        return false;
    }
}

// 播放法术特效
void SpellCard::playSpellEffect() {
    // 调用动画管理器播放法术动画
    AnimationManager::getInstance()->playSpellAnimation(getName(), nullptr);
}

// 法术施放完成后的处理
void SpellCard::onSpellComplete() {
    // 将法术卡牌移到弃牌堆
    GameManager::getInstance()->discardCard(this);
}

/*
使用示例：

// 创建一个法术卡牌
auto spell = SpellCard::create(1, "火球术");

// 设置法术属性
spell->setTargetType(SpellTarget::SINGLE_TARGET);
spell->setNeedsTarget(true);

// 当玩家选择目标后施放法术
Card* target = getSelectedTarget();
if (target) {
    spell->castSpell(target);
}
*/