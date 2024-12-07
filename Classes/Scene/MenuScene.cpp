#include "MenuScene.h"
#include "GameScene.h"
#include "Utils/Constants.h"
#include "Audio/AudioManager.h"
#include "Settingscene1.h"
#include "DeckBuilderScene.h"

USING_NS_CC;
// 创建场景的静态方法
Scene* MenuScene::createScene() {
    return MenuScene::create();
}
// 场景初始化方法
bool MenuScene::init() {
    if (!Scene::init()) {
        return false;
    }
    // 初始化场景的三个主要组件
    initBackground();// 初始化背景
    initMenu(); // 初始化菜单按钮
    initUI();// 初始化UI元素
    return true;
}

void MenuScene::initBackground() { // 获取屏幕尺寸和原点位置
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // 创建并设置背景精灵
    auto background = Sprite::create(GameConstants::Resources::BACKGROUND);
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x,
        visibleSize.height / 2 + origin.y));
    this->addChild(background);
}
// 初始化菜单按钮
void MenuScene::initMenu() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // 创建四个主要菜单项：开始游戏、卡组构建、设置和退出
       // 设置它们的垂直间距和位置
       // 添加点击回调函数
       // 创建动画效果

    //设置偏移量
    float upOffset = 70.0f;                                 //整体性上偏移量

    //下面开始做开场箱子
    auto spriteTable = Sprite::create("menu/Table.png");
    // position the sprite on the center of the screen    
    spriteTable->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + upOffset));
    // add the sprite as a child to this layer
    this->addChild(spriteTable, 0);

    //下面开始做选项面板
    auto spriteCircle = Sprite::create("menu/Button_Circle.png");
    // position the sprite on the center of the screen
    spriteCircle->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + upOffset));
    // add the sprite as a child to this layer
    this->addChild(spriteCircle, 1);

    //下面开始做卡牌收藏处的选项卡
    auto bottomSelectionBackground = Sprite::create("menu/BottomSelection_Background.png");
    // position the sprite on the center of the screen
    bottomSelectionBackground->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - spriteTable->getContentSize().height / 2 + upOffset / 2));
    // add the sprite as a child to this layer
    this->addChild(bottomSelectionBackground, 1);

    //对战按钮
    auto playItem = MenuItemImage::create(
        "menu/Button_First_Normal.png",
        "menu/Button_First_Selected.png",
        CC_CALLBACK_1(MenuScene::onPlayGame, this));

    //酒馆战旗按钮
    auto battleItem = MenuItemImage::create(
        "menu/Button_Second_Normal.png",
        "menu/Button_Second_Selected.png",
        CC_CALLBACK_1(MenuScene::onBattleGronds, this));

    //冒险模式按钮
    auto adventureItem = MenuItemImage::create(
        "menu/Button_Third_Normal.png",
        "menu/Button_Third_Selected.png",
        CC_CALLBACK_1(MenuScene::onAdventureMode, this));

    //现为设置按钮，应该为设置按钮（内容不改，图片更换，位置更换）
    auto settingsItem = MenuItemImage::create(
        "menu/Setting_Button_Normal.png",
        "menu/Setting_Button_Selected.png",
        CC_CALLBACK_1(MenuScene::onSettings, this));

    //卡牌收藏按钮
    auto deckItem = MenuItemImage::create(
        "menu/MyCollaction_Normal.png",
        "menu/MyCollaction_Selected.png",
        CC_CALLBACK_1(MenuScene::onDeckBuilder, this));

    // 设置位置
    float spacing = 80.0f;
    playItem->setPosition(Vec2(0, spacing * 1.2f + upOffset));
    battleItem->setPosition(Vec2(0, spacing * 0.45f + upOffset));
    adventureItem->setPosition(Vec2(0, -spacing * 0.35f + upOffset));
    settingsItem->setPosition(Vec2(visibleSize.width / 2 - settingsItem->getContentSize().width / 2, -visibleSize.height / 2 + settingsItem->getContentSize().height / 2));
    Rect boundingBox = bottomSelectionBackground->getBoundingBox();              // 获取 bottomSelectionBackground 的边界矩形
    Vec2 deckItemPos = Vec2(boundingBox.getMaxX() - 50 - visibleSize.width / 2 - deckItem->getContentSize().width / 2, boundingBox.getMinY() + 30 - visibleSize.height / 2 + deckItem->getContentSize().height / 2);   // 计算 bottomSelectionBackground 右下角的位置,并合理计算其位置
    deckItem->setPosition(deckItemPos);

    // 创建菜单
    auto menu = Menu::create(playItem, battleItem, adventureItem, settingsItem, deckItem, nullptr);
    menu->setPosition(Vec2(visibleSize.width / 2 + origin.x,
        visibleSize.height / 2 + origin.y));

    this->addChild(menu, 2);

    // 播放菜单动画
    playMenuAnimation();
}
// 初始化UI元素（标题和版本号）
void MenuScene::initUI() {
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

    //关于位置的参数
    float upOffset = 70.0f;                     //整体性上偏移量
    float spacing = 80.0f;
    //添加菜单按钮文字提示
    auto playStartLable = Label::createWithTTF("Hearthstone", "fonts/Marker Felt.ttf", 24);
    if (playStartLable) {
        playStartLable->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + upOffset + spacing * 1.2f));
        playStartLable->setTextColor(Color4B(0, 0, 0, 255));
        this->addChild(playStartLable, 3);
    }

    auto battleGroundsLable = Label::createWithTTF("Battlegrounds", "fonts/Marker Felt.ttf", 24);
    if (battleGroundsLable) {
        battleGroundsLable->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + spacing * 0.45f + upOffset));
        battleGroundsLable->setTextColor(Color4B(0, 0, 0, 255));
        this->addChild(battleGroundsLable, 3);
    }

    auto adventureModeLable = Label::createWithTTF("Adventuremode", "fonts/Marker Felt.ttf", 24);
    if (adventureModeLable) {
        adventureModeLable->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - spacing * 0.35f + upOffset));
        adventureModeLable->setTextColor(Color4B(0, 0, 0, 255));
        this->addChild(adventureModeLable, 3);
    }

    // 菜单项回调函数
}

