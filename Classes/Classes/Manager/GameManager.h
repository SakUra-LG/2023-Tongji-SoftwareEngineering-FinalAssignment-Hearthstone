 
#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "cocos2d.h"
#include <vector>
#include <queue>
#include "Network/GameAction.h"  // ������У����������� GameAction ����
class Card;
class Player;


class GameManager {
public:
    static GameManager* getInstance();

    // ��Ϸ״̬
    enum class GameState {
        MENU,
        PLAYING,
        PAUSED,
        GAME_OVER
    };
    Card* getSelectedTarget() const;
    void selectTarget(Card* target);  // �������
    // ��Ϸ����
    void startGame();
    void pauseGame();
    void resumeGame();
    void endGame();

    // ��ҹ���
    void setCurrentPlayer(Player* player);
    Player* getCurrentPlayer() const { return _currentPlayer; }
    Player* getOpponentPlayer() const { return _opponentPlayer; }

    // ���Ʋ���
    void playCard(Card* card, const cocos2d::Vec2& position);
    void drawCard(Player* player);
    void discardCard(Card* card);

    // ��Ϸ�߼�
    void processGameAction(const GameAction& action);
    void checkGameState();
    bool isGameOver() const { return _gameState == GameState::GAME_OVER; }

private:
    GameManager();
    ~GameManager();
    static GameManager* _instance;

    // ��Ϸ״̬
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