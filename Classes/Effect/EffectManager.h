#ifndef __EFFECT_MANAGER_H__
#define __EFFECT_MANAGER_H__

#include "cocos2d.h"
#include "IEffect.h"
#include <map>
#include <queue>
#include <memory>

class Card;

class EffectManager {
public:
    static EffectManager* getInstance();
    void triggerEffects(TriggerType type);
    // 效果注册和触发
    void registerEffect(Card* card, std::shared_ptr<IEffect> effect, TriggerType trigger);
    void unregisterEffect(Card* card);

    // 效果队列管理
    void processEffectQueue();
    void clearEffects();

private:
    EffectManager();
    static EffectManager* _instance;

    // 效果存储
    std::multimap<TriggerType, std::pair<Card*, std::shared_ptr<IEffect>>> _effects;
    std::queue<std::shared_ptr<IEffect>> _effectQueue;

    bool _isProcessingEffects;
};

#endif // __EFFECT_MANAGER_H__