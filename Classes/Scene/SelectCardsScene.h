#ifndef __SELECT_CARDS_SCENE_H__
#define __SELECT_CARDS_SCENE_H__

#include "cocos2d.h"
#include "Card/DeckManager.h"
#include "GameScene.h"
#pragma execution_character_set("utf-8")
class SelectCardsScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;

    CREATE_FUNC(SelectCardsScene);

private:
    void initBackground3();//
    void initMenu3();//
    void initUI3();
    void initListeners3();     
    void onBack(cocos2d::Ref* sender);
    void onCards1(cocos2d::Ref* sender);
    void onCards2(cocos2d::Ref* sender);
    
    void onSettings3(cocos2d::Ref* sender);//
    void SelectCardsScene::onSelection3(Ref* sender);//
    void onQuit3(cocos2d::Ref* sender);//

    void displayInitialCards();
};

#endif // __SELECT_CARDS_SCENE_H__