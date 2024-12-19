#ifndef __EFFECT_TYPES_H__
#define __EFFECT_TYPES_H__
#pragma execution_character_set("utf-8")
// 触发类型枚举
enum class TriggerType {
    ON_PLAY,        // 卡牌被打出时
    ON_DEATH,       // 卡牌死亡时
    ON_DAMAGE,      // 受到伤害时
    ON_HEAL,        // 受到治疗时
    ON_ATTACK,      // 发起攻击时
    ON_ATTACKED,    // 被攻击时
    ON_TURN_START,  // 回合开始时
    ON_TURN_END     // 回合结束时
};

#endif // __EFFECT_TYPES_H__