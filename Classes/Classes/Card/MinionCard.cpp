// MinionCard.cpp

// 包含必要的头文件
#include "Card.h"              // 基础卡牌类
#include "MinionCard.h"        // 随从卡牌类
#include "Game/TurnSystem.h"   // 回合系统
#include "Effect/EffectManager.h"    // 效果管理器
#include "Network/NetworkManager.h"   // 网络管理器
#include "Animation/AnimationManager.h" // 动画管理器

// 初始化随从卡牌
bool MinionCard::init(int id, const std::string& name) {
    // 调用父类的初始化方法，如果失败则返回false
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

    // TODO: 这些默认值应该从配置文件或数据库中读取
    // 后续需要实现配置系统来加载具体的卡牌数值

    return true;  // 初始化成功
}

// 打出随从卡牌的方法
void MinionCard::playCard() {
    // 检查玩家是否有足够的法力值来打出这张卡
    if (!TurnSystem::getInstance()->useMana(_cost)) {
        return;  // 如果法力值不足，直接返回
    }

    // 播放打出卡牌时的动画效果
    AnimationManager::getInstance()->playCardAnimation(this);

    // 触发卡牌的战吼效果（如果有的话）
    EffectManager::getInstance()->triggerEffects(TriggerType::BATTLECRY);

    // 创建一个游戏动作并发送到网络
    GameAction playAction;     // 创建一个新的游戏动作对象
    playAction.type = ActionType::PLAY_CARD;  // 设置动作类型为打出卡牌
    playAction.sourceId = getId();  // 设置被打出卡牌的ID
    playAction.targetId = 0;      // 设置目标ID为0（表示无特定目标）
    // playAction.extraData 保持为空（无额外数据需要传递）

    // 通过网络管理器发送这个动作到其他玩家
    NetworkManager::getInstance()->sendGameAction(playAction);
}