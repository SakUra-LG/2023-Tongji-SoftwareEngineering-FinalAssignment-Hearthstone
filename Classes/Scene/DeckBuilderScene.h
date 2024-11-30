
#ifndef __DECK_BUILDER_SCENE_H__
#define __DECK_BUILDER_SCENE_H__

#include "cocos2d.h"

class DeckBuilderScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;

    CREATE_FUNC(DeckBuilderScene);

private:
    void initBackground1();
    void initMenu1();
    void initUI1();
    void initListeners1();      // 这个函数需要实现
    void onPageRight(cocos2d::Ref* sender);
    void onPageLeft(cocos2d::Ref* sender);
    void onReturnMenu(cocos2d::Ref* sender);
    // 菜单回调
    ///void onPlayGame(cocos2d::Ref* sender);
    //void onAdventureMode(cocos2d::Ref* sender);
   //void DeckBuilderScene::onBattleGronds(Ref* sender);
   // void onDeckBuilder(cocos2d::Ref* sender);
   // void onSettings(cocos2d::Ref* sender);
   // void DeckBuilderScene::onSelection(Ref* sender);
   // void onQuit(cocos2d::Ref* sender);
   // void onEndTurnClicked(cocos2d::Ref* sender);    // 这个函数需要实现
   // void onSettingsClicked(cocos2d::Ref* sender);   // 这个函数需要实现
    // 动画
   // void playMenuAnimation();
   // void playButtonAnimation(cocos2d::Node* button);
};

#endif // __DECK_BUILDER_SCENE_H__
