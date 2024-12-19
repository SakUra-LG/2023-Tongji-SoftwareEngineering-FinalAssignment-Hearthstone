#ifndef __MINION_CARD_H__
#define __MINION_CARD_H__
#pragma execution_character_set("utf-8")
#include "Card.h"

/**
 * @class MinionCard
 * @brief 随从卡牌类，继承自Card基类
 * 
 * 随从卡牌是可以放置在战场上的生物单位，具有攻击力和生命值属性。
 * 随从可以进行攻击，承受伤害，获得增益效果等。
 */
class MinionCard : public Card {
public:
    /**
     * @brief 创建随从卡牌的静态工厂方法
     * @param id 卡牌的唯一标识符
     * @param name 卡牌的名称
     * @return 返回创建的随从卡牌指针，失败返回nullptr
     */
    static MinionCard* create(int id, const std::string& name);

    // 随从基本属性
    CC_SYNTHESIZE(int, _attack, Attack);               // 攻击力
    CC_SYNTHESIZE(int, _health, Health);               // 当前生命值
    CC_SYNTHESIZE(int, _maxHealth, MaxHealth);         // 最大生命值
    CC_SYNTHESIZE(bool, _canAttack, CanAttack);        // 是否可以攻击
    CC_SYNTHESIZE(bool, _hasTaunt, HasTaunt);          // 是否有嘲讽
    CC_SYNTHESIZE(bool, _hasDivineShield, HasDivineShield); // 是否有圣盾

    /**
     * @brief 检查是否可以攻击指定目标
     * @param target 目标卡牌
     * @return 如果可以攻击返回true，否则返回false
     */
    virtual bool canAttackTarget(Card* target) const;

    /**
     * @brief 对目标发起攻击
     * @param target 攻击目标
     */
    virtual void attackTarget(Card* target);

    /**
     * @brief 受到伤害时的处理
     * @param amount 伤害数值
     */
    virtual void takeDamage(int amount);

    /**
     * @brief 接受治疗
     * @param amount 治疗数值
     */
    virtual void heal(int amount);

    /**
     * @brief 打出随从卡牌
     */
    virtual void playCard();

    /**
     * @brief 添加增益效果
     * @param attack 攻击力增加值
     * @param health 生命值增加值
     */
    void addBuff(int attack, int health);

    /**
     * @brief 沉默随从，移除所有效果
     */
    void silence();

protected:
    /**
     * @brief 初始化随从卡牌
     * @param id 卡牌ID
     * @param name 卡牌名称
     * @return 初始化成功返回true，失败返回false
     */
    virtual bool init(int id, const std::string& name) override;

    /**
     * @brief 初始化UI组件
     */
    virtual void initUI();

    /**
     * @brief 更新UI显示
     */
    virtual void updateUI();

    /**
     * @brief 触发战吼效果
     */
    virtual void onBattlecry();

    /**
     * @brief 触发亡语效果
     */
    virtual void onDeathrattle();

private:
    /**
     * @brief 检查随从是否死亡
     */
    void checkDeath();

    /**
     * @brief 更新随从状态
     */
    void updateStats();

    Label* _attackLabel;   // 攻击力显示标签
    Label* _healthLabel;   // 生命值显示标签
};

#endif // __MINION_CARD_H__ 