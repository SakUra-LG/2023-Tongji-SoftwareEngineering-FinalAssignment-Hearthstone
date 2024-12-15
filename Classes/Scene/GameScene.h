#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include <vector>

class Card;
class Player;
class Deck;
// 添加游戏状态枚举

class GameScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    static Scene* createWithDeck(Deck* deck);
    virtual bool init() override;
    virtual bool initWithDeck(Deck* deck);

    CREATE_FUNC(GameScene);
    virtual ~GameScene();

private:
    // 添加新的成员变量
    cocos2d::Sprite* _selectedCard = nullptr;    // 当前选中的卡牌
    bool _isAttacking = false;                   // 是否在攻击状态
    std::map<cocos2d::Sprite*, bool> _hasAttacked;  // 记录每张卡本回合是否已攻击
    // 卡牌相关
    void dealCard(const std::string& cardName, bool isPlayerCard, int handPosition);
    void createCardSprite(const std::string& cardName, bool isPlayerCard);
    void arrangeHandCards(bool isPlayerHand);

    // 卡牌常量
    const float CARD_SPACING = 80.0f;        // 卡牌间距
    const float CARD_SCALE = 0.8f;           // 卡牌缩放
    const float CARD_MOVE_DURATION = 0.5f;   // 卡牌移动动画时间
    std::vector<cocos2d::Sprite*> _playerFieldCards;    // 玩家战场卡牌
    std::vector<cocos2d::Sprite*> _opponentFieldCards;  // 对手战场卡牌
    // 卡牌容器
    std::vector<cocos2d::Sprite*> _playerHandCards;    // 玩家手牌精灵
    std::vector<cocos2d::Sprite*> _opponentHandCards;  // 对手手牌精灵
    // UI组件
    cocos2d::Node* _gameLayer;
    cocos2d::Node* _uiLayer;
    cocos2d::Node* _playerHand;
    cocos2d::Node* _playerField;
    cocos2d::Node* _opponentHand;
    cocos2d::Node* _opponentField;

    // UI元素
    cocos2d::Label* _turnTimerLabel;
    cocos2d::MenuItemImage* _playerHeroPower;  // 添加英雄技能按钮
    cocos2d::Label* _playerManaLabel;
    cocos2d::Label* _playerHealthLabel;
    cocos2d::Label* _opponentManaLabel;
    cocos2d::Label* _opponentHealthLabel;

    // 按钮
    cocos2d::MenuItemImage* _endTurnButton;
    cocos2d::MenuItemImage* _settingsButton;
    // 回合相关
    bool _isPlayerTurn;         // 是否是玩家回合
    int _turnTimeRemaining;     // 回合剩余时间
    const int TURN_TIME = 30;   // 回合总时间（秒）
    void addCardInteraction(cocos2d::Sprite*);
    // 回合控制方法
    void startTurn();          // 开始新回合
    void endTurn();           // 结束当前回合
    void updateTurnTimer(float dt);  // 更新回合计时器
    // 初始化方法
    void initLayers();
    void initUI();
    void initListeners();
    void initGame();
    // 更新方法
    void updateUI(float dt);
    void updateHandPositions();
    void updateFieldPositions();
    void addDebugLabels();
    void showGameOverUI();
    // 事件处理
    void onEndTurnClicked(cocos2d::Ref* sender);
    void onSettingsClicked(cocos2d::Ref* sender);
    void onCardPlayed(Card* card);
    void onCardDrawn(Card* card);
    void onHeroPowerClicked(cocos2d::Ref* sender);  // 添加英雄技能回调
    void playCardToField(cocos2d::Sprite* cardSprite);
    void addBattleCardInteraction(cocos2d::Sprite* cardSprite);
    void attackCard(cocos2d::Sprite* attacker, cocos2d::Sprite* target);
    // 动画
    void animateCardDraw(Card* card);
    void animateCardPlay(Card* card);
    void animateAttack(Card* attacker, Card* target);
    cocos2d::Label* _turnIndicatorLabel;  // 回合指示器
    void updateTurnIndicator();           // 更新回合指示器
    void useHeroPower();                  // 使用英雄技能

    Deck* _playerDeck;
};

#endif // __GAME_SCENE_H__