void MenuScene::onDeckBuilder(Ref* sender) {
    AudioManager::getInstance()->playEffect(GameConstants::Sound::MAIN_MENU_THREEBUTTON_CLICK);
    AudioManager::getInstance()->playEffect(GameConstants::Sound::ENTER_MYCOLLECTION);
    // TODO: 实现卡组构建场景切换

    auto scene = DeckBuilderScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));

}

void MenuScene::onSettings(Ref* sender) {
    AudioManager::getInstance()->playEffect(GameConstants::Sound::CHANGE_HELP_SCENE);

    // 切换到设置1场景
    auto scene = Settingscene1::createScene();
    Director::getInstance()->replaceScene(scene);

}

void MenuScene::onQuit(Ref* sender) {
    AudioManager::getInstance()->playEffect(GameConstants::Sound::MAIN_MENU_THREEBUTTON_CLICK);
    Director::getInstance()->end();    // 播放按钮点击音效
    // 退出游戏
}

void MenuScene::playMenuAnimation() {
    // 为菜单项添加渐入动画
    for (const auto& child : this->getChildren()) {
        child->setOpacity(0);
        child->runAction(FadeIn::create(0.5f));
    }
}

void MenuScene::onPlayGame(Ref* sender) {
    AudioManager::getInstance()->playEffect(GameConstants::Sound::MAIN_MENU_THREEBUTTON_CLICK);
    AudioManager::getInstance()->playEffect(GameConstants::Sound::CHANGE_SCENE_FROM_MAIN);
    // 切换到游戏场景
    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
}

// 直接跳到其他MenuScene的方法
void MenuScene::onEndTurnClicked(Ref* sender) {
    // 处理结束回合按钮点击
    AudioManager::getInstance()->playEffect(GameConstants::Sound::MAIN_MENU_THREEBUTTON_CLICK);
    // TODO: 添加结束回合逻辑
}

void GameScene::onSettingsClicked(Ref* sender) {
    // 处理设置按钮点击
    AudioManager::getInstance()->playEffect(GameConstants::Sound::MAIN_MENU_THREEBUTTON_CLICK);
    // TODO: 显示设置菜单
}


//以下为扩展内容，按时间条件选做
//冒险模式
void MenuScene::onAdventureMode(Ref* sender)
{
    // 处理结束回合按钮点击
    AudioManager::getInstance()->playEffect(GameConstants::Sound::MAIN_MENU_THREEBUTTON_CLICK);
    // TODO: 添加冒险模式逻辑
}

//酒馆战旗
void MenuScene::onBattleGronds(Ref* sender)
{
    // 处理结束回合按钮点击
    AudioManager::getInstance()->playEffect(GameConstants::Sound::MAIN_MENU_THREEBUTTON_CLICK);
    // TODO: 添加冒险模式逻辑
}
