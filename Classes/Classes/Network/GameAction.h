// GameAction.h
#ifndef __GAME_ACTION_H__
#define __GAME_ACTION_H__

#include <map>
#include <string>

// ��Ϸ��������ö��
enum class ActionType {
    GAME_START,     // ������У���Ϸ��ʼ����
    PLAY_CARD,      // ������ƣ�������ʹ�ÿ��ƣ�
    ATTACK,         // ������������ӹ�����Ӣ�۹�����
    END_TURN,       // �����غ�
    USE_HERO_POWER, // ʹ��Ӣ�ۼ���
    CONCEDE        // Ͷ������
};

/**
 * @brief ��Ϸ�����ṹ�壬����������ҵĸ�����Ϸ����
 *
 * ����ṹ�����ڷ�װ��Ϸ�еĸ��ֶ���������������ơ�������
 * ����ͨ�����紫�䣬����ͬ����Ҳ���
 */
struct GameAction {
    ActionType type;    // �������ͣ���������ʲô���͵Ĳ���
    int sourceId;       // ���������ߵ�ID�����磺�����ߵ�ID��ʹ�ÿ��Ƶ�ID��
    int targetId;       // ����Ŀ���ID�����磺��������Ŀ��ID��

    // ���������ֵ䣬���ڴ洢������ص�������Ϣ
    // key: ���ݱ�ʶ��
    // value: ����ֵ
    // ���磺{"damage": "5", "effect": "poison"}
    std::map<std::string, std::string> data;

    // Ĭ�Ϲ��캯��
    GameAction() : type(ActionType::PLAY_CARD), sourceId(0), targetId(0) {}
};

/*
ʹ��ʾ����

// ����һ����������
GameAction attackAction;
attackAction.type = ActionType::ATTACK;
attackAction.sourceId = 1;  // ������ID
attackAction.targetId = 2;  // Ŀ��ID
attackAction.data["damage"] = "5";  // �����˺�ֵ

// ����һ��ʹ�ÿ��ƶ���
GameAction playCardAction;
playCardAction.type = ActionType::PLAY_CARD;
playCardAction.sourceId = cardId;
playCardAction.data["position"] = "3";  // ����λ��
*/

#endif // __GAME_ACTION_H__