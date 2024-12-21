// SpellCard.cpp
// 法术卡牌类的实现文件
// 处理法术卡牌的创建、初始化、施放等功能
#pragma execution_character_set("utf-8")
#include "SpellCard.h"
#include "Manager/GameManager.h"
#include "Animation/AnimationManager.h"
#include "Effect/EffectManager.h"
#include "Utils/GameLogger.h"

// 创建法术卡牌的静态工厂方法
// @param id: 卡牌ID
// @param name: 卡牌名称
// @return: 返回创建的法术卡牌指针，创建失败返回nullptr
SpellCard* SpellCard::create(int id, const std::string& name) {
    SpellCard* spell = new (std::nothrow) SpellCard();
    if (spell && spell->init(id, name)) {
        spell->autorelease();
        return spell;
    }
    CC_SAFE_DELETE(spell);
    return nullptr;
}

// 初始化法术卡牌
// @param id: 卡牌ID
// @param name: 卡牌名称
// @return: 初始化成功返回true，失败返回false
bool SpellCard::init(int id, const std::string& name) {
    if (!Card::init(id, name)) {
        return false;
    }

    // 设置法术牌的默认属性
    _hasBattlecry = false;
    _hasOutcast = false;

    return true;
}

// 初始化法术卡牌的UI元素
//void SpellCard::initUI() {
//    //Card::initUI();
//    updateUI();
//}

void SpellCard::initUI2() {
    auto logger = GameLogger::getInstance();
    logger->log(LogLevel::DEBUG, "Starting SpellCard UI initialization");

    try {
        // 创建并设置卡牌名称标签
        _nameLabel = Label::createWithTTF(_name, "fonts/STKAITI.TTF", 20);
        if (_nameLabel) {
            _nameLabel->setPosition(Vec2(this->getContentSize().width / 2,
                this->getContentSize().height - 55));
            _nameLabel->setTextColor(Color4B::WHITE);
            _nameLabel->enableOutline(Color4B::BLACK, 1);
            this->addChild(_nameLabel, 2);
            logger->log(LogLevel::DEBUG, "Name label created and added");
        }

        // 创建并设置法力值消耗标签
        _costLabel = Label::createWithTTF(std::to_string(_cost), "fonts/arial.ttf", 30);
        if (_costLabel) {
            _costLabel->setPosition(Vec2(-23, this->getContentSize().height + 60));
            _costLabel->setTextColor(Color4B::WHITE);
            _costLabel->enableOutline(Color4B::BLACK, 2);
            _costLabel->enableShadow(Color4B::BLACK);
            this->addChild(_costLabel, 2);
            logger->log(LogLevel::DEBUG, "Cost label created and added");
        }

        // 创建并设置卡牌描述标签
        _descriptionLabel = Label::createWithTTF(_description, "fonts/STKAITI.TTF", 15);
        if (_descriptionLabel) {
            _descriptionLabel->setPosition(Vec2(this->getContentSize().width / 2, -50));
            _descriptionLabel->setDimensions(this->getContentSize().width + 120, 120);
            _descriptionLabel->setTextColor(Color4B::BLACK);
            _descriptionLabel->enableOutline(Color4B::WHITE, 1);
            _descriptionLabel->setAlignment(TextHAlignment::CENTER);
            this->addChild(_descriptionLabel, 2);
            logger->log(LogLevel::DEBUG, "Description label created and added");
        }

        logger->log(LogLevel::DEBUG, "SpellCard UI initialization complete");
    }
    catch (const std::exception& e) {
        logger->log(LogLevel::ERR, "Exception in SpellCard UI initialization: " + std::string(e.what()));
        throw;
    }
}

void SpellCard::updateUI() {
    // 法术牌可能需要更新的UI元素
    if (_costLabel) {
        _costLabel->setString(std::to_string(_cost));
    }
}

//// 施放法术
//// @param target: 法术目标，可以为nullptr（无目标法术）
//void SpellCard::castSpell(Card* target) {
//    // 检查目标要求
//    if (_needsTarget && !target) {
//        // 需要目标但没有提供目标，直接返回
//        return;
//    }
//
//    // 检查目标合法性
//    if (_needsTarget && !canTargetCard(target)) {
//        // 目标不合法，直接返回
//        return;
//    }
//
//    // 播放法术施放动画
//    playSpellEffect();
//
//    // 触发所有法术效果
//    for (auto& effect : _effects) {
//        effect->setOwner(this);  // 设置效果的所有者
//        effect->onActivate();    // 激活效果
//    }
//
//    // 法术施放完成的处理
//    onSpellComplete();
//}
//
//// 检查是否可以将法术指向目标
//// @param target: 要检查的目标
//// @return: 如果可以指向返回true，否则返回false
//bool SpellCard::canTargetCard(Card* target) const {
//    // 空指针检查
//    if (!target) return false;
//
//    // 根据法术目标类型判断
//    switch (_targetType) {
//    case SpellTarget::SINGLE_TARGET:
//        return true;  // 单体目标，任何有效目标都可以
//    case SpellTarget::FRIENDLY:
//        // 检查目标是否为友方随从
//        return target->getOwner() == GameManager::getInstance()->getCurrentPlayer();
//    case SpellTarget::ENEMY:
//        // 检查目标是否为敌方随从
//        return target->getOwner() == GameManager::getInstance()->getOpponentPlayer();
//    default:
//        return false;
//    }
//}
//
//// 播放法术特效
//void SpellCard::playSpellEffect() {
//    // 调用动画管理器播放法术动画
//    AnimationManager::getInstance()->playSpellAnimation(getName(), nullptr);
//}
//
//// 法术施放完成后的处理
//void SpellCard::onSpellComplete() {
//    // 将法术卡牌移到弃牌堆
//    GameManager::getInstance()->discardCard(this);
//}

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