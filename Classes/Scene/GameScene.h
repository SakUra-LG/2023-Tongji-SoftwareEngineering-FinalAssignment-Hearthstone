#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__
#pragma execution_character_set("utf-8")
#include "cocos2d.h"
#include <vector>
#include <map>

class Card;
class Player;
class Deck;

class GameScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    static cocos2d::Scene* createWithDeck(Deck* deck);
    virtual bool init() override;
    bool initWithDeck(Deck* deck);

    CREATE_FUNC(GameScene);
    virtual ~GameScene();

private:
    // 玩家相关
    Player* _currentPlayer = nullptr;    // 当前玩家
    Player* _opponentPlayer = nullptr;   // 对手玩家
    Deck* _playerDeck = nullptr;         // 玩家卡组

    // 游戏状态
    bool _isPlayerTurn = true;           // 是否是玩家回合
    bool _isAttacking = false;           // 是否在攻击状态
    int _turnTimeRemaining = 30;         // 回合剩余时间
    const int TURN_TIME = 30;            // 回合总时间（秒）

    // UI层和游戏层
    cocos2d::Node* _gameLayer = nullptr;
    cocos2d::Node* _uiLayer = nullptr;
    cocos2d::Node* _handLayer = nullptr;
    cocos2d::Node* _playerField = nullptr;
    cocos2d::Node* _opponentHand = nullptr;
    cocos2d::Node* _opponentField = nullptr;
    
    // UI元素
    cocos2d::Label* _turnIndicatorLabel = nullptr;
    cocos2d::Label* _turnTimerLabel = nullptr;
    cocos2d::Label* _playerManaLabel = nullptr;
    cocos2d::Label* _playerHealthLabel = nullptr;
    cocos2d::Label* _opponentManaLabel = nullptr;
    cocos2d::Label* _opponentHealthLabel = nullptr;
    cocos2d::MenuItemImage* _endTurnButton = nullptr;
    cocos2d::MenuItemImage* _settingsButton = nullptr;
    cocos2d::MenuItemImage* _playerHeroPower = nullptr;

    // 卡牌相关
    cocos2d::Sprite* _selectedCard = nullptr;
    std::vector<Card*> _playerHand;  // 存储玩家手牌的卡牌指针
    std::vector<cocos2d::Sprite*> _playerHandSprites;  // 存储手牌的精灵
    std::vector<cocos2d::Sprite*> _playerFieldCards;
    std::vector<cocos2d::Sprite*> _opponentHandCards;
    std::vector<cocos2d::Sprite*> _opponentFieldCards;
    std::map<cocos2d::Sprite*, bool> _hasAttacked;

    // 卡牌常量
    static constexpr float CARD_SPACING = 100.0f;
    static constexpr float CARD_SCALE = 0.1f;
    static constexpr float CARD_MOVE_DURATION = 0.5f;

    // 方法声明
    void dealCard(const std::string& cardName, bool isPlayerCard, int handPosition);
    void createCardSprite(Card* card, bool isPlayerCard);
    void arrangeHandCards(bool isPlayerHand);
    void drawInitialCard();
    void updateHandPositions();
    void updateFieldPositions();
    void addDebugLabels();
    void showGameOverUI();
    void addCardInteraction(cocos2d::Sprite* cardSprite);
    void addBattleCardInteraction(cocos2d::Sprite* cardSprite);
    void playCardToField(cocos2d::Sprite* cardSprite);
    void attackCard(cocos2d::Sprite* attacker, cocos2d::Sprite* target);

    // 初始化方法
    void initLayers();
    void initUI();
    void initListeners();
    void initGame();
    void initBackground();
    void drawInitialHand();
    
    // 更新方法
    void updateUI(float dt);
    void updateTurnTimer(float dt);
    void updateTurnIndicator();
    
    // 事件处理
    void onEndTurnClicked(cocos2d::Ref* sender);
    void onSettingsClicked(cocos2d::Ref* sender);
    void onCardPlayed(Card* card);
    void onCardDrawn(Card* card);
    void onHeroPowerClicked(cocos2d::Ref* sender);
    
    // 回合控制
    void startTurn();
    void endTurn();
    void useHeroPower();
};

#endif // __GAME_SCENE_H__
