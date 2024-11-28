 
#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"

class MenuScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;

    CREATE_FUNC(MenuScene);

private:
    void initBackground();
    void initMenu();
    void initUI();
    void initListeners();      // ���������Ҫʵ��
    // �˵��ص�
    void onPlayGame(cocos2d::Ref* sender);
    void onDeckBuilder(cocos2d::Ref* sender);
    void onSettings(cocos2d::Ref* sender);
    void onQuit(cocos2d::Ref* sender);
    void onEndTurnClicked(cocos2d::Ref* sender);    // ���������Ҫʵ��
    void onSettingsClicked(cocos2d::Ref* sender);   // ���������Ҫʵ��
    void updateUI(float dt);   // ���������Ҫʵ��
    // ����
    void playMenuAnimation();
    void playButtonAnimation(cocos2d::Node* button);
};

#endif // __MENU_SCENE_H__