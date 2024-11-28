// GameAction.h
#ifndef __GAME_ACTION_H__
#define __GAME_ACTION_H__

#include <map>
#include <string>

// 游戏动作类型枚举
enum class ActionType {
    GAME_START,     // 添加这行：游戏开始动作
    PLAY_CARD,      // 打出卡牌（从手牌使用卡牌）
    ATTACK,         // 攻击动作（随从攻击或英雄攻击）
    END_TURN,       // 结束回合
    USE_HERO_POWER, // 使用英雄技能
    CONCEDE        // 投降认输
};

/**
 * @brief 游戏动作结构体，用于描述玩家的各种游戏操作
 *
 * 这个结构体用于封装游戏中的各种动作，包括打出卡牌、攻击等
 * 可以通过网络传输，用于同步玩家操作
 */
struct GameAction {
    ActionType type;    // 动作类型，表明这是什么类型的操作
    int sourceId;       // 动作发起者的ID（例如：攻击者的ID或被使用卡牌的ID）
    int targetId;       // 动作目标的ID（例如：被攻击的目标ID）

    // 额外数据字典，用于存储动作相关的其他信息
    // key: 数据标识符
    // value: 数据值
    // 例如：{"damage": "5", "effect": "poison"}
    std::map<std::string, std::string> data;

    // 默认构造函数
    GameAction() : type(ActionType::PLAY_CARD), sourceId(0), targetId(0) {}
};

/*
使用示例：

// 创建一个攻击动作
GameAction attackAction;
attackAction.type = ActionType::ATTACK;
attackAction.sourceId = 1;  // 攻击者ID
attackAction.targetId = 2;  // 目标ID
attackAction.data["damage"] = "5";  // 设置伤害值

// 创建一个使用卡牌动作
GameAction playCardAction;
playCardAction.type = ActionType::PLAY_CARD;
playCardAction.sourceId = cardId;
playCardAction.data["position"] = "3";  // 放置位置
*/

#endif // __GAME_ACTION_H__