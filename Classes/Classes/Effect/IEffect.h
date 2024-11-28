 
#ifndef __I_EFFECT_H__
#define __I_EFFECT_H__
class Card; // ǰ������
enum class TriggerType {
    ON_PLAY,            // ���ʱ
    ON_DEATH,           // ����ʱ
    START_OF_TURN,      // �غϿ�ʼ
    END_OF_TURN,        // �غϽ���
    ON_DAMAGE,          // �ܵ��˺�ʱ
    ON_HEAL,            // ����ʱ
    BATTLECRY,          // ս��
    DEATHRATTLE,       // ����
    ON_ATTACK,      // ���𹥻�ʱ
    ON_ATTACKED,    // ������ʱ
    ON_TURN_START,  // �غϿ�ʼʱ
    ON_TURN_END     // �غϽ���ʱ
};

class IEffect {
public:
    virtual ~IEffect() = default;

    virtual void onActivate() = 0;
    virtual void onDeactivate() = 0;
    virtual bool canActivate() = 0;

    virtual TriggerType getTriggerType() const = 0;
    virtual void setOwner(Card* owner) = 0;
    // ��ӻ�ȡ�����ߵķ���
    Card* getOwner() const { return _owner; }  // ���ؿ���ָ��
protected:
    Card* _owner;
};

#endif // __I_EFFECT_H__