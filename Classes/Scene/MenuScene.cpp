#include "MenuScene.h"
#include "GameScene.h"
#include "Utils/Constants.h"
#include "Audio/AudioManager.h"

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
    auto playItem = MenuItemImage::create(
        "menu/play_normal.png",
        "menu/play_selected.png",
        CC_CALLBACK_1(MenuScene::onPlayGame, this));

    auto deckItem = MenuItemImage::create(
        "menu/deck_normal.png",
        "menu/deck_selected.png",
        CC_CALLBACK_1(MenuScene::onDeckBuilder, this));

    auto settingsItem = MenuItemImage::create(
        "menu/settings_normal.png",
        "menu/settings_selected.png",
        CC_CALLBACK_1(MenuScene::onSettings, this));

    auto quitItem = MenuItemImage::create(
        "menu/quit_normal.png",
        "menu/quit_selected.png",
        CC_CALLBACK_1(MenuScene::onQuit, this));

    // 设置位置
    float spacing = 80.0f;
    playItem->setPosition(Vec2(0, spacing * 1.5f));
    deckItem->setPosition(Vec2(0, spacing * 0.5f));
    settingsItem->setPosition(Vec2(0, -spacing * 0.5f));
    quitItem->setPosition(Vec2(0, -spacing * 1.5f));

    // 创建菜单
    auto menu = Menu::create(playItem, deckItem, settingsItem, quitItem, nullptr);
    menu->setPosition(Vec2(visibleSize.width / 2 + origin.x,
        visibleSize.height / 2 + origin.y));

    this->addChild(menu);

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
        versionLabel->setPosition(Vec2(origin.x + visibleSize.width - 100,
            origin.y + 30));
        this->addChild(versionLabel, 1);
    }
    // 菜单项回调函数
}void MenuScene::onDeckBuilder(Ref* sender) {
    AudioManager::getInstance()->playEffect(GameConstants::Sound::BUTTON_CLICK);
    // TODO: 实现卡组构建场景切换
}

void MenuScene::onSettings(Ref* sender) {
    AudioManager::getInstance()->playEffect(GameConstants::Sound::BUTTON_CLICK);
    // TODO: 实现设置场景切换
}

void MenuScene::onQuit(Ref* sender) {
    AudioManager::getInstance()->playEffect(GameConstants::Sound::BUTTON_CLICK);
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
    AudioManager::getInstance()->playEffect(GameConstants::Sound::BUTTON_CLICK);

    // 切换到游戏场景
    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
}
void GameScene::initListeners() {
    // 初始化事件监听器
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [](Touch* touch, Event* event) {
        return true;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameScene::updateUI(float dt) {
    // 更新UI逻辑
    // 这里可以更新生命值、法力水晶等显示
}

void GameScene::onEndTurnClicked(Ref* sender) {
    // 处理结束回合按钮点击
    AudioManager::getInstance()->playEffect(GameConstants::Sound::BUTTON_CLICK);
    // TODO: 添加结束回合逻辑
}

void GameScene::onSettingsClicked(Ref* sender) {
    // 处理设置按钮点击
    AudioManager::getInstance()->playEffect(GameConstants::Sound::BUTTON_CLICK);
    // TODO: 显示设置菜单
}