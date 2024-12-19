#pragma execution_character_set("utf-8")
#include "AnimationManager.h"
#include "Card/Card.h"
#include"Audio/AudioManager.h"
AnimationManager* AnimationManager::_instance = nullptr;

AnimationManager* AnimationManager::getInstance() {
    if (!_instance) {
        _instance = new AnimationManager();
    }
    return _instance;
}
AnimationManager::AnimationManager()
    : _isAnimating(false)
    , _currentAnimations() {
}
void AnimationManager::playCardDrawAnimation(Card* card) {
    if (!card) return;

    _isAnimating = true;

    // 创建卡牌抽取动画序列
    auto moveUp = MoveBy::create(0.2f, Vec2(0, 30));
    auto moveDown = MoveBy::create(0.1f, Vec2(0, -30));
    auto scale = ScaleTo::create(0.2f, 1.2f);
    auto scaleBack = ScaleTo::create(0.1f, 1.0f);

    // 添加发光效果
    auto glow = Sprite::create("effects/card_glow.png");
    if (glow) {
        card->addChild(glow, -1);
        glow->setOpacity(0);
        auto fadeIn = FadeIn::create(0.2f);
        auto fadeOut = FadeOut::create(0.2f);
        glow->runAction(Sequence::create(fadeIn, fadeOut, RemoveSelf::create(), nullptr));
    }

    // 运行动画序列
    auto sequence = Sequence::create(
        Spawn::create(moveUp, scale, nullptr),
        Spawn::create(moveDown, scaleBack, nullptr),
        CallFunc::create([this, card]() {
            this->onAnimationComplete(card);
            }),
        nullptr
    );

    card->runAction(sequence);
}
void AnimationManager::playAttackAnimation(Card* attacker, Card* target) {
    if (!attacker || !target) return;

    _isAnimating = true;

    // 保存原始位置
    Vec2 originalPos = attacker->getPosition();
    Vec2 targetPos = target->getPosition();
    // 创建攻击动画序列:
// 1. 移动到目标位置 (0.3秒)
// 2. 在目标位置震动
// 3. 返回原始位置 (0.2秒)
    // 创建攻击动画序列
    auto moveToTarget = MoveTo::create(0.3f, targetPos);
    auto moveBack = MoveTo::create(0.2f, originalPos);
    auto shake = Sequence::create(
        MoveBy::create(0.05f, Vec2(5, 0)),
        MoveBy::create(0.05f, Vec2(-5, 0)),
        nullptr
    );

    // 创建完整的动画序列
    auto sequence = Sequence::create(
        moveToTarget,
        shake,
        moveBack,
        CallFunc::create([this, attacker]() {
            this->onAnimationComplete(attacker);
            }),
        nullptr
    );

    attacker->runAction(sequence);

    // 播放攻击特效
    playParticleEffect("attack_effect", targetPos);
}
void AnimationManager::playDamageAnimation(Card* target, int damage) {
    if (!target) return;

    _isAnimating = true;

    // 创建伤害数字标签 // 创建伤害数字标签,包含淡出和向上飘动效果
    auto damageLabel = Label::createWithTTF("-" + std::to_string(damage), "fonts/arial.ttf", 32);
    damageLabel->setColor(Color3B::RED);
    damageLabel->setPosition(target->getPosition() + Vec2(0, 50));
    target->getParent()->addChild(damageLabel);

    // 目标卡牌的震动效果
    auto shake = Sequence::create(
        MoveBy::create(0.05f, Vec2(5, 0)),   // 向右
        MoveBy::create(0.05f, Vec2(-5, 0)),  // 向左
        MoveBy::create(0.05f, Vec2(5, 0)),   // 向右
        MoveBy::create(0.05f, Vec2(-5, 0)),  // 向左
        nullptr
    );

    // 伤害数字的动画效果
    auto fadeIn = FadeIn::create(0.1f);
    auto moveUp = MoveBy::create(0.5f, Vec2(0, 30));
    auto fadeOut = FadeOut::create(0.2f);

    // 运行目标卡牌的震动动画
    target->runAction(Sequence::create(
        shake,
        CallFunc::create([this, target]() {
            this->onAnimationComplete(target);
            }),
        nullptr
    ));

    // 运行伤害数字的动画
    damageLabel->runAction(Sequence::create(
        fadeIn,
        Spawn::create(moveUp, fadeOut, nullptr),
        RemoveSelf::create(),
        nullptr
    ));

    // 播放伤害特效
    playParticleEffect("damage_effect", target->getPosition());
}
void AnimationManager::playCardAnimation(Card* card) {
    if (!card) return;

    _isAnimating = true;

    // 卡牌放大效果
    auto scale = ScaleTo::create(0.2f, 1.2f);
    auto scaleBack = ScaleTo::create(0.1f, 1.0f);

    // 发光效果
    auto glow = Sprite::create("effects/card_glow.png");
    card->addChild(glow, -1);
    glow->setOpacity(0);
    auto fadeIn = FadeIn::create(0.2f);
    auto fadeOut = FadeOut::create(0.2f);

    auto sequence = Sequence::create(
        scale,
        scaleBack,
        CallFunc::create([this, card]() {
            this->onAnimationComplete(card);
            }),
        nullptr
    );

    card->runAction(sequence);
    glow->runAction(Sequence::create(fadeIn, fadeOut, RemoveSelf::create(), nullptr));
}
void AnimationManager::deathAnimation(Card* card) {
    if (!card) return;

    _isAnimating = true;
    // 创建消失动画：同时进行淡出和缩小
    auto fadeOut = cocos2d::FadeOut::create(0.5f);     // 0.5秒内淡出
    auto scale = cocos2d::ScaleTo::create(0.5f, 0.0f); // 0.5秒内缩小到0
    auto spawn = cocos2d::Spawn::create(fadeOut, scale, nullptr);

    // 添加完成回调，在动画结束后移除卡牌
    auto sequence = cocos2d::Sequence::create(
        spawn,
        cocos2d::CallFunc::create([this, card]() {
            card->removeFromParent();
            this->onAnimationComplete(card);
            }),
        nullptr
    );
    card->runAction(sequence);
    _currentAnimations.push_back(sequence);
}

