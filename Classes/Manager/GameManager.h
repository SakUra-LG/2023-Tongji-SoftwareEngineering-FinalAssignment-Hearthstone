#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "cocos2d.h"
#include <vector>
#include <queue>
#include "Network/GameAction.h"

class Card;
class Player;

class GameManager {
public:
    // ��Ϸ״̬ö��
    enum class GameState {
        MENU,
        PLAYING,
        PAUSED,
        GAME_OVER
    };

    // ��������
    static GameManager* getInstance();

    // ====== ��Ϸ���̿��� ======
    void initGame();
    void startGame();
    void pauseGame();
    void resumeGame();
    void endGame();
    void cleanupGame();

    // ====== ��ҹ��� ======
    void setCurrentPlayer(Player* player);
    Player* getCurrentPlayer() const { return _currentPlayer; }
    Player* getOpponentPlayer() const { return _opponentPlayer; }

    // ====== ���Ʋ��� ======
    void playCard(Card* card, const cocos2d::Vec2& position);
    void drawCard(Player* player);
    void discardCard(Card* card);
    void addCardToDeck(Player* player, Card* card);

    // ====== Ŀ��ѡ�� ======
    Card* getSelectedTarget() const;
    void selectTarget(Card* target);
    void clearSelectedTarget() { _selectedTarget = nullptr; }

    // ====== ��Ϸ״̬ ======
    void processGameAction(const GameAction& action);
    void checkGameState();
    bool isGameOver() const { return _gameState == GameState::GAME_OVER; }

private:
    GameManager();
    ~GameManager();

    // ���ÿ���
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    static GameManager* _instance;

    // ��Ա����
    GameState _gameState;
    Player* _currentPlayer;
    Player* _opponentPlayer;
    Card* _selectedTarget;
    std::queue<GameAction> _actionQueue;

    // ˽�з���
    void processActionQueue();
};

#endif // __GAME_MANAGER_H__