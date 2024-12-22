#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__
#pragma execution_character_set("utf-8")
#include "cocos2d.h"
#include <vector>
#include <queue>
#include "Network/GameAction.h"
#include"Card/Card.h"
#include"Scene/GameScene.h"

class Card;
class Player;
class GameScene;
// 在某个头文件中定义游戏结果枚举
enum class GameResult {
    ONGOING,    // 游戏进行中
    VICTORY,    // 玩家胜利
    DEFEAT      // 玩家失败
};

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
    void setOpponentPlayer(Player* player);

    // ====== 卡牌操作 ======
    //void playCard(Card* card, const cocos2d::Vec2& position);
    void drawCard(Player* player);
    void discardCard(Card* card);
    //void addCardToDeck(Player* player, Card* card);

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
    bool isGameOver() const {
        auto logger = GameLogger::getInstance();
        logger->log(LogLevel::DEBUG, "开始检查游戏结束条件...");

        // 获取当前运行的场景
        auto scene = dynamic_cast<GameScene*>(cocos2d::Director::getInstance()->getRunningScene());
        if (!scene) {
            logger->log(LogLevel::WARNING, "无法获取GameScene");
            return false;
        }

        // 检查对手场上是否有卡牌
        logger->log(LogLevel::DEBUG, "对手场上卡牌数量: " + std::to_string(scene->_opponentFieldCards2.size()));
        if (scene->_opponentFieldCards2.empty()) {
            logger->log(LogLevel::DEBUG, "对手场上没有卡牌，游戏继续");
            return false;
        }

        int player = 0;
        int enemy = 0;
        
        // 记录玩家手牌状态
        logger->log(LogLevel::DEBUG, "检查玩家手牌状态，手牌数量: " + std::to_string(scene->_playerHand2.size()));
        for (const auto& minion : scene->_playerHand2) {
            if (!minion) {
                logger->log(LogLevel::WARNING, "玩家手牌中存在空指针");
                continue;
            }
            logger->log(LogLevel::DEBUG, "玩家卡牌: " + minion->getName() + 
                ", 生命值: " + std::to_string(minion->getHealth()) +
                ", isDead: " + std::to_string(minion->_isDead));
            if (minion->_isDead) {
                player++;
            }
        }
        logger->log(LogLevel::DEBUG, "玩家死亡卡牌总数: " + std::to_string(player));

        // 记录敌方场上卡牌状态
        logger->log(LogLevel::DEBUG, "检查敌方场上卡牌状态，场上卡牌数量: " + std::to_string(scene->_opponentFieldCards2.size()));
        for (const auto& minion : scene->_opponentFieldCards2) {
            if (!minion) {
                logger->log(LogLevel::WARNING, "敌方场上存在空指针");
                continue;
            }
            logger->log(LogLevel::DEBUG, "敌方卡牌: " + minion->getName() + 
                ", 生命值: " + std::to_string(minion->getHealth()) +
                ", isDead: " + std::to_string(minion->_isDead));
            if (minion->_isDead) {
                enemy++;
            }
        }
        logger->log(LogLevel::DEBUG, "敌方死亡卡牌总数: " + std::to_string(enemy));

        // 判断胜负
        if (player == 3) {
            logger->log(LogLevel::INFO, "玩家所有卡牌死亡 - 失败");
            _gameResult = GameResult::DEFEAT;
            return true;
        }

        if (enemy == 3) {
            logger->log(LogLevel::INFO, "敌方所有卡牌死亡 - 胜利");
            _gameResult = GameResult::VICTORY;
            return true;
        }

        logger->log(LogLevel::DEBUG, "游戏继续...");
        return false;
    }



    // 获取游戏结果
    GameResult getGameResult() const { return _gameResult; }

    // 处理卡牌死亡
    void handleCardDeath(Card* card);

    //void setOpponentFieldCards(const std::vector<Card*>& cards) { _opponentFieldCards2 = cards; }
    //void setPlayerHand(const std::vector<Card*>& cards) { _playerHand2 = cards; }

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
    mutable GameResult _gameResult = GameResult::ONGOING;  // 使用 mutable 因为 isGameOver 是 const

    // 私有方法
    void processActionQueue();

    //std::vector<Card*> _opponentFieldCards2;
    //std::vector<Card*> _playerHand2;
};

#endif // __GAME_MANAGER_H__
