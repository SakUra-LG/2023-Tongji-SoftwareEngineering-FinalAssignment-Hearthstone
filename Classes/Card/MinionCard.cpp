// MinionCard.cpp
// 随从卡牌类的实现文件
#pragma execution_character_set("utf-8")
// 包含所需的头文件
#include "Card.h"                      // 卡牌基类
#include "MinionCard.h"                // 随从卡牌类
#include "Game/TurnSystem.h"           // 回合系统
#include "Effect/EffectManager.h"      // 效果管理器
#include "Network/NetworkManager.h"     // 网络管理器
#include "Animation/AnimationManager.h" // 动画管理器

// 创建随从卡牌的静态工厂方法
// @param id: 卡牌ID
// @param name: 卡牌名称
// @return: 返回创建的随从卡牌指针，失败则返回nullptr
MinionCard* MinionCard::create(int id, const std::string& name) {
    MinionCard* minion = new (std::nothrow) MinionCard();
    if (minion && minion->init(id, name)) {
        minion->autorelease();
        return minion;
    }
    CC_SAFE_DELETE(minion);
    return nullptr;
}

// 初始化随从卡牌
// @param id: 卡牌ID
// @param name: 卡牌名称
// @return: 初始化成功返回true，失败返回false
bool MinionCard::init(int id, const std::string& name) {
    // 调用父类的初始化，如果失败则返回false
    if (!Card::init(id, name)) {
        return false;
    }

    // 设置随从的默认属性
    _attack = 1;              // 设置初始攻击力为1
    _health = 1;              // 设置初始生命值为1
    _maxHealth = 1;           // 设置最大生命值为1
    _canAttack = false;       // 设置初始状态不能攻击（通常需要等待一回合）
    _hasTaunt = false;        // 设置初始状态无嘲讽效果
    _hasDivineShield = false; // 设置初始状态无圣盾效果

    // 初始化UI组件
    initUI();

    return true;  // 初始化成功
}

// 初始化随从卡牌的UI组件
void MinionCard::initUI() {
    Card::initUI();  // 调用父类的UI初始化

    // 添加攻击力和生命值标签
    _attackLabel = Label::createWithTTF("", "fonts/arial.ttf", 32);
    _attackLabel->setPosition(Vec2(-80, -80));
    this->addChild(_attackLabel);

    _healthLabel = Label::createWithTTF("", "fonts/arial.ttf", 32);
    _healthLabel->setPosition(Vec2(80, -80));
    this->addChild(_healthLabel);

    updateUI();
}

// 更新随从卡牌的UI显示
void MinionCard::updateUI() {
    _attackLabel->setString(std::to_string(_attack));
    _healthLabel->setString(std::to_string(_health));
}

// 打出随从卡牌时的处理
void MinionCard::playCard() {
    // 检查是否有足够的法力值来打出卡牌
    if (!TurnSystem::getInstance()->useMana(_cost)) {
        return;  // 法力值不足，直接返回
    }

    // 播放打出卡牌时的动画效果
    AnimationManager::getInstance()->playCardAnimation(this);

    // 触发随从卡牌的战吼效果和其他触发效果
    EffectManager::getInstance()->triggerEffects(TriggerType::BATTLECRY);

    // 创建一个游戏动作并发送到网络
    GameAction playAction;     // 创建一个新的游戏动作对象
    playAction.type = ActionType::PLAY_CARD;  // 设置动作类型为打出卡牌
    playAction.sourceId = getId();  // 设置来源卡牌的ID
    playAction.targetId = 0;      // 设置目标ID为0（表示无特定目标）
    // playAction.extraData 保持为空，无额外数据需要传递

    // 通过网络管理器发送游戏动作
    NetworkManager::getInstance()->sendGameAction(playAction);
}

// 处理随从受到伤害
void MinionCard::takeDamage(int amount) {
    if (_hasDivineShield) {
        _hasDivineShield = false;  // 圣盾抵消伤害
        return;
    }
    
    _health -= amount;
    if (_health < 0) _health = 0;
    
    updateUI();
    checkDeath();
}

// 处理随从受到治疗
void MinionCard::heal(int amount) {
    _health = std::min(_health + amount, _maxHealth);
    updateUI();
}

// 为随从添加增益效果
void MinionCard::addBuff(int attack, int health) {
    _attack += attack;
    _maxHealth += health;
    _health += health;
    updateUI();
}

// 沉默随从，移除所有效果
void MinionCard::silence() {
    // 移除所有效果
    _effects.clear();
    
    // 重置所有状态
    _hasTaunt = false;
    _hasDivineShield = false;
    
    // 更新UI
    updateUI();
}

// 检查是否可以攻击指定目标
bool MinionCard::canAttackTarget(Card* target) const {
    if (!target || !_canAttack) return false;
    
    // 检查是否被冻结或其他状态影响
    if (_isFrozen) return false;
    
    // 如果目标有嘲讽，只能攻击嘲讽目标
    if (target->getHasProvoke()) return true;
    
    return true;
}

// 执行攻击目标的动作
void MinionCard::attackTarget(Card* target) {
    if (!canAttackTarget(target)) return;
    
    // 执行攻击
    target->takeDamage(_attack);
    if (target->getCardType() == CardType::MINION) {
        this->takeDamage(target->getAttack());
    }
    
    _canAttack = false;  // 攻击后不能再次攻击
}

// 检查随从是否死亡
void MinionCard::checkDeath() {
    if (_health <= 0) {
        onDeathrattle();
        // TODO: 处理随从死亡的逻辑
    }
}

// 触发战吼效果
void MinionCard::onBattlecry() {
    // 触发战吼效果
    for (auto& effect : _effects) {
        if (effect->getTriggerType() == TriggerType::BATTLECRY) {
            effect->onActivate();
        }
    }
}

// 触发亡语效果
void MinionCard::onDeathrattle() {
    // 触发亡语效果
    for (auto& effect : _effects) {
        if (effect->getTriggerType() == TriggerType::DEATHRATTLE) {
            effect->onActivate();
        }
    }
}

// 更新随从的状态
void MinionCard::updateStats() {
    updateUI();
}