
#ifndef __SETTING1_SCENE_H__
#define __SETTING1_SCENE_H__

#include "cocos2d.h"
#pragma execution_character_set("utf-8")
class Settingscene1 : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;

    CREATE_FUNC(Settingscene1);

private:
    void initBackground1();//
    void initMenu1();//
    void initUI1();
    void initListeners1();      // 这个函数需要实现
    // 菜单回调
    //void onPlayGame(cocos2d::Ref* sender);
    //void onAdventureMode(cocos2d::Ref* sender);
    //void onDeckBuilder(cocos2d::Ref* sender);
    void onSettings1(cocos2d::Ref* sender);//
    void Settingscene1::onSelection1(Ref* sender);//
    void onQuit1(cocos2d::Ref* sender);//
    //void onEndTurnClicked(cocos2d::Ref* sender);    // 这个函数需要实现
    //void onSettingsClicked(cocos2d::Ref* sender);   // 这个函数需要实现
    //void updateUI(float dt);   // 这个函数需要实现
    // 动画
    //void playMenuAnimation();
    //void playButtonAnimation(cocos2d::Node* button);
};

#endif // __SETTING1_SCENE_H__