#ifndef __EFFECT_TYPES_H__
#define __EFFECT_TYPES_H__

// ��������ö��
enum class TriggerType {
    ON_PLAY,        // ���Ʊ����ʱ
    ON_DEATH,       // ��������ʱ
    ON_DAMAGE,      // �ܵ��˺�ʱ
    ON_HEAL,        // �ܵ�����ʱ
    ON_ATTACK,      // ���𹥻�ʱ
    ON_ATTACKED,    // ������ʱ
    ON_TURN_START,  // �غϿ�ʼʱ
    ON_TURN_END     // �غϽ���ʱ
};

#endif // __EFFECT_TYPES_H__