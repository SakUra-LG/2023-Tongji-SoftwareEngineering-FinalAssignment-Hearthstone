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

    // �������Ƴ�ȡ��������
    auto moveUp = MoveBy::create(0.2f, Vec2(0, 30));
    auto moveDown = MoveBy::create(0.1f, Vec2(0, -30));
    auto scale = ScaleTo::create(0.2f, 1.2f);
    auto scaleBack = ScaleTo::create(0.1f, 1.0f);

    // ��ӷ���Ч��
    auto glow = Sprite::create("effects/card_glow.png");
    if (glow) {
        card->addChild(glow, -1);
        glow->setOpacity(0);
        auto fadeIn = FadeIn::create(0.2f);
        auto fadeOut = FadeOut::create(0.2f);
        glow->runAction(Sequence::create(fadeIn, fadeOut, RemoveSelf::create(), nullptr));
    }

    // ���ж�������
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

    // ����ԭʼλ��
    Vec2 originalPos = attacker->getPosition();
    Vec2 targetPos = target->getPosition();
    // ����������������:
// 1. �ƶ���Ŀ��λ�� (0.3��)
// 2. ��Ŀ��λ����
// 3. ����ԭʼλ�� (0.2��)
    // ����������������
    auto moveToTarget = MoveTo::create(0.3f, targetPos);
    auto moveBack = MoveTo::create(0.2f, originalPos);
    auto shake = Sequence::create(
        MoveBy::create(0.05f, Vec2(5, 0)),
        MoveBy::create(0.05f, Vec2(-5, 0)),
        nullptr
    );

    // ���������Ķ�������
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

    // ���Ź�����Ч
    playParticleEffect("attack_effect", targetPos);
}
void AnimationManager::playDamageAnimation(Card* target, int damage) {
    if (!target) return;

    _isAnimating = true;

    // �����˺����ֱ�ǩ // �����˺����ֱ�ǩ,��������������Ʈ��Ч��
    auto damageLabel = Label::createWithTTF("-" + std::to_string(damage), "fonts/arial.ttf", 32);
    damageLabel->setColor(Color3B::RED);
    damageLabel->setPosition(target->getPosition() + Vec2(0, 50));
    target->getParent()->addChild(damageLabel);

    // Ŀ�꿨�Ƶ���Ч��
    auto shake = Sequence::create(
        MoveBy::create(0.05f, Vec2(5, 0)),   // ����
        MoveBy::create(0.05f, Vec2(-5, 0)),  // ����
        MoveBy::create(0.05f, Vec2(5, 0)),   // ����
        MoveBy::create(0.05f, Vec2(-5, 0)),  // ����
        nullptr
    );

    // �˺����ֵĶ���Ч��
    auto fadeIn = FadeIn::create(0.1f);
    auto moveUp = MoveBy::create(0.5f, Vec2(0, 30));
    auto fadeOut = FadeOut::create(0.2f);

    // ����Ŀ�꿨�Ƶ��𶯶���
    target->runAction(Sequence::create(
        shake,
        CallFunc::create([this, target]() {
            this->onAnimationComplete(target);
            }),
        nullptr
    ));

    // �����˺����ֵĶ���
    damageLabel->runAction(Sequence::create(
        fadeIn,
        Spawn::create(moveUp, fadeOut, nullptr),
        RemoveSelf::create(),
        nullptr
    ));

    // �����˺���Ч
    playParticleEffect("damage_effect", target->getPosition());
}
void AnimationManager::playCardAnimation(Card* card) {
    if (!card) return;

    _isAnimating = true;

    // ���ƷŴ�Ч��
    auto scale = ScaleTo::create(0.2f, 1.2f);
    auto scaleBack = ScaleTo::create(0.1f, 1.0f);

    // ����Ч��
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
    // ������ʧ������ͬʱ���е�������С
    auto fadeOut = cocos2d::FadeOut::create(0.5f);     // 0.5���ڵ���
    auto scale = cocos2d::ScaleTo::create(0.5f, 0.0f); // 0.5������С��0
    auto spawn = cocos2d::Spawn::create(fadeOut, scale, nullptr);

    // �����ɻص����ڶ����������Ƴ�����
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
    // ��plist�ļ���������ϵͳ
    auto particleSystem = cocos2d::ParticleSystemQuad::create("particles/" + effectName + ".plist");
    if (particleSystem) {
        particleSystem->setPosition(position);
        cocos2d::Director::getInstance()->getRunningScene()->addChild(particleSystem);
        particleSystem->setAutoRemoveOnFinish(true);  // ��Ч��ɺ��Զ��Ƴ�
    }
}

void AnimationManager::playSpellAnimation(const std::string& spellName, Card* target) {
    if (!target) return;

    _isAnimating = true;

    // ����������Ч����
    auto spellSprite = cocos2d::Sprite::create("spells/" + spellName + ".png");
    if (!spellSprite) return;

    spellSprite->setPosition(target->getPosition());
    target->getParent()->addChild(spellSprite);

    // ���������������У��Ŵ�(0.3s) -> �ָ���С(0.3s) -> ����(0.2s)
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
    // ����Ƿ��ж������ڲ���
    return !_currentAnimations.empty() && _isAnimating;
}

void AnimationManager::onAnimationComplete(cocos2d::Node* target) {
    // ʹ��remove_if�㷨��������ɵĶ���
    _currentAnimations.erase(
        std::remove_if(_currentAnimations.begin(), _currentAnimations.end(),
            [](cocos2d::Action* action) {
                return action->isDone();
            }
        ),
        _currentAnimations.end()
    );

    // ���¶���������״̬
    if (_currentAnimations.empty()) {
        _isAnimating = false;
    }
}
void AnimationManager::playHealAnimation(Card* target, int amount) {
    if (!target) return;

    // ��������Ч��
    auto glow = Sprite::create("effects/heal_glow.png");
    target->addChild(glow, 1);
    glow->setScale(0.0f);
    glow->setOpacity(0);

    // �������ֱ�ǩ
    auto healLabel = Label::createWithTTF("+" + std::to_string(amount), "fonts/arial.ttf", 24);
    healLabel->setColor(Color3B::GREEN);
    healLabel->setPosition(Vec2(target->getContentSize().width / 2, target->getContentSize().height));
    target->addChild(healLabel, 2);

    // ���⶯��
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

    // ������������
    auto moveUp = MoveBy::create(1.0f, Vec2(0, 50));
    auto fadeOut = FadeOut::create(1.0f);
    auto labelSeq = Sequence::create(
        Spawn::create(moveUp, fadeOut, nullptr),
        RemoveSelf::create(),
        nullptr
    );
    healLabel->runAction(labelSeq);

    // ������Ч
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