void AnimationManager::playParticleEffect(const std::string& effectName, const cocos2d::Vec2& position) {
    // 从plist文件创建粒子系统
    auto particleSystem = cocos2d::ParticleSystemQuad::create("particles/" + effectName + ".plist");
    if (particleSystem) {
        particleSystem->setPosition(position);
        cocos2d::Director::getInstance()->getRunningScene()->addChild(particleSystem);
        particleSystem->setAutoRemoveOnFinish(true);  // 特效完成后自动移除
    }
}

void AnimationManager::playSpellAnimation(const std::string& spellName, Card* target) {
    if (!target) return;

    _isAnimating = true;

    // 创建法术特效精灵
    auto spellSprite = cocos2d::Sprite::create("spells/" + spellName + ".png");
    if (!spellSprite) return;

    spellSprite->setPosition(target->getPosition());
    target->getParent()->addChild(spellSprite);

    // 创建法术动画序列：放大(0.3s) -> 恢复大小(0.3s) -> 淡出(0.2s)
    auto animation = cocos2d::Sequence::create(
        cocos2d::ScaleTo::create(0.3f, 1.2f),
        cocos2d::ScaleTo::create(0.3f, 1.0f),
        cocos2d::FadeOut::create(0.2f),
        cocos2d::CallFunc::create([spellSprite]() {
            spellSprite->removeFromParent();
            }),
        nullptr
    );

    spellSprite->runAction(animation);
    _currentAnimations.push_back(animation);
}


bool AnimationManager::isAnimating() const {
    // 检查是否有动画正在播放
    return !_currentAnimations.empty() && _isAnimating;
}

void AnimationManager::onAnimationComplete(cocos2d::Node* target) {
    // 使用remove_if算法清理已完成的动画
    _currentAnimations.erase(
        std::remove_if(_currentAnimations.begin(), _currentAnimations.end(),
            [](cocos2d::Action* action) {
                return action->isDone();
            }
        ),
        _currentAnimations.end()
    );

    // 更新动画管理器状态
    if (_currentAnimations.empty()) {
        _isAnimating = false;
    }
}
void AnimationManager::playHealAnimation(Card* target, int amount) {
    if (!target) return;

    // 创建发光效果
    auto glow = Sprite::create("effects/heal_glow.png");
    target->addChild(glow, 1);
    glow->setScale(0.0f);
    glow->setOpacity(0);

    // 创建数字标签
    auto healLabel = Label::createWithTTF("+" + std::to_string(amount), "fonts/arial.ttf", 24);
    healLabel->setColor(Color3B::GREEN);
    healLabel->setPosition(Vec2(target->getContentSize().width / 2, target->getContentSize().height));
    target->addChild(healLabel, 2);

    // 发光动画
    auto glowIn = Spawn::create(
        ScaleTo::create(0.3f, 1.0f),
        FadeIn::create(0.3f),
        nullptr
    );
    auto glowOut = Spawn::create(
        ScaleTo::create(0.3f, 0.5f),
        FadeOut::create(0.3f),
        nullptr
    );
    auto glowSeq = Sequence::create(
        glowIn,
        glowOut,
        RemoveSelf::create(),
        nullptr
    );
    glow->runAction(glowSeq);

    // 数字上升动画
    auto moveUp = MoveBy::create(1.0f, Vec2(0, 50));
    auto fadeOut = FadeOut::create(1.0f);
    auto labelSeq = Sequence::create(
        Spawn::create(moveUp, fadeOut, nullptr),
        RemoveSelf::create(),
        nullptr
    );
    healLabel->runAction(labelSeq);

    // 播放音效
    AudioManager::play2d("sounds/heal.mp3");
}
void AnimationManager::pauseAllAnimations() {
    for (auto action : _currentAnimations) {
        if (action && action->getTarget()) {
            action->getTarget()->pause();
        }
    }
    _isAnimating = false;
}

void AnimationManager::resumeAllAnimations() {
    for (auto action : _currentAnimations) {
        if (action && action->getTarget()) {
            action->getTarget()->resume();
        }
    }
    _isAnimating = true;
}