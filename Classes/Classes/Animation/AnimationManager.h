#ifndef __ANIMATION_MANAGER_H__
#define __ANIMATION_MANAGER_H__

#include "cocos2d.h"

class Card;

class AnimationManager {
public:
    static AnimationManager* getInstance();

    // ���ƶ���
    void playCardAnimation(Card* card);
    void playAttackAnimation(Card* attacker, Card* target);  // ��Ϊ playAttackAnimation
    void deathAnimation(Card* card);// ������������

    // ��Ч����
    void playDamageAnimation(Card* target, int damage);  // ����˺���������
    void playHealAnimation(Card* target, int healing);  // ������ƶ�������
    void playParticleEffect(const std::string& effectName, const cocos2d::Vec2& position);
    void playSpellAnimation(const std::string& spellName, Card* target);
    void playCardDrawAnimation(Card* card);
    // ��������
    void pauseAllAnimations();// ��ͣ���ж���
    void resumeAllAnimations();// �ָ����ж���
    bool isAnimating() const;// ����Ƿ��ж������ڲ���

private:
    AnimationManager();
    static AnimationManager* _instance;

    bool _isAnimating;
    std::vector<cocos2d::Action*> _currentAnimations;

    void onAnimationComplete(cocos2d::Node* target);// ������ɻص�
};

#endif // __ANIMATION_MANAGER_H__