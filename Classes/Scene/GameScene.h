#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__
#pragma execution_character_set("utf-8")
#include "cocos2d.h"
#include <vector>
#include <map>
#include"Card/MinionCard.h"

class Card;
class Player;
class Deck;

class GameScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createWithDeck(Deck* deck, Deck* deckenemy2);
    bool initWithDeck(Deck* deck, Deck* deckenemy2);

    CREATE_FUNC(GameScene);
    virtual ~GameScene();
    void handleCardSelection(Card* card, const Vec2& touchPos);
    //void handleTargetSelection(const Vec2& touchPos);
    void GameScene::clearSelection();
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);

    virtual bool init() override;

    // 从场上移除卡牌
    void removeCardFromField(Card* card);

    std::vector<MinionCard*> _opponentFieldCards2;
    std::vector<Card*> _opponentFieldCards;
    std::vector<Card*> _playerHand;  // 存储玩家手牌的卡牌指针
    std::vector<MinionCard*> _playerHand2;
private:
    // 玩家相关
    Player* _currentPlayer = nullptr;    // 当前玩家
    Player* _opponentPlayer = nullptr;   // 对手玩家
    Deck* _playerDeck = nullptr;         // 玩家卡组
    Deck* _enemyDeck = nullptr;

    bool isStart = false;           //用于判断是否开始进行胜利判断

    // 游戏状态
    bool _isPlayerTurn = true;           // 是否是玩家回合
    bool _isAttacking = false;           // 是否在攻击状态
    int _turnTimeRemaining = 30;         // 回合剩余时间
    const int TURN_TIME = 30;            // 回合总时间（秒）
    int deth=0;

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
    MinionCard* _selectedCard = nullptr;
    //std::vector<Card*> _playerHand;  // 存储玩家手牌的卡牌指针
    //std::vector<MinionCard*> _playerHand2;
    std::vector<cocos2d::Sprite*> _playerHandSprites;  // 存储手牌的精灵
    std::vector<Card*> _playerFieldCards;
    //std::vector<Card*> _opponentFieldCards;
    //std::vector<MinionCard*> _opponentFieldCards2;
    std::map<Card*, bool> _hasAttacked;

    // 卡牌常量
    static constexpr float CARD_SPACING = 100.0f;
    static constexpr float CARD_SCALE = 0.1f;
    static constexpr float CARD_MOVE_DURATION = 0.5f;

    // 添加对手手牌相关变量
    std::vector<cocos2d::Sprite*> _opponentHandSprites;  // 对手手牌精灵
    std::vector<Card*> _opponentHandCards;  // 对手手牌
    std::vector<MinionCard*> _opponentHandCards2;

    // 添加场地位置常量
    static constexpr float FIELD_Y = 300.0f;  // 场地的Y坐标
    static constexpr float FIELD_CARD_SPACING = 120.0f;  // 场上卡牌间距

    // 方法声明
    //void dealCard(const std::string& cardName, bool isPlayerCard, int handPosition);
    //void createCardSprite(Card* card, bool isPlayerCard);
    void arrangeHandCards(bool isPlayerHand);
    //void drawInitialCard();
    void updateHandPositions();
    void updateFieldPositions();
    void addDebugLabels();
    void showGameOverUI();
    void addCardInteraction(Card* card);
    void playCardToField(Card* card, const cocos2d::Vec2& position);
    bool isValidFieldPosition(const cocos2d::Vec2& position) const;
    void updateFieldCardPositions();
    std::vector<MinionCard*> convertToMinionCards(const std::vector<Card*>& cards);
    void GameScene::showGameOverUI(const std::string& title, const std::string& message);
    void GameScene::showGameOverUI2();
    // 初始化方法
    void initLayers();
    void initUI();
    void initListeners();
    void initBackground();
    void drawInitialHand();
    
    // 更新方法
    void updateUI(float dt);
    void updateTurnTimer(float dt);
    void updateTurnIndicator();
    void addMana(int amount);
    bool useMana(int amount);
    bool hasEnoughMana(int amount);
    // 事件处理
    void onEndTurnClicked(cocos2d::Ref* sender);
    void onSettingsClicked(cocos2d::Ref* sender);
    //void onCardPlayed(Card* card);
    //void onCardDrawn(Card* card);
    void onHeroPowerClicked(cocos2d::Ref* sender);
    // 回合控制
    void startTurn();
    void endTurn();
    //void useHeroPower();
    //void executeAITurn();
    void autoEndTurn(float delay);  // delay: 延迟时间（秒）
    // 战斗相关方法
    void addBattleCardInteraction(Card* card);
    void attackCard(Card* attacker, Card* target);

    void GameScene::initHandInteraction();  // 初始化手牌区域交互
    void showTemporaryMessage(const std::string& message, float duration = 1.0f);

    void drawEnemyCard();  // 抽取对手卡牌
    void playEnemyCardToField(Card* card);  // 将对手卡牌放到场上
    void updateEnemyFieldPositions();  // 更新对手场上卡牌位置

    // 添加新的成员变量(和选择目标有关系
    //Card* _selectedCard = nullptr;  // 当前选中的卡牌
    cocos2d::Sprite* _selectionSprite = nullptr;  // 选中效果精灵
    cocos2d::Sprite* _targetSprite = nullptr;  // 目标选择效果

};

#endif // __GAME_SCENE_H__
