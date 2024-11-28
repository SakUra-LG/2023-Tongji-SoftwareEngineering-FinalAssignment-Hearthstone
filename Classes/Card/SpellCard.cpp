// SpellCard.cpp
// �����������ʵ���ļ�
// ���������ƵĴ�������ʼ����ʩ�ŵȹ���

#include "SpellCard.h"
#include "Manager/GameManager.h"
#include "Animation/AnimationManager.h"
#include "Effect/EffectManager.h"

// �����������Ƶľ�̬��������
// @param id: ����ID
// @param name: ��������
// @return: ���ش����ķ�������ָ�룬����ʧ�ܷ���nullptr
SpellCard* SpellCard::create(int id, const std::string& name) {
    SpellCard* spell = new (std::nothrow) SpellCard();
    if (spell && spell->init(id, name)) {
        spell->autorelease();  // �����Զ��ͷų�
        return spell;
    }
    CC_SAFE_DELETE(spell);    // ����ʧ��ʱ��ȫɾ��
    return nullptr;
}

// ��ʼ����������
// @param id: ����ID
// @param name: ��������
// @return: ��ʼ���ɹ�����true��ʧ�ܷ���false
bool SpellCard::init(int id, const std::string& name) {
    // ���ø����ʼ��
    if (!Card::init(id, name)) {
        return false;
    }

    // ���ÿ�������Ϊ����
    _cardType = CardType::SPELL;
    // Ĭ�ϲ���ҪĿ��
    _targetType = SpellTarget::NONE;
    _needsTarget = false;

    return true;
}

// ��ʼ���������Ƶ�UIԪ��
void SpellCard::initUI() {
    // ���ȳ�ʼ����������UI
    Card::initUI();

    // ��ӷ������е�ͼ��
    auto spellIcon = Sprite::create("cards/spell_icon.png");
    spellIcon->setPosition(Vec2(0, 30));
    this->addChild(spellIcon);
}

// ʩ�ŷ���
// @param target: ����Ŀ�꣬����Ϊnullptr����Ŀ�귨����
void SpellCard::castSpell(Card* target) {
    // ���Ŀ��Ҫ��
    if (_needsTarget && !target) {
        // ��ҪĿ�굫û���ṩĿ�ֱ꣬�ӷ���
        return;
    }

    // ���Ŀ��Ϸ���
    if (_needsTarget && !canTargetCard(target)) {
        // Ŀ�겻�Ϸ���ֱ�ӷ���
        return;
    }

    // ���ŷ���ʩ�Ŷ���
    playSpellEffect();

    // �������з���Ч��
    for (auto& effect : _effects) {
        effect->setOwner(this);  // ����Ч����������
        effect->onActivate();    // ����Ч��
    }

    // ����ʩ����ɵĴ���
    onSpellComplete();
}

// ����Ƿ���Խ�����ָ��Ŀ��
// @param target: Ҫ����Ŀ��
// @return: �������ָ�򷵻�true�����򷵻�false
bool SpellCard::canTargetCard(Card* target) const {
    // ��ָ����
    if (!target) return false;

    // ���ݷ���Ŀ�������ж�
    switch (_targetType) {
    case SpellTarget::SINGLE_TARGET:
        return true;  // ����Ŀ�꣬�κ���ЧĿ�궼����
    case SpellTarget::FRIENDLY:
        // ���Ŀ���Ƿ�Ϊ�ѷ����
        return target->getOwner() == GameManager::getInstance()->getCurrentPlayer();
    case SpellTarget::ENEMY:
        // ���Ŀ���Ƿ�Ϊ�з����
        return target->getOwner() == GameManager::getInstance()->getOpponentPlayer();
    default:
        return false;
    }
}

// ���ŷ�����Ч
void SpellCard::playSpellEffect() {
    // ���ö������������ŷ�������
    AnimationManager::getInstance()->playSpellAnimation(getName(), nullptr);
}

// ����ʩ����ɺ�Ĵ���
void SpellCard::onSpellComplete() {
    // �����������Ƶ����ƶ�
    GameManager::getInstance()->discardCard(this);
}

/*
ʹ��ʾ����

// ����һ����������
auto spell = SpellCard::create(1, "������");

// ���÷�������
spell->setTargetType(SpellTarget::SINGLE_TARGET);
spell->setNeedsTarget(true);

// �����ѡ��Ŀ���ʩ�ŷ���
Card* target = getSelectedTarget();
if (target) {
    spell->castSpell(target);
}
*/