// EffectManager.cpp
// 效果管理器的实现文件，负责管理和触发游戏中的各种效果
#pragma execution_character_set("utf-8")
#include "EffectManager.h"
#include "Card/Card.h"

// 静态实例指针初始化
EffectManager* EffectManager::_instance = nullptr;

// 获取效果管理器的单例实例
// @return: 返回 EffectManager 的唯一实例
EffectManager* EffectManager::getInstance() {
    if (!_instance) {
        _instance = new EffectManager();  // 懒汉式单例模式
    }
    return _instance;
}

// 构造函数
// 初始化效果处理状态为未处理
EffectManager::EffectManager()
    : _isProcessingEffects(false) {  // 防止效果触发时的递归调用
}

// 注册一个效果到管理器
// @param card: 效果所属的卡牌
// @param effect: 要注册的效果
// @param trigger: 效果的触发类型
void EffectManager::registerEffect(Card* card, std::shared_ptr<IEffect> effect, TriggerType trigger) {
    // 使用 multimap 存储效果，允许多个效果使用相同的触发类型
    _effects.insert({ trigger, {card, effect} });
}

// 注销指定卡牌的所有效果
// @param card: 要注销效果的卡牌
void EffectManager::unregisterEffect(Card* card) {
    // 遍历所有效果并移除属于指定卡牌的效果
    for (auto it = _effects.begin(); it != _effects.end();) {
        if (it->second.first == card) {
            // 如果效果属于指定卡牌，则删除
            it = _effects.erase(it);
        }
        else {
            // 否则继续检查下一个效果
            ++it;
        }
    }
}

// 触发指定类型的所有效果
// @param trigger: 要触发的效果类型
void EffectManager::triggerEffects(TriggerType trigger) {
    // 获取所有具有指定触发类型的效果
    auto range = _effects.equal_range(trigger);

    // 遍历所有匹配的效果
    for (auto it = range.first; it != range.second; ++it) {
        // 检查卡牌是否激活且效果是否可以触发
        if (it->second.first->isActive() && it->second.second->canActivate()) {
            // 将可以触发的效果加入队列
            _effectQueue.push(it->second.second);
        }
    }

    // 如果当前没有在处理效果队列，则开始处理
    if (!_isProcessingEffects) {
        processEffectQueue();
    }
}
void EffectManager::processEffectQueue() {
    if (_isProcessingEffects) {
        return;  // 防止递归调用
    }

    _isProcessingEffects = true;

    // 处理队列中的所有效果
    while (!_effectQueue.empty()) {
        auto effect = _effectQueue.front();
        _effectQueue.pop();

        // 检查效果是否有效并可以激活
        if (effect && effect->canActivate()) {
            try {
                effect->onActivate();
            }
            catch (const std::exception& e) {
                CCLOG("Effect activation failed: %s", e.what());
            }
        }
    }

    _isProcessingEffects = false;
}

void EffectManager::clearEffects() {
    // 清空效果队列
    std::queue<std::shared_ptr<IEffect>> empty;
    std::swap(_effectQueue, empty);

    // 清空效果注册表
    _effects.clear();

    // 重置处理状态
    _isProcessingEffects = false;
}
/*
使用示例：

// 注册效果
auto effect = std::make_shared<SomeEffect>();
EffectManager::getInstance()->registerEffect(card, effect, TriggerType::ON_PLAY);

// 触发效果
EffectManager::getInstance()->triggerEffects(TriggerType::ON_PLAY);

// 注销效果
EffectManager::getInstance()->unregisterEffect(card);
*/

/*
可能的扩展：
1. 添加效果优先级系统
2. 添加效果的条件检查
3. 添加效果的持续时间管理
4. 添加效果的叠加规则
5. 添加效果的互斥检查
*/