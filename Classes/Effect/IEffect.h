#ifndef __I_EFFECT_H__
#define __I_EFFECT_H__

class Card; // 前向声明

/**
 * @enum TriggerType
 * @brief 效果触发类型枚举
 * 
 * 定义了所有可能的效果触发时机，用于确定效果何时被激活
 */
enum class TriggerType {
    ON_PLAY,        // 卡牌被打出时触发
    ON_DEATH,       // 随从死亡时触发
    START_OF_TURN,  // 回合开始时触发
    END_OF_TURN,    // 回合结束时触发
    ON_DAMAGE,      // 受到伤害时触发
    ON_HEAL,        // 接受治疗时触发
    BATTLECRY,      // 战吼效果触发
    DEATHRATTLE,    // 亡语效果触发
    ON_ATTACK,      // 发起攻击时触发
    ON_ATTACKED,    // 被攻击时触发
    ON_TURN_START,  // 回合开始时触发
    ON_TURN_END     // 回合结束时触发
};

/**
 * @class IEffect
 * @brief 效果系统的基础接口类
 * 
 * 所有卡牌效果都必须继承自该接口。
 * 提供了效果的基本行为定义，包括激活、停用、条件检查等功能。
 */
class IEffect {
public:
    /**
     * @brief 虚析构函数
     */
    virtual ~IEffect() = default;

    /**
     * @brief 激活效果
     * 当效果的触发条件满足时调用
     */
    virtual void onActivate() = 0;

    /**
     * @brief 停用效果
     * 当效果需要被移除或无效化时调用
     */
    virtual void onDeactivate() = 0;

    /**
     * @brief 检查效果是否可以被触发
     * @return 如果效果可以被触发返回true，否则返回false
     */
    virtual bool canActivate() = 0;

    /**
     * @brief 获取效果的触发类型
     * @return 返回效果的触发类型
     */
    virtual TriggerType getTriggerType() const = 0;

    /**
     * @brief 设置效果所属的卡牌
     * @param owner 效果所属的卡牌指针
     */
    virtual void setOwner(Card* owner) = 0;

    /**
     * @brief 获取效果所属的卡牌
     * @return 返回效果所属的卡牌指针
     */
    Card* getOwner() const { return _owner; }

protected:
    Card* _owner;  // 效果所属的卡牌指针
};

#endif // __I_EFFECT_H__ 