// HealEffect.cpp
// ����Ч����ʵ���ļ��������������ƺ�Ⱥ�����ƹ���

#include "HealEffect.h"
#include "Card/Card.h"
#include "Game/GameManager.h"
#include "Animation/AnimationManager.h"
#include"Player/Player.h"
#include"Card/MinionCard.h"
#include "Game/GameManager.h"
// ���캯������ʼ������Ч��
// @param healing: ������
// @param targetAll: �Ƿ�ΪȺ������
HealEffect::HealEffect(int healing, bool targetAll)
    : _healing(healing)      // ����������
    , _targetAll(targetAll)  // �����Ƿ�Ⱥ������
{
    // ���캯����Ϊ�գ�ʹ�ó�ʼ���б���ɳ�ʼ��
}

// Ч������ʱ�Ĵ�����
// ����Ч�����ͣ�Ⱥ��/���壩ִ����Ӧ�������߼�
void HealEffect::onActivate() {
    if (_targetAll) {
        // �����Ⱥ�����ƣ�����Ⱥ�����ƺ���
        healAllTargets();
    }
    else {
        // �������ƣ���ȡ��ǰѡ�е�Ŀ��
        Card* target = GameManager::getInstance()->getSelectedTarget();
        if (target) {
            // �������ЧĿ�꣬��������
            healTarget(target);
        }
    }
}

// Ч��ͣ��ʱ�Ĵ�����
// ��������Ч����ص���Դ��״̬
void HealEffect::onDeactivate() {
    // ��ǰ�汾����������Դ
    // Ԥ���ӿ��Թ�������չ
}

// ���Ч���Ƿ���Ա�����
// @return: �����Ƿ���Լ���Ч��
bool HealEffect::canActivate() {
    // ���Ч���Ƿ�����������
    if (!_owner) return false;

    if (_targetAll) {
        // Ⱥ���������ǿ����ͷ�
        // ��ʹ����û�����˵���ӣ�Ҳ�����ͷż���
        return true;
    }
    else {
        // ��ȡ��ǰѡ�е�Ŀ��
        Card* target = GameManager::getInstance()->getSelectedTarget();
        // ���Ŀ���Ƿ����������ֵδ��
        return target && target->getHealth() < target->getMaxHealth();
    }
}

// �Ե���Ŀ���������
// @param target: ����Ŀ��
void HealEffect::healTarget(Card* target) {
    // ��ָ����
    if (!target) return;

    // �������ƶ���Ч��
    AnimationManager::getInstance()->playHealAnimation(target, _healing);

    // ���Ŀ���Ƿ�Ϊ��ӿ���
    if (target->getCardType() == CardType::MINION) {
        // ��Ŀ��ת��Ϊ��ӿ������Ͳ���������
        // ʹ�� dynamic_cast ȷ������ת���İ�ȫ��
        dynamic_cast<MinionCard*>(target)->heal(_healing);
    }
}

// ִ��Ⱥ������Ч��
// ���Ƴ����������
void HealEffect::healAllTargets() {
    // ��ȡ��Ϸ������ʵ��
    auto gameManager = GameManager::getInstance();

    // ��ȡ˫�����ϵ��������
    // ʹ�����ñ��ⲻ��Ҫ�ĸ���
    auto& playerField = gameManager->getCurrentPlayer()->getField();
    auto& opponentField = gameManager->getOpponentPlayer()->getField();

    // ���Ƽ��������������
    for (auto minion : playerField) {
        healTarget(minion);
    }

    // ���ƶԷ������������
    for (auto minion : opponentField) {
        healTarget(minion);
    }
}

/* ���ܵ���չ��
1. �������Ŀ��Ĺ�����������ֻ�����ѷ���ӣ�
2. �������Ч�������Σ����������ת��Ϊ���ף�
3. �������Ч���Ĵ����¼��������ƺ���ƣ�
4. ����������ļ��������������˼���Ч����
*/