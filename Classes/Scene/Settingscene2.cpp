#include "Settingscene1.h"
#include "Settingscene2.h"
#include "GameScene.h"
#include "Utils/Constants.h"
#include "Audio/AudioManager.h"
#include"MenuScene.h"

USING_NS_CC;
// 创建场景的静态方法
Scene* Settingscene2::createScene() {
    return Settingscene2::create();
}
// 场景初始化方法
bool Settingscene2::init() {
    if (!Scene::init()) {
        return false;
    }
    // 初始化场景的三个主要组件
    Settingscene2::initBackground2();// 初始化背景
    Settingscene2::initMenu2(); // 初始化菜单按钮
    Settingscene2::initUI2();// 初始化UI元素
    return true;
}

void Settingscene2::initBackground2() { // 获取屏幕尺寸和原点位置
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // 创建并设置背景精灵
    auto background = Sprite::create(GameConstants::Resources::BACKGROUND_SETTING);
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x,
        visibleSize.height / 2 + origin.y));
    this->addChild(background);
}

void Settingscene2::initMenu2()
{

}

void Settingscene2::initUI2()
{

}
