
#ifndef __SETTING2_SCENE_H__
#define __SETTING2_SCENE_H__

#include "cocos2d.h"

class Settingscene2 : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;

    CREATE_FUNC(Settingscene2);

private:
    void initBackground2();
    void initMenu2();
    void initUI2();
    void initListeners2();      // 这个函数需要实现
    // 菜单回调
    //void onPlayGame(cocos2d::Ref* sender);
    //void onAdventureMode(cocos2d::Ref* sender);
    //void onDeckBuilder(cocos2d::Ref* sender);
    void onSettings2(cocos2d::Ref* sender);
    //void onQuit(cocos2d::Ref* sender);
    ///void onEndTurnClicked(cocos2d::Ref* sender);    // 这个函数需要实现
    //void onSettingsClicked(cocos2d::Ref* sender);   // 这个函数需要实现
    //void updateUI(float dt);   // 这个函数需要实现
    // 动画
   // void playMenuAnimation();
    //void playButtonAnimation(cocos2d::Node* button);
};

#endif // __SETTING2_SCENE_H__