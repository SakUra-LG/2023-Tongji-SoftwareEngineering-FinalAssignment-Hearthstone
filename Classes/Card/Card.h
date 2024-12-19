#ifndef __CARD_H__
#define __CARD_H__
#pragma execution_character_set("utf-8")
#include "cocos2d.h"
#include "Effect/IEffect.h"
#include <vector>
#include <memory>
#include "Utils/GameLogger.h"

USING_NS_CC;

class Player;  // 前向声明
class IEffect; // 前向声明
class CardEffect;  // 添加 CardEffect 的前向声明

// 卡牌类型枚举
enum class CardType {
    MINION,     // 随从
    SPELL,      // 法术
    HERO        // 英雄
};

// 卡牌稀有度
enum class CardRarity {
    NORMAL,     // 普通
    RARE,       // 稀有
    EPIC,       // 史诗
    LEGENDARY   // 传说
};

class Card : public Node {
protected:
    // 基本属性
    int _id;
    std::string _name;
    CardType _type;
    CardRarity _rarity;
    int _cost;
    std::string _description;
    int _count;  // 卡牌数量
    
    // UI组件
    Label* _nameLabel;
    Label* _costLabel;
    Label* _descriptionLabel;
    Sprite* _cardFrame;
    Sprite* _cardImage;
    
    // 特效列表
    std::vector<std::shared_ptr<IEffect>> _effects;
    
    // 构造函数
    Card() 
        : _id(0)
        , _cost(0)
        , _count(0)
        , _isPlayable(true)
        , _isSelected(false)
        , _hasEffect(false)
        , _isActive(true)
        , _isSilenced(false)
        , _isFrozen(false)
        , _owner(nullptr)
        , _nameLabel(nullptr)
        , _costLabel(nullptr)
        , _descriptionLabel(nullptr)
        , _cardFrame(nullptr)
        , _cardImage(nullptr)
        , _highlightSprite(nullptr)
        , _health(0)
        , _maxHealth(0)
        , _attack(0)
        , _hasAttacked(false)
    {
    }
    
    // 初始化方法
    virtual bool init(int id, const std::string& name);
    
    // UI初始化
    virtual void initUI();
    
    // 游戏状态
    Player* _owner;
    bool _isPlayable;
    bool _isSelected;
    bool _hasEffect;
    bool _isActive;
    bool _isSilenced;
    bool _isFrozen;
    Vec2 _originalPosition;
    Sprite* _highlightSprite;
    
    // 战斗相关属性
    int _health;
    int _maxHealth;
    int _attack;
    bool _hasAttacked;
    
    std::string _portraitPath;  // 卡牌图片路径
    
public:
    // 创建方法
    static Card* create(int id, const std::string& name);
    
    // Getters & Setters
    int getId() const { return _id; }
    const std::string& getName() const { return _name; }
    void setName(const std::string& name) { _name = name; }
    const std::string& getCardName() const { return _name; }
    CardType getType() const { return _type; }
    void setType(CardType type) { _type = type; }
    CardRarity getRarity() const { return _rarity; }
    void setRarity(CardRarity rarity) { _rarity = rarity; }
    int getCost() const { return _cost; }
    void setCost(int cost) { _cost = cost; }
    const std::string& getCardDescription() const { return _description; }
    void setCardDescription(const std::string& desc) { _description = desc; }
    int getCount() const { return _count; }
    void setCount(int count) { _count = count; }
    
    // 效果系统
    void addEffect(std::shared_ptr<IEffect> effect);
    void removeEffect(std::shared_ptr<IEffect> effect);
    void triggerEffects();
    const std::vector<std::shared_ptr<IEffect>>& getEffects() const { return _effects; }
    
    // 游戏状态
    bool isPlayable() const { return _isPlayable; }
    void setPlayable(bool playable) { _isPlayable = playable; }
    bool isSelected() const { return _isSelected; }
    void setSelected(bool selected) { _isSelected = selected; }
    bool hasEffect() const { return _hasEffect; }
    void setHasEffect(bool hasEffect) { _hasEffect = hasEffect; }
    bool isActive() const { return _isActive; }
    void setActive(bool active) { _isActive = active; }
    bool isSilenced() const { return _isSilenced; }
    void setSilenced(bool silenced) { _isSilenced = silenced; }
    bool isFrozen() const { return _isFrozen; }
    void setFrozen(bool frozen) { _isFrozen = frozen; }
    
    // 所有者
    Player* getOwner() const { return _owner; }
    void setOwner(Player* owner) { _owner = owner; }
    
    // 游戏行为
    virtual bool canPlay() const;
    virtual void onPlay();
    virtual void onDraw();
    virtual void onDiscard();
    virtual void takeDamage(int amount);
    virtual void heal(int amount);
    
    // UI相关
    void showPlayableHighlight();
    void returnToOriginalPosition();
    bool isInPlayableArea(const cocos2d::Vec2& position);
    Sprite* getSprite() const { return _cardImage; }
    void setSprite(Sprite* sprite) { _cardImage = sprite; }
    
    // 触摸事件
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    
    // 战斗相关访问器
    int getHealth() const { return _health; }
    void setHealth(int health) { _health = health; }
    int getMaxHealth() const { return _maxHealth; }
    void setMaxHealth(int maxHealth) { _maxHealth = maxHealth; }
    int getAttack() const { return _attack; }
    void setAttack(int attack) { _attack = attack; }
    bool hasAttacked() const { return _hasAttacked; }
    void setHasAttacked(bool attacked) { _hasAttacked = attacked; }
    
    // 卡牌类型相关
    CardType getCardType() const { return _type; }  // 为了兼容性
    void setCardType(CardType type) { _type = type; }  // 为了兼容性
    
    // 状态效果
    bool getHasProvoke() const { return false; }  // 默认实现
    virtual bool hasTaunt() const { return false; }  // 默认实现
    
    const std::string& getPortraitPath() const { return _portraitPath; }
    void setPortraitPath(const std::string& path) { _portraitPath = path; }
};

#endif // __CARD_H__
