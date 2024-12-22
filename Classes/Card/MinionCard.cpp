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
#include "Manager/GameManager.h" 
#include "Scene/GameScene.h"    

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
    //initUI();

    return true;  // 初始化成功
}

// 初始化随从卡牌的UI组件
void MinionCard::initUI() {
    //Card::initUI();  // 调用父类的UI初始化

    
    updateUI();
}
void MinionCard::initUI2()
{
    auto logger = GameLogger::getInstance();
    logger->log(LogLevel::DEBUG, "Starting Card UI initialization");

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
        else {
            logger->log(LogLevel::WARNING, "Failed to create name label");
        }

        // 创建并设置法力值消耗标签
        _costLabel = Label::createWithTTF(std::to_string(_cost), "fonts/arial.ttf", 30);//!!！！！！！！！！！！！！！！！！!!!!!!!!!!!
        if (_costLabel) {
            _costLabel->setPosition(Vec2(-23, this->getContentSize().height + 60));
            _costLabel->setTextColor(Color4B::WHITE);  // 改为白色
            _costLabel->enableOutline(Color4B::BLACK, 2);  // 保持黑色描边
            _costLabel->enableShadow(Color4B::BLACK);  // 保持阴影
            this->addChild(_costLabel, 2);
            logger->log(LogLevel::DEBUG, "Cost label created and added");
        }
        else {
            logger->log(LogLevel::WARNING, "Failed to create cost label");
        }

        // 创建并设置卡牌描述标签                                                                         待修改！！！！！！！
        _descriptionLabel = Label::createWithTTF(_description, "fonts/STKAITI.TTF", 15);
        if (_descriptionLabel) {
            _descriptionLabel->setPosition(Vec2(this->getContentSize().width / 2, -50));
            _descriptionLabel->setDimensions(this->getContentSize().width + 120, 120);//文本框大小
            _descriptionLabel->setTextColor(Color4B::BLACK);  // 改为黑色
            _descriptionLabel->enableOutline(Color4B::WHITE, 1);  // 改为白色描边
            _descriptionLabel->setAlignment(TextHAlignment::CENTER);
            this->addChild(_descriptionLabel, 2);
            logger->log(LogLevel::DEBUG, "Description label created and added");
        }
        else {
            logger->log(LogLevel::WARNING, "Failed to create description label");
        }

       ///攻击力
            auto attackLabel = Label::createWithTTF(std::to_string(this->getAttack()), "fonts/arial.ttf", 40);
            attackLabel->setPosition(Vec2(this->getContentSize().width / 2-60, -55));
            attackLabel->setTextColor(Color4B::WHITE);  // 改为白色
            attackLabel->enableOutline(Color4B::BLACK, 2);
            attackLabel->enableShadow(Color4B::BLACK);
            this->addChild(attackLabel, 2);

            ////// 生命值标签
            auto healthLabel = Label::createWithTTF(std::to_string(this->getHealth()), "fonts/arial.ttf", 40);
            healthLabel->setPosition(Vec2(this->getContentSize().width /2+80, -55));
            healthLabel->setTextColor(Color4B::WHITE);  // 改为白色
            healthLabel->enableOutline(Color4B::BLACK, 2);
            healthLabel->enableShadow(Color4B::BLACK);
            this->addChild(healthLabel, 2);
        

        logger->log(LogLevel::DEBUG, "Card UI initialization complete");
    }
    catch (const std::exception& e) {
        logger->log(LogLevel::ERR, "Exception in Card UI initialization: " + std::string(e.what()));
        throw;
    }
}

// 更新随从卡牌的UI显示
void MinionCard::updateUI() {
    initUI2();
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
   
    
    _health -= amount;
    if (_health < 0) _health = 0;
    
    updateUI();
    checkDeath(this);
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
bool MinionCard::canAttackTarget(MinionCard* target) const {
    if (!target || !_canAttack) return false;
    
    // 检查是否被冻结或其他状态影响
    if (_isFrozen) return false;
    
    // 如果目标有嘲讽，只能攻击嘲讽目标
    if (target->getHasProvoke()) return true;
    
    return true;
}

// 执行攻击目标的动作
void MinionCard::attackTarget(MinionCard* target) {
    auto logger = GameLogger::getInstance();
    logger->log(LogLevel::DEBUG, "开始攻击 - 攻击者: " + this->getName() + 
        " 目标: " + target->getName());
    
    // 执行攻击
    target->takeDamage(this->getAttack());
    this->takeDamage(target->getAttack());
    
    logger->log(LogLevel::DEBUG, "攻击后状态 - 攻击者血量: " + std::to_string(this->getHealth()) + 
        " 目标血量: " + std::to_string(target->getHealth()));
    
    checkDeath(target);
    checkDeath(this);

    _canAttack = false;
}

// 检查随从是否死亡
void MinionCard::checkDeath(MinionCard* card) {
    auto logger = GameLogger::getInstance();
    
    if (card->getHealth() <= 0) {
        logger->log(LogLevel::DEBUG, "卡牌死亡检查 - Name: " + card->getName() + 
            " Health: " + std::to_string(card->getHealth()));
        
        card->_isDead = true;
        logger->log(LogLevel::DEBUG, "设置isDead变量变成true，卡牌: " + card->getName() + 
            " isDead: " + std::to_string(card->_isDead));
        
        card->setPosition(Vec2(20000, 20000));
        logger->log(LogLevel::DEBUG, "将卡牌移出屏幕: " + card->getName());

        //亡语效果
        if (card->getDeathrattle(card))
        {
            // 获取当前场景
            auto scene = dynamic_cast<GameScene*>(Director::getInstance()->getRunningScene());
            if (scene) {
                // 通过场景访问对手场上的卡牌
                if (!scene->_opponentFieldCards2.empty()) {
                    for (auto& opponentCard : scene->_opponentFieldCards2) {
                        if (opponentCard) {
                            opponentCard->setAttack(opponentCard->getAttack() + 2);
                            opponentCard->setHealth(opponentCard->getHealth() + 3);
                            opponentCard->initUI2();
                        }
                    }
                }
            }
        }

    } else {
        logger->log(LogLevel::DEBUG, "卡牌存活 - Name: " + card->getName() + 
            " Health: " + std::to_string(card->getHealth()));
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
    this->setPosition(Vec2(20000, 20000));
}

// 更新随从的状态
void MinionCard::updateStats() {
    updateUI();
}