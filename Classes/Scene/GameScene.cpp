// 包含所需的头文件
#include "GameScene.h"          // 游戏场景类的声明
#include "Manager/GameManager.h"   // 游戏管理器
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
    Size visibleSize = Director::getInstance()->getVisibleSize();  // 2048x1024

    // 1. 创建游戏主层和背景（居中显示）
    _gameLayer = Node::create();
    _gameLayer->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));  // (1024, 512)

    // 添加游戏层背景图片（全屏大小）
    auto gameLayerBg = Sprite::create("backgrounds/game_board.png");
    if (gameLayerBg) {
        gameLayerBg->setContentSize(Size(visibleSize.width, visibleSize.height));  // 2048x1024
        gameLayerBg->setPosition(Vec2::ZERO);  // 相对于_gameLayer中心点
        _gameLayer->addChild(gameLayerBg, -1);
    }
    this->addChild(_gameLayer);

    // 2. 创建UI层（全屏覆盖）
    _uiLayer = Node::create();
    auto uiArea = DrawNode::create();
    uiArea->drawRect(
        Vec2::ZERO,
        Vec2(2048, 1024),  // 全屏大小
        Color4F(1, 1, 1, 0.1f)  // 半透明白色边框
    );
    _uiLayer->addChild(uiArea);
    this->addChild(_uiLayer);

    // 3. 创建玩家手牌区（底部区域）
    _playerHand = Node::create();
    _playerHand->setPosition(Vec2(0, -320));  // 相对于游戏层中心向下320像素
    auto playerHandArea = DrawNode::create();
    playerHandArea->drawRect(
        Vec2(-600, -160),  // 左下角坐标
        Vec2(600, 100),    // 右上角坐标，总大小1200x260
        Color4F(0, 1, 0, 0.2f)  // 半透明绿色
    );
    _playerHand->addChild(playerHandArea);
    _gameLayer->addChild(_playerHand);

    // 4. 创建玩家场地（手牌区上方）
    _playerField = Node::create();
    _playerField->setPosition(Vec2(0, -70));  // 相对于游戏层中心向下70像素
    auto playerFieldArea = DrawNode::create();
    playerFieldArea->drawRect(
        Vec2(-450, -100),  // 左下角坐标
        Vec2(450, 100),    // 右上角坐标，总大小900x200
        Color4F(0, 0, 1, 0.2f)  // 半透明蓝色
    );
    _playerField->addChild(playerFieldArea);
    _gameLayer->addChild(_playerField);

    // 5. 创建对手区域
    _opponentHand = Node::create();
    _opponentField = Node::create();

    // 对手手牌区（顶部区域）
    auto opponentHandArea = DrawNode::create();
    opponentHandArea->drawRect(
        Vec2(-600, -160),  // 左下角坐标
        Vec2(600, 100),    // 右上角坐标，总大小1200x260
        Color4F(1, 0, 0, 0.2f)  // 半透明红色
    );
    _opponentHand->addChild(opponentHandArea);

    // 对手场地（手牌区下方）
    auto opponentFieldArea = DrawNode::create();
    opponentFieldArea->drawRect(
        Vec2(-450, -100),  // 左下角坐标
        Vec2(450, 100),    // 右上角坐标，总大小900x200
        Color4F(1, 0.5f, 0, 0.2f)  // 半透明橙色
    );
    _opponentField->addChild(opponentFieldArea);

    _opponentHand->setPosition(Vec2(0, 450));   // 相对于游戏层中心向上450像素
    _opponentField->setPosition(Vec2(0, 140));  // 相对于游戏层中心向上140像素
    _gameLayer->addChild(_opponentHand);
    _gameLayer->addChild(_opponentField);
    //可删
    addDebugLabels();
}
// 添加调试标签（可选）
// 添加调试标签（显示各区域信息）
void GameScene::addDebugLabels() {
    // 创建标准白色24号字体标签的lambda函数
    auto createLabel = [](const std::string& text) {
        auto label = Label::createWithTTF(text, "fonts/arial.ttf", 24);
        label->setTextColor(Color4B::WHITE);
        return label;
        };

    // 更新各区域的尺寸标签
    auto playerHandLabel = createLabel("Player Hand\n1200x260");  // 更新尺寸信息
    playerHandLabel->setPosition(Vec2::ZERO);
    _playerHand->addChild(playerHandLabel);

    auto playerFieldLabel = createLabel("Player Field\n900x200");  // 更新尺寸信息
    playerFieldLabel->setPosition(Vec2::ZERO);
    _playerField->addChild(playerFieldLabel);

    auto opponentHandLabel = createLabel("Opponent Hand\n1200x260");  // 更新尺寸信息
    opponentHandLabel->setPosition(Vec2::ZERO);
    _opponentHand->addChild(opponentHandLabel);

    auto opponentFieldLabel = createLabel("Opponent Field\n900x200");  // 更新尺寸信息
    opponentFieldLabel->setPosition(Vec2::ZERO);
    _opponentField->addChild(opponentFieldLabel);
}

