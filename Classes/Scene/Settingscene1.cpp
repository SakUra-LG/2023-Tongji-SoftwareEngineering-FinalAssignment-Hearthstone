#include "Settingscene1.h"
#include "GameScene.h"
#include "Utils/Constants.h"
#include "Audio/AudioManager.h"
#include"MenuScene.h"
#pragma execution_character_set("utf-8")
USING_NS_CC;
// 创建场景的静态方法
Scene* Settingscene1::createScene() {
    return Settingscene1::create();
}
// 场景初始化方法
bool Settingscene1::init() {
    if (!Scene::init()) {
        return false;
    }
    // 初始化场景的三个主要组件
    Settingscene1::initBackground1();// 初始化背景
    Settingscene1::initMenu1(); // 初始化菜单按钮
    Settingscene1::initUI1();// 初始化UI元素
    return true;
}

void Settingscene1::initBackground1() { // 获取屏幕尺寸和原点位置
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // 创建并设置背景精灵
    auto background = Sprite::create(GameConstants::Resources::BACKGROUND_SETTING);
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x,
        visibleSize.height / 2 + origin.y));
    this->addChild(background);
}

void Settingscene1::initMenu1() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // 创建四个主要菜单项：开始游戏、卡组构建、设置和退出
       // 设置它们的垂直间距和位置
       // 添加点击回调函数
       // 创建动画效果

    float upOffset = 70.0f;  // 整体性上偏移量
    float spacing = 80.0f;

    // 设置背景图
    auto settingBackground = Sprite::create("menu/SettingBackground.png");
    settingBackground->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + upOffset));

    this->addChild(settingBackground, 0);

    // 设置菜单按钮
    auto selectionItem = MenuItemImage::create(
        "menu/Selection_Button_Normal.png",
        "menu/Selection_Button_Selected.png",
        CC_CALLBACK_1(Settingscene1::onSelection1, this));

    auto quitItem = MenuItemImage::create(
        "menu/Quite_Button_normal.png",
        "menu/Quite_Button_Selected.png",
        CC_CALLBACK_1(Settingscene1::onQuit1, this));

    //现为设置按钮，应该为设置按钮（内容不改，图片更换，位置更换）
    auto settingsItem = MenuItemImage::create(
        "menu/Setting_Button_Normal.png",
        "menu/Setting_Button_Selected.png",
        CC_CALLBACK_1(Settingscene1::onSettings1, this));

    selectionItem->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + upOffset + 58));
    quitItem->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + upOffset - 84));
    settingsItem->setPosition(Vec2(visibleSize.width - settingsItem->getContentSize().width / 2, settingsItem->getContentSize().height / 2));
    auto menu2 = Menu::create(selectionItem, quitItem, settingsItem, nullptr);
    menu2->setPosition(Vec2(0, 0));

    this->addChild(menu2,1);
    
    
    // 播放菜单动画
    //playMenuAnimation();
}

void Settingscene1::initUI1() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 添加标题
    auto titleLabel = Label::createWithTTF("HearthStone", "fonts/arial.ttf", 48);
    if (titleLabel) {
        titleLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - titleLabel->getContentSize().height));
        this->addChild(titleLabel, 1);
    }

    // 添加版本号
    auto versionLabel = Label::createWithTTF("Version 1.0", "fonts/arial.ttf", 24);
    if (versionLabel) {
        versionLabel->setPosition(Vec2(versionLabel->getContentSize().width / 2,
            visibleSize.height - versionLabel->getContentSize().height / 2));
        this->addChild(versionLabel, 1);
    }

    
    // 菜单项回调函数
}

void Settingscene1::onSelection1(Ref* sender)
{
    //此功能考虑到实际作用不大，视时间情况完成
}

void Settingscene1::onQuit1(Ref* sender) {
    AudioManager::getInstance()->playEffect(GameConstants::Sound::MAIN_MENU_THREEBUTTON_CLICK);
    Director::getInstance()->end();    // 播放按钮点击音效
    // 退出游戏
}

void Settingscene1::onSettings1(Ref* sender)
{
    AudioManager::getInstance()->playEffect(GameConstants::Sound::CHANGE_HELP_SCENE);

    // 切换到设置1场景
    auto scene = MenuScene::createScene();
    Director::getInstance()->replaceScene(scene);
}