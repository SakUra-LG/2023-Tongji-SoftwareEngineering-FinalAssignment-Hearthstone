 
#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "cocos2d.h"
#include <vector>
#include <queue>
#include "Network/GameAction.h"  // 添加这行，包含完整的 GameAction 定义
class Card;
class Player;


class GameManager {
public:
    static GameManager* getInstance();

    // 游戏状态
    enum class GameState {
        MENU,
        PLAYING,
        PAUSED,
        GAME_OVER
    };
    Card* getSelectedTarget() const;
    void selectTarget(Card* target);  // 添加这行
    // 游戏控制
    void startGame();
    void pauseGame();
    void resumeGame();
    void endGame();

    // 玩家管理
    void setCurrentPlayer(Player* player);
    Player* getCurrentPlayer() const { return _currentPlayer; }
    Player* getOpponentPlayer() const { return _opponentPlayer; }

    // 卡牌操作
    void playCard(Card* card, const cocos2d::Vec2& position);
    void drawCard(Player* player);
    void discardCard(Card* card);

    // 游戏逻辑
    void processGameAction(const GameAction& action);
    void checkGameState();
    bool isGameOver() const { return _gameState == GameState::GAME_OVER; }

private:
    GameManager();
    ~GameManager();
    static GameManager* _instance;

    // 游戏状态
    GameState _gameState;
    Player* _currentPlayer;
    Player* _opponentPlayer;
    Card* _selectedTarget = nullptr;
    std::queue<GameAction> _actionQueue;

    void initGame();
    void cleanupGame();
    void processActionQueue();
};

#endif // __GAME_MANAGER_H__