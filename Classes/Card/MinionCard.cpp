// MinionCard.cpp

// ������Ҫ��ͷ�ļ�
#include "Card.h"              // ����������
#include "MinionCard.h"        // ��ӿ�����
#include "Game/TurnSystem.h"   // �غ�ϵͳ
#include "Effect/EffectManager.h"    // Ч��������
#include "Network/NetworkManager.h"   // ���������
#include "Animation/AnimationManager.h" // ����������

// ��ʼ����ӿ���
bool MinionCard::init(int id, const std::string& name) {
    // ���ø���ĳ�ʼ�����������ʧ���򷵻�false
    if (!Card::init(id, name)) {
        return false;
    }

    // ������ӵ�Ĭ������
    _attack = 1;              // ���ó�ʼ������Ϊ1
    _health = 1;              // ���ó�ʼ����ֵΪ1
    _maxHealth = 1;           // �����������ֵΪ1
    _canAttack = false;       // ���ó�ʼ״̬���ܹ�����ͨ����Ҫ�ȴ�һ�غϣ�
    _hasTaunt = false;        // ���ó�ʼ״̬�޳���Ч��
    _hasDivineShield = false; // ���ó�ʼ״̬��ʥ��Ч��

    // TODO: ��ЩĬ��ֵӦ�ô������ļ������ݿ��ж�ȡ
    // ������Ҫʵ������ϵͳ�����ؾ���Ŀ�����ֵ

    return true;  // ��ʼ���ɹ�
}

// �����ӿ��Ƶķ���
void MinionCard::playCard() {
    // �������Ƿ����㹻�ķ���ֵ��������ſ�
    if (!TurnSystem::getInstance()->useMana(_cost)) {
        return;  // �������ֵ���㣬ֱ�ӷ���
    }

    // ���Ŵ������ʱ�Ķ���Ч��
    AnimationManager::getInstance()->playCardAnimation(this);

    // �������Ƶ�ս��Ч��������еĻ���
    EffectManager::getInstance()->triggerEffects(TriggerType::BATTLECRY);

    // ����һ����Ϸ���������͵�����
    GameAction playAction;     // ����һ���µ���Ϸ��������
    playAction.type = ActionType::PLAY_CARD;  // ���ö�������Ϊ�������
    playAction.sourceId = getId();  // ���ñ�������Ƶ�ID
    playAction.targetId = 0;      // ����Ŀ��IDΪ0����ʾ���ض�Ŀ�꣩
    // playAction.extraData ����Ϊ�գ��޶���������Ҫ���ݣ�

    // ͨ�������������������������������
    NetworkManager::getInstance()->sendGameAction(playAction);
}