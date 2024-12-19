#ifndef __ANIMATION_MANAGER_H__
#define __ANIMATION_MANAGER_H__
#pragma execution_character_set("utf-8")
#include "cocos2d.h"

class Card;

class AnimationManager {
public:
    static AnimationManager* getInstance();

    // 卡牌动画
    void playCardAnimation(Card* card);
    void playAttackAnimation(Card* attacker, Card* target);  // 改为 playAttackAnimation
    void deathAnimation(Card* card);// 卡牌死亡动画

    // 特效动画
    void playDamageAnimation(Card* target, int damage);  // 添加伤害动画方法
    void playHealAnimation(Card* target, int healing);  // 添加治疗动画方法
    void playParticleEffect(const std::string& effectName, const cocos2d::Vec2& position);
    void playSpellAnimation(const std::string& spellName, Card* target);
    void playCardDrawAnimation(Card* card);
    // 动画控制
    void pauseAllAnimations();// 暂停所有动画
    void resumeAllAnimations();// 恢复所有动画
    bool isAnimating() const;// 检查是否有动画正在播放

private:
    AnimationManager();
    static AnimationManager* _instance;

    bool _isAnimating;
    std::vector<cocos2d::Action*> _currentAnimations;

    void onAnimationComplete(cocos2d::Node* target);// 动画完成回调
};

#endif // __ANIMATION_MANAGER_H__