// HealEffect.cpp
// 治疗效果的实现文件，包含单体治疗和群体治疗功能

#include "HealEffect.h"
#include "Card/Card.h"
#include "Game/GameManager.h"
#include "Animation/AnimationManager.h"
#include"Player/Player.h"
#include"Card/MinionCard.h"
#include "Game/GameManager.h"
// 构造函数：初始化治疗效果
// @param healing: 治疗量
// @param targetAll: 是否为群体治疗
HealEffect::HealEffect(int healing, bool targetAll)
    : _healing(healing)      // 设置治疗量
    , _targetAll(targetAll)  // 设置是否群体治疗
{
    // 构造函数体为空，使用初始化列表完成初始化
}

// 效果激活时的处理函数
// 根据效果类型（群体/单体）执行相应的治疗逻辑
void HealEffect::onActivate() {
    if (_targetAll) {
        // 如果是群体治疗，调用群体治疗函数
        healAllTargets();
    }
    else {
        // 单体治疗：获取当前选中的目标
        Card* target = GameManager::getInstance()->getSelectedTarget();
        if (target) {
            // 如果有有效目标，进行治疗
            healTarget(target);
        }
    }
}

// 效果停用时的处理函数
// 用于清理效果相关的资源和状态
void HealEffect::onDeactivate() {
    // 当前版本无需清理资源
    // 预留接口以供后续扩展
}

// 检查效果是否可以被激活
// @return: 返回是否可以激活效果
bool HealEffect::canActivate() {
    // 检查效果是否有所属卡牌
    if (!_owner) return false;

    if (_targetAll) {
        // 群体治疗总是可以释放
        // 即使场上没有受伤的随从，也可以释放技能
        return true;
    }
    else {
        // 获取当前选中的目标
        Card* target = GameManager::getInstance()->getSelectedTarget();
        // 检查目标是否存在且生命值未满
        return target && target->getHealth() < target->getMaxHealth();
    }
}

// 对单个目标进行治疗
// @param target: 治疗目标
void HealEffect::healTarget(Card* target) {
    // 空指针检查
    if (!target) return;

    // 播放治疗动画效果
    AnimationManager::getInstance()->playHealAnimation(target, _healing);

    // 检查目标是否为随从卡牌
    if (target->getCardType() == CardType::MINION) {
        // 将目标转换为随从卡牌类型并进行治疗
        // 使用 dynamic_cast 确保类型转换的安全性
        dynamic_cast<MinionCard*>(target)->heal(_healing);
    }
}

// 执行群体治疗效果
// 治疗场上所有随从
void HealEffect::healAllTargets() {
    // 获取游戏管理器实例
    auto gameManager = GameManager::getInstance();

    // 获取双方场上的随从引用
    // 使用引用避免不必要的复制
    auto& playerField = gameManager->getCurrentPlayer()->getField();
    auto& opponentField = gameManager->getOpponentPlayer()->getField();

    // 治疗己方场上所有随从
    for (auto minion : playerField) {
        healTarget(minion);
    }

    // 治疗对方场上所有随从
    for (auto minion : opponentField) {
        healTarget(minion);
    }
}

/* 可能的扩展：
1. 添加治疗目标的过滤条件（如只治疗友方随从）
2. 添加治疗效果的修饰（如过量治疗转化为护甲）
3. 添加治疗效果的触发事件（如治疗后抽牌）
4. 添加治疗量的计算修正（如受伤加深效果）
*/