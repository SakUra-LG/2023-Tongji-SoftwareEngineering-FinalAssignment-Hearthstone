
#ifndef __SETTING1_SCENE_H__
#define __SETTING1_SCENE_H__

#include "cocos2d.h"

class Settingscene1 : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;

    CREATE_FUNC(Settingscene1);

private:
    void initBackground1();//
    void initMenu1();//
    void initUI1();
    void initListeners1();      // ���������Ҫʵ��
    // �˵��ص�
    //void onPlayGame(cocos2d::Ref* sender);
    //void onAdventureMode(cocos2d::Ref* sender);
    //void onDeckBuilder(cocos2d::Ref* sender);
    void onSettings1(cocos2d::Ref* sender);//
    void Settingscene1::onSelection1(Ref* sender);//
    void onQuit1(cocos2d::Ref* sender);//
    //void onEndTurnClicked(cocos2d::Ref* sender);    // ���������Ҫʵ��
    //void onSettingsClicked(cocos2d::Ref* sender);   // ���������Ҫʵ��
    //void updateUI(float dt);   // ���������Ҫʵ��
    // ����
    //void playMenuAnimation();
    //void playButtonAnimation(cocos2d::Node* button);
};

#endif // __SETTING1_SCENE_H__