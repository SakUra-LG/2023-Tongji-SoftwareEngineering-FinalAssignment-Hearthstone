#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__
#pragma execution_character_set("utf-8")
#include "cocos2d.h"
#include <vector>
#include <queue>
#include "Network/GameAction.h"

class Card;
class Player;

class GameManager {
public:
    // 游戏状态枚举
    enum class GameState {
        MENU,
        PLAYING,
        PAUSED,
        GAME_OVER
    };

    // 单例访问
    static GameManager* getInstance();

    // ====== 游戏流程控制 ======
    void initGame();
    void startGame();
    void pauseGame();
    void resumeGame();
    void endGame();
    void cleanupGame();

    // ====== 玩家管理 ======
    void setCurrentPlayer(Player* player);
    Player* getCurrentPlayer() const { return _currentPlayer; }
    Player* getOpponentPlayer() const { return _opponentPlayer; }

    // ====== 卡牌操作 ======
    void playCard(Card* card, const cocos2d::Vec2& position);
    void drawCard(Player* player);
    void discardCard(Card* card);
    void addCardToDeck(Player* player, Card* card);

    // ====== 目标选择 ======
    Card* getSelectedTarget() const;
    void selectTarget(Card* target);
    void clearSelectedTarget() { _selectedTarget = nullptr; }
    // 英雄技能相关
    bool useHeroPower(Player* player);
    bool canUseHeroPower(Player* player) const;
    static const int HERO_POWER_COST = 2;  // 英雄技能费用
    // ====== 游戏状态 ======
    void processGameAction(const GameAction& action);
    void checkGameState();
    bool isGameOver() const { return _gameState == GameState::GAME_OVER; }

    // 处理卡牌死亡
    void handleCardDeath(Card* card);

private:
    GameManager();
    ~GameManager();

    // 禁用拷贝
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    static GameManager* _instance;

    // 成员变量
    GameState _gameState;
    Player* _currentPlayer;
    Player* _opponentPlayer;
    Card* _selectedTarget;
    std::queue<GameAction> _actionQueue;

    // 私有方法
    void processActionQueue();
};

#endif // __GAME_MANAGER_H__
