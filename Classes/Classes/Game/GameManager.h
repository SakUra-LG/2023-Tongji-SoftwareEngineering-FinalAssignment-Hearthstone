// GameManager.h
// 游戏管理器类 - 负责管理游戏的核心逻辑和状态
// 使用单例模式确保全局只有一个实例

#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "cocos2d.h"
#include <vector>
#include <queue>

// 前向声明，避免循环包含
class Card;
class Player;

// 游戏管理器类
// 负责：
// 1. 游戏流程控制
// 2. 玩家管理
// 3. 卡牌操作
// 4. 游戏状态维护
class GameManager {
public:
    // 获取游戏管理器的单例实例
    // @return: 返回 GameManager 的唯一实例
    static GameManager* getInstance();

    // ====== 游戏流程控制 ======

    // 初始化游戏环境和资源
    // 在游戏开始前调用，设置初始状态
    void initGame();

    // 开始新游戏
    // 包括：洗牌、分发初始手牌、决定先手等
    void startGame();

    // 结束当前游戏
    // 清理资源，显示结算界面
    void endGame();

    // ====== 玩家管理 ======

    // 设置当前行动的玩家
    // @param player: 要设置为当前玩家的指针
    void setCurrentPlayer(Player* player);

    // 获取当前行动的玩家
    // @return: 当前玩家的指针
    Player* getCurrentPlayer() const;

    // 获取对手玩家
    // @return: 对手玩家的指针
    Player* getOpponentPlayer() const;

    // ====== 卡牌操作 ======
    void discardCard(Card* card);//弃牌
    // 为指定玩家抽一张牌
    // @param player: 要抽牌的玩家
    void drawCard(Player* player);

    // 在指定位置打出一张卡牌
    // @param card: 要打出的卡牌
    // @param position: 卡牌放置的目标位置
    void playCard(Card* card, const cocos2d::Vec2& position);

    // 向玩家牌库添加卡牌
    // @param player: 目标玩家
    // @param card: 要添加的卡牌
    void addCardToDeck(Player* player, Card* card);

    // 目标选择相关
    void setSelectedTarget(Card* target);  // 设置当前选中的目标
    Card* getSelectedTarget() const;       // 获取当前选中的目标
    void clearSelectedTarget();            // 清除当前选中的目标
    // ====== 游戏状态 ======

    // 检查游戏是否结束
    // @return: 如果游戏结束返回 true，否则返回 false
    bool isGameOver() const;

    // 检查游戏状态
    // 包括：检查胜负条件、回合限制等
    void checkGameState();

private:
    // 私有构造函数，防止外部直接创建实例
    GameManager();

    // 私有析构函数，确保正确清理资源
    ~GameManager();

    // 单例实例指针
    static GameManager* _instance;

    // 当前行动玩家
    Player* _currentPlayer;

    // 对手玩家
    Player* _opponentPlayer;

    // 游戏是否结束的标志
    bool _isGameOver;

    // 当前选中的目标卡牌
    Card* _selectedTarget;

    // ====== 禁用拷贝操作 ======

    // 禁用拷贝构造函数
    // 确保不会意外创建对象副本
    GameManager(const GameManager&) = delete;

    // 禁用赋值运算符
    // 确保不会意外复制对象
    GameManager& operator=(const GameManager&) = delete;
};

#endif // __GAME_MANAGER_H__

/*
使用示例：

// 获取游戏管理器实例
auto gameManager = GameManager::getInstance();

// 初始化并开始新游戏
gameManager->initGame();
gameManager->startGame();

// 执行游戏操作
gameManager->drawCard(gameManager->getCurrentPlayer());

// 检查游戏状态
if (gameManager->isGameOver()) {
    gameManager->endGame();
}
*/