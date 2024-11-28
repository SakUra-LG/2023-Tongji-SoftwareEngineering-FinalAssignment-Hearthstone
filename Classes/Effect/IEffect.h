 
#ifndef __I_EFFECT_H__
#define __I_EFFECT_H__
class Card; // 前向声明
enum class TriggerType {
    ON_PLAY,            // 打出时
    ON_DEATH,           // 死亡时
    START_OF_TURN,      // 回合开始
    END_OF_TURN,        // 回合结束
    ON_DAMAGE,          // 受到伤害时
    ON_HEAL,            // 治疗时
    BATTLECRY,          // 战吼
    DEATHRATTLE,       // 亡语
    ON_ATTACK,      // 发起攻击时
    ON_ATTACKED,    // 被攻击时
    ON_TURN_START,  // 回合开始时
    ON_TURN_END     // 回合结束时
};

class IEffect {
public:
    virtual ~IEffect() = default;

    virtual void onActivate() = 0;
    virtual void onDeactivate() = 0;
    virtual bool canActivate() = 0;

    virtual TriggerType getTriggerType() const = 0;
    virtual void setOwner(Card* owner) = 0;
    // 添加获取所有者的方法
    Card* getOwner() const { return _owner; }  // 返回卡牌指针
protected:
    Card* _owner;
};

#endif // __I_EFFECT_H__