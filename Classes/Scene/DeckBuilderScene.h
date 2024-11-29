#ifndef __Deck_Builder_Scene_H__
#define __Deck_Builder_Scene_H__

#include "cocos2d.h"
#include <vector>

class Card;
class Player;

class GameScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;

    CREATE_FUNC(GameScene);

private:
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
    

    // 初始化方法
    void initLayers();
    void initUI();
    void initListeners();

    // 更新方法
    void updateUI(float dt);
    void updateHandPositions();
    void updateFieldPositions();
    void addDebugLabels();
    // 事件处理
    void onEndTurnClicked(cocos2d::Ref* sender);
    void onSettingsClicked(cocos2d::Ref* sender);
    void onCardPlayed(Card* card);
    void onCardDrawn(Card* card);
    // 动画
    void animateCardDraw(Card* card);
    void animateCardPlay(Card* card);
    void animateAttack(Card* attacker, Card* target);
};

#endif // __Deck_Builder_Scene_H__