// 初始化UI元素（按钮和标签）
void GameScene::initUI() {
    Size visibleSize = Director::getInstance()->getVisibleSize();  // 2048x1024

    // 回合结束按钮（右侧中间）
    _endTurnButton = MenuItemImage::create(
        "buttons/end_turn_normal.png",
        "buttons/end_turn_pressed.png",
        CC_CALLBACK_1(GameScene::onEndTurnClicked, this)
    );
    _endTurnButton->setPosition(Vec2(visibleSize.width - 450, visibleSize.height / 2+30));  // (1948, 512)

    // 设置按钮（左上角）
    _settingsButton = MenuItemImage::create(
        "buttons/settings_normal.png",
        "buttons/settings_pressed.png",
        CC_CALLBACK_1(GameScene::onSettingsClicked, this)
    );
    _settingsButton->setPosition(Vec2(50, visibleSize.height - 50));  // (50, 974)

    // 添加玩家英雄技能（头像右侧）
    _playerHeroPower = MenuItemImage::create(
        "heroes/hero_power_normal.png",
        "heroes/hero_power_pressed.png",
        CC_CALLBACK_1(GameScene::onHeroPowerClicked, this)
    );
    _playerHeroPower->setPosition(Vec2(visibleSize.width / 2 + 170, 250));  // 头像右侧100像素
    _playerHeroPower->setScale(0.7f);

    // 更新菜单，包含英雄技能按钮
    cocos2d::Menu* menu = Menu::create(_endTurnButton, _settingsButton, _playerHeroPower, nullptr);
    if (menu) {
        menu->setPosition(Vec2::ZERO);
        _uiLayer->addChild(menu);
    }

    // 回合计时器标签（回合结束按钮上方）
    _turnTimerLabel = Label::createWithTTF("90", "fonts/arial.ttf", 32);
    _turnTimerLabel->setPosition(Vec2(visibleSize.width - 450, visibleSize.height / 2 + 80));  // (1948, 562)
    _uiLayer->addChild(_turnTimerLabel);

    // 玩家信息标签（左下角）
    _playerManaLabel = Label::createWithTTF("0/0", "fonts/arial.ttf", 24);     // 法力值
    _playerHealthLabel = Label::createWithTTF("30", "fonts/arial.ttf", 24);    // 生命值
    _playerManaLabel->setPosition(Vec2(100, 100));      // (100, 100)
    _playerHealthLabel->setPosition(Vec2(100, 150));    // (100, 150)
    _uiLayer->addChild(_playerManaLabel);
    _uiLayer->addChild(_playerHealthLabel);

    // 添加玩家头像（正下方）
    auto playerHeroPortrait = Sprite::create("heroes/player_portrait.png");
    if (playerHeroPortrait) {
        playerHeroPortrait->setPosition(Vec2(visibleSize.width / 2-8, 265));  // x=1024, y=120
        playerHeroPortrait->setScale(0.8f);
        _uiLayer->addChild(playerHeroPortrait);

        // 玩家生命值显示（头像下方）
        _playerHealthLabel = Label::createWithTTF("30", "fonts/arial.ttf", 24);
        _playerHealthLabel->setPosition(Vec2(visibleSize.width / 2, 80));
        _uiLayer->addChild(_playerHealthLabel);
    }




    // 添加对手头像（正上方）
    auto opponentHeroPortrait = Sprite::create("heroes/opponent_portrait.png");
    if (opponentHeroPortrait) {
        opponentHeroPortrait->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 120));  // x=1024, y=904
        opponentHeroPortrait->setScale(0.8f);
        _uiLayer->addChild(opponentHeroPortrait);

        // 对手生命值显示（头像上方）
        auto opponentHealth = Label::createWithTTF("30", "fonts/arial.ttf", 24);
        opponentHealth->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 80));
        _uiLayer->addChild(opponentHealth);
    }

    // 添加对手英雄技能（头像右侧）
    auto opponentHeroPower = Sprite::create("heroes/hero_power_disabled.png");
    if (opponentHeroPower) {
        opponentHeroPower->setPosition(Vec2(visibleSize.width / 2 + 100, visibleSize.height - 120));
        opponentHeroPower->setScale(0.8f);
        _uiLayer->addChild(opponentHeroPower);
    }

    // 玩家法力水晶显示（头像左侧）
    _playerManaLabel = Label::createWithTTF("0/0", "fonts/arial.ttf", 24);
    _playerManaLabel->setPosition(Vec2(visibleSize.width / 2 - 100, 120));
    _uiLayer->addChild(_playerManaLabel);

    // 对手法力水晶显示（头像左侧）
    auto opponentMana = Label::createWithTTF("0/0", "fonts/arial.ttf", 24);
    opponentMana->setPosition(Vec2(visibleSize.width / 2 - 100, visibleSize.height - 120));
    _uiLayer->addChild(opponentMana);

    // 初始化回合相关变量
    _isPlayerTurn = true;  // 游戏开始时是玩家回合
    _turnTimeRemaining = TURN_TIME;

    // 启动回合计时器
    this->schedule(CC_SCHEDULE_SELECTOR(GameScene::updateTurnTimer), 1.0f);
}
// 开始新回合
void GameScene::startTurn() {
    _turnTimeRemaining = TURN_TIME;  // 重置计时器
    _isPlayerTurn = !_isPlayerTurn;  // 切换回合

    // 更新UI显示
    _endTurnButton->setEnabled(_isPlayerTurn);  // 只有玩家回合才能点击结束按钮

    if (_isPlayerTurn) {
        // 玩家回合开始时的操作
        _endTurnButton->setColor(Color3B::WHITE);  // 按钮高亮

        // TODO: 增加玩家的法力水晶
        // TODO: 抽一张牌
        // TODO: 播放回合开始动画

    }
    else {
        // 对手回合开始时的操作
        _endTurnButton->setColor(Color3B::GRAY);   // 按钮变灰

        // TODO: 启动AI决策
        // TODO: 播放对手回合开始动画
    }
}

