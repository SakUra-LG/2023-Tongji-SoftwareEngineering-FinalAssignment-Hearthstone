// 包含所需的头文件
#include "GameScene.h"          // 游戏场景类的声明
#include "Game/GameManager.h"   // 游戏管理器
#include "Card/Card.h"          // 卡牌类
#include "Player/Player.h"      // 玩家类
#include "Utils/Constants.h"    // 游戏常量
#include "Animation/AnimationManager.h"  // 动画管理器

// 使用 cocos2d 命名空间
USING_NS_CC;

// 创建场景的静态方法
Scene* GameScene::createScene() {
    return GameScene::create();  // 创建并返回场景实例
}

// 场景初始化方法
bool GameScene::init() {
    // 调用父类的初始化方法
    if (!Scene::init()) {
        return false;  // 如果父类初始化失败，返回false
    }

    // 初始化场景的各个组件
    initLayers();     // 初始化游戏层级
    initUI();         // 初始化用户界面
    initListeners();  // 初始化事件监听器

    // 启动UI更新定时器，每秒调用一次updateUI方法
    this->schedule(CC_SCHEDULE_SELECTOR(GameScene::updateUI), 1.0f);

    return true;  // 初始化成功
}

// 初始化游戏层级
void GameScene::initLayers() {
    // 获取屏幕可见区域大小
    Size visibleSize = Director::getInstance()->getVisibleSize();

    // 创建游戏主层
    _gameLayer = Node::create();  // 创建游戏层节点
    _gameLayer->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));  // 设置位置为屏幕中心
    this->addChild(_gameLayer);   // 将游戏层添加到场景

    // 创建UI层
    _uiLayer = Node::create();    // 创建UI层节点
    this->addChild(_uiLayer);     // 将UI层添加到场景

    // 创建玩家手牌区
    _playerHand = Node::create();  // 创建玩家手牌节点
    _playerHand->setPosition(Vec2(visibleSize.width / 2, 100));  // 设置位置在底部
    _gameLayer->addChild(_playerHand);  // 添加到游戏层

    // 创建玩家场地
    _playerField = Node::create();  // 创建玩家场地节点
    _playerField->setPosition(Vec2(visibleSize.width / 2, 300));  // 设置位置
    _gameLayer->addChild(_playerField);  // 添加到游戏层

    // 创建对手手牌区和场地
    _opponentHand = Node::create();    // 创建对手手牌节点
    _opponentField = Node::create();   // 创建对手场地节点
    // 设置对手区域位置（在屏幕上方）
    _opponentHand->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 100));
    _opponentField->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 300));
    _gameLayer->addChild(_opponentHand);    // 添加对手手牌区到游戏层
    _gameLayer->addChild(_opponentField);   // 添加对手场地到游戏层
}

// 初始化UI元素
void GameScene::initUI() {
    // 获取屏幕可见区域大小
    Size visibleSize = Director::getInstance()->getVisibleSize();

    // 创建回合结束按钮
    _endTurnButton = MenuItemImage::create(
        "buttons/end_turn_normal.png",      // 按钮正常状态的图片
        "buttons/end_turn_pressed.png",     // 按钮按下状态的图片
        CC_CALLBACK_1(GameScene::onEndTurnClicked, this)  // 点击回调函数
    );
    // 设置回合结束按钮位置
    _endTurnButton->setPosition(Vec2(visibleSize.width - 100, visibleSize.height / 2));

    // 创建设置按钮
    _settingsButton = MenuItemImage::create(
        "buttons/settings_normal.png",      // 按钮正常状态的图片
        "buttons/settings_pressed.png",     // 按钮按下状态的图片
        CC_CALLBACK_1(GameScene::onSettingsClicked, this)  // 点击回调函数
    );
    // 设置设置按钮位置
    _settingsButton->setPosition(Vec2(50, visibleSize.height - 50));

    // 创建包含所有按钮的菜单
    auto menu = Menu::create(_endTurnButton, _settingsButton, nullptr);
    menu->setPosition(Vec2::ZERO);  // 设置菜单位置为原点
    _uiLayer->addChild(menu);       // 将菜单添加到UI层

    // 创建回合计时器标签
    _turnTimerLabel = Label::createWithTTF("90", "fonts/arial.ttf", 32);  // 创建计时器文本
    _turnTimerLabel->setPosition(Vec2(visibleSize.width - 100, visibleSize.height / 2 + 50));  // 设置位置
    _uiLayer->addChild(_turnTimerLabel);  // 添加到UI层

    // 创建玩家信息标签
    _playerManaLabel = Label::createWithTTF("0/0", "fonts/arial.ttf", 24);     // 创建法力值标签
    _playerHealthLabel = Label::createWithTTF("30", "fonts/arial.ttf", 24);    // 创建生命值标签
    _playerManaLabel->setPosition(Vec2(100, 100));     // 设置法力值标签位置
    _playerHealthLabel->setPosition(Vec2(100, 150));   // 设置生命值标签位置
    _uiLayer->addChild(_playerManaLabel);     // 添加法力值标签到UI层
    _uiLayer->addChild(_playerHealthLabel);   // 添加生命值标签到UI层
}