// 结束当前回合
void GameScene::endTurn() {
    // 播放结束回合动画
    auto endTurnEffect = Sprite::create("effects/end_turn.png");
    if (endTurnEffect) {
        endTurnEffect->setPosition(Director::getInstance()->getVisibleSize() / 2);
        this->addChild(endTurnEffect);

        // 动画完成后删除特效并开始新回合
        auto sequence = Sequence::create(
            FadeIn::create(0.2f),
            DelayTime::create(0.5f),
            FadeOut::create(0.2f),
            CallFunc::create([this, endTurnEffect]() {
                endTurnEffect->removeFromParent();
                this->startTurn();
                }),
            nullptr
        );
        endTurnEffect->runAction(sequence);
    }
    else {
        // 如果没有特效图片，直接开始新回合
        startTurn();
    }
}

// 更新回合计时器
void GameScene::updateTurnTimer(float dt) {
    if (_turnTimeRemaining > 0) {
        _turnTimeRemaining--;
        _turnTimerLabel->setString(std::to_string(_turnTimeRemaining));

        // 时间不多时改变颜色提醒
        if (_turnTimeRemaining <= 10) {
            _turnTimerLabel->setTextColor(Color4B::RED);
        }
        else {
            _turnTimerLabel->setTextColor(Color4B::WHITE);
        }

        // 时间耗尽时自动结束回合
        if (_turnTimeRemaining == 0 && _isPlayerTurn) {
            endTurn();
        }
    }
}
void GameScene::onHeroPowerClicked(Ref* sender) {
    if (_isPlayerTurn) {  // 只在玩家回合可用
        // TODO: 检查法力值是否足够
        // TODO: 实现英雄技能效果
        // TODO: 扣除法力值
        // TODO: 播放技能动画
        CCLOG("Hero power clicked!");
    }
}
