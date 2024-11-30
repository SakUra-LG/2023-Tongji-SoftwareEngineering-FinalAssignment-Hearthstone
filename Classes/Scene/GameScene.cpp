// 1. 包含头文件和命名空间
#include "GameScene.h"          // 游戏场景类的声明
#include "Manager/GameManager.h"   // 游戏管理器
#include "Card/Card.h"          // 卡牌类
#include "Player/Player.h"      // 玩家类
#include "Utils/Constants.h"    // 游戏常量
#include "Animation/AnimationManager.h"  // 动画管理器

// 使用 cocos2d 命名空间
USING_NS_CC;

// 2. 场景创建和初始化
Scene* GameScene::createScene() {
    return GameScene::create();  // 创建并返回场景实例
}

// 3. 初始化相关方法
bool GameScene::init() {
    // 调用父类的初始化方法
    if (!Scene::init()) {
        return false;  // 如果父类初始化失败，返回false
    }

    // 初始化场景的各个组件
    initLayers();     // 初始化游戏层级
    initUI();         // 初始化用户界面
    initListeners();  // 初始化事件监听器
    initGame();  // 初始化游戏状态和发牌

    // 启动UI更新定时器，每秒调用一次updateUI方法
    this->schedule(CC_SCHEDULE_SELECTOR(GameScene::updateUI), 1.0f);
    return true;  // 初始化成功
}

void GameScene::initGame() {
    // 发初始手牌
    for (int i = 0; i < 3; i++) {  // 初始3张手牌
        createCardSprite("card_" + std::to_string(i + 1), true);   // 玩家手牌
        createCardSprite("card_" + std::to_string(i + 1), false);  // 对手手牌
    }
}

// 初始化游戏层级
void GameScene::initLayers() {
    Size visibleSize = Director::getInstance()->getVisibleSize();  // 1920x1080

    // 1. 创建游戏主层和背景（居中显示）
    _gameLayer = Node::create();
    _gameLayer->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));  // (1024, 512)

    // 添加游戏层背景图片（全屏大小）
    auto gameLayerBg = Sprite::create("backgrounds/game_board.png");
    if (gameLayerBg) {
        gameLayerBg->setContentSize(Size(visibleSize.width, visibleSize.height));  // 1920x1080
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
        Vec2(-550, -100),  // 左下角坐标
        Vec2(550, 100),    // 右上角坐标，总大小900x200
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
        Vec2(-550, -100),  // 左下角坐标
        Vec2(550, 100),    // 右上角坐标，总大小1100x200
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

    auto playerFieldLabel = createLabel("Player Field\n1100x200");  // 更新尺寸信息
    playerFieldLabel->setPosition(Vec2::ZERO);
    _playerField->addChild(playerFieldLabel);

    auto opponentHandLabel = createLabel("Opponent Hand\n1200x260");  // 更新尺寸信息
    opponentHandLabel->setPosition(Vec2::ZERO);
    _opponentHand->addChild(opponentHandLabel);

    auto opponentFieldLabel = createLabel("Opponent Field\n1100x200");  // 更新尺寸信息
    opponentFieldLabel->setPosition(Vec2::ZERO);
    _opponentField->addChild(opponentFieldLabel);
}

// 初始化UI元素（按钮和标签）
void GameScene::initUI() {
    Size visibleSize = Director::getInstance()->getVisibleSize();  // 2048x1024

    // 回合结束按钮（右侧中间）
    _endTurnButton = MenuItemImage::create(
        "buttons/Button_For_EndTheTurn_Normal.png",
        "buttons/Button_For_EndTheTurn_Selected.png",
        CC_CALLBACK_1(GameScene::onEndTurnClicked, this)
    );
    _endTurnButton->setPosition(Vec2(visibleSize.width - 275, visibleSize.height / 2+30));  // (1948, 512)

    // 设置按钮（左上角）
    _settingsButton = MenuItemImage::create(
        "buttons/Setting_Button_Normal.png",
        "buttons/Setting_Button_Selected.png",
        CC_CALLBACK_1(GameScene::onSettingsClicked, this)
    );
    _settingsButton->setPosition(Vec2(visibleSize.width- _settingsButton->getContentSize().width/2, _settingsButton->getContentSize().height / 2));  

    // 添加玩家英雄技能（头像右侧）
    _playerHeroPower = MenuItemImage::create(
        "heroes/hero_power_normal.png",
        "heroes/hero_power_pressed.png",
        CC_CALLBACK_1(GameScene::onHeroPowerClicked, this)
    );
    _playerHeroPower->setPosition(Vec2(visibleSize.width / 2 + 185, 250));  // 头像右侧100像素
    _playerHeroPower->setScale(0.7f);

    // 更新菜单，包含英雄技能按钮
    cocos2d::Menu* menu = Menu::create(_endTurnButton, _settingsButton, _playerHeroPower, nullptr);
    if (menu) {
        menu->setPosition(Vec2::ZERO);
        _uiLayer->addChild(menu);
    }

    // 回合计时器标签（回合结束按钮上方）
    _turnTimerLabel = Label::createWithTTF("90", "fonts/arial.ttf", 32);
    _turnTimerLabel->setPosition(Vec2(visibleSize.width - 275, visibleSize.height / 2 + 80));  // (1948, 562)
    _uiLayer->addChild(_turnTimerLabel);

    // 玩家信息标签（左下角）
    _playerManaLabel = Label::createWithTTF("0/0", "fonts/arial.ttf", 24);     // 法力值
    _playerHealthLabel = Label::createWithTTF("30", "fonts/arial.ttf", 35);    // 生命值
    _playerManaLabel->setPosition(Vec2(1320, 95));      // (100, 100)
    _playerHealthLabel->setPosition(Vec2(visibleSize.width / 2 + 60, 200));  
    //_playerManaLabel->setTextColor(Color4B(0, 0, 0, 255));
    //_playerHealthLabel->setTextColor(Color4B(0, 0, 0, 255));
    _uiLayer->addChild(_playerManaLabel,2);
    _uiLayer->addChild(_playerHealthLabel,2);

    // 添加玩家头像（正下方）
    auto playerHeroPortrait = Sprite::create("heroes/player_portrait.png");
    if (playerHeroPortrait) {
        playerHeroPortrait->setPosition(Vec2(visibleSize.width / 2+20, 265));  // x=1024, y=120
        playerHeroPortrait->setScale(0.8f);
        _uiLayer->addChild(playerHeroPortrait,0);

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
        opponentHealth->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 95));
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
    opponentMana->setPosition(Vec2(1280, visibleSize.height - 95));
    _uiLayer->addChild(opponentMana);

    // 初始化回合相关变量
    _isPlayerTurn = true;  // 游戏开始时是玩家回合
    _turnTimeRemaining = TURN_TIME;

    // 启动回合计时器
    this->schedule(CC_SCHEDULE_SELECTOR(GameScene::updateTurnTimer), 1.0f);

    //添加卡牌库里的卡牌图片（结束按键上下两侧，双方都有）
    float offset = 135.0f;                                          //卡牌库相对于结束回合按钮的偏移量y方向
    auto playerCardsLibrary = Sprite::create("Cards/Cards_Library.png");
    playerCardsLibrary->setPosition(Vec2(visibleSize.width - 185, visibleSize.height / 2 + 30- offset));
    _uiLayer->addChild(playerCardsLibrary, 1);

    auto opponentCardsLibrary = Sprite::create("Cards/Cards_Library.png");
    opponentCardsLibrary->setPosition(Vec2(visibleSize.width - 185, visibleSize.height / 2 + 30 + offset));
    _uiLayer->addChild(opponentCardsLibrary, 1);

}
// 4. 卡牌相关方法
// 创建卡牌精灵
void GameScene::createCardSprite(const std::string& cardName, bool isPlayerCard) {
    Size visibleSize = Director::getInstance()->getVisibleSize();

    // 创建卡牌精灵
    auto cardSprite = Sprite::create(isPlayerCard ?
        "cards/" + cardName + ".png" : "cards/card_back.png");
    if (!cardSprite) {
        CCLOG("Failed to create card sprite: %s", cardName.c_str());
        return;
    }

    // 设置卡牌基本属性
    cardSprite->setScale(0.7f);  // 与英雄技能大小一致

    // 设置初始位置（从牌库位置开始）
    Vec2 startPos;
    if (isPlayerCard) {
        startPos = Vec2(visibleSize.width - 100, -320);  // 从玩家牌库位置开始
        _playerHandCards.push_back(cardSprite);
        _playerHand->addChild(cardSprite);
    }
    else {
        startPos = Vec2(-100, 450);  // 从对手牌库位置开始
        _opponentHandCards.push_back(cardSprite);
        _opponentHand->addChild(cardSprite);
    }
    cardSprite->setPosition(startPos);

    // 重新排列手牌
    arrangeHandCards(isPlayerCard);
}

// 重新排列手牌
void GameScene::arrangeHandCards(bool isPlayerHand) {
    auto& cards = isPlayerHand ? _playerHandCards : _opponentHandCards;
    int cardCount = cards.size();

    // 计算起始X坐标，使卡牌居中
    float startX = -((cardCount - 1) * 80.0f / 2);  // 80.0f 是卡牌间距

    // 为每张卡牌创建移动动画
    for (int i = 0; i < cardCount; i++) {
        Vec2 targetPos = Vec2(startX + i * 80.0f, 0);

        // 添加弧线动画
        ccBezierConfig bezier;
        bezier.controlPoint_1 = Vec2(targetPos.x, targetPos.y + 100);
        bezier.controlPoint_2 = Vec2(targetPos.x, targetPos.y + 100);
        bezier.endPosition = targetPos;

        auto bezierTo = BezierTo::create(0.5f, bezier);
        auto spawn = Spawn::create(
            bezierTo,
            ScaleTo::create(0.5f, 0.7f),  // 确保最终大小一致
            nullptr
        );

        cards[i]->runAction(spawn);

        // 如果是玩家卡牌，添加交互功能
        if (isPlayerHand) {
            addCardInteraction(cards[i]);
        }
    }
}

// 添加卡牌交互
void GameScene::addCardInteraction(cocos2d::Sprite* cardSprite) {
    auto listener = EventListenerTouchOneByOne::create();

    // 触摸开始
    listener->onTouchBegan = [this, cardSprite](Touch* touch, Event* event) {
        Vec2 touchPos = cardSprite->getParent()->convertToNodeSpace(touch->getLocation());
        Rect bounds = cardSprite->getBoundingBox();

        if (bounds.containsPoint(touchPos)) {
            // 卡牌被点击时上浮并放大
            cardSprite->runAction(Spawn::create(
                MoveBy::create(0.1f, Vec2(0, 30)),
                ScaleTo::create(0.1f, 0.8f),
                nullptr
            ));
            return true;
        }
        return false;
        };

    // 触摸结束
    listener->onTouchEnded = [this, cardSprite](Touch* touch, Event* event) {
        // 卡牌回到原位
        cardSprite->runAction(Spawn::create(
            MoveBy::create(0.1f, Vec2(0, -30)),
            ScaleTo::create(0.1f, 0.7f),
            nullptr
        ));

        // TODO: 处理卡牌打出逻辑
        CCLOG("Card played!");
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, cardSprite);
}
// 5. 回合控制相关方法
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
// 6. UI更新和事件处理方法
void GameScene::onHeroPowerClicked(Ref* sender) {
    auto gameManager = GameManager::getInstance();
    if (gameManager->canUseHeroPower(gameManager->getCurrentPlayer())) {
        if (gameManager->useHeroPower(gameManager->getCurrentPlayer())) {
            // 播放技能动画
            auto heroPowerEffect = Sprite::create("effects/hero_power.png");
            if (heroPowerEffect) {
                heroPowerEffect->setPosition(_playerHeroPower->getPosition());
                _uiLayer->addChild(heroPowerEffect);

                auto sequence = Sequence::create(
                    FadeIn::create(0.2f),
                    DelayTime::create(0.5f),
                    FadeOut::create(0.2f),
                    CallFunc::create([heroPowerEffect]() {
                        heroPowerEffect->removeFromParent();
                        }),
                    nullptr
                );
                heroPowerEffect->runAction(sequence);
            }

            // 更新UI（添加dt参数）
            updateUI(0.0f);
        }
    }
    else {
        // 可以添加提示音效或视觉效果表示无法使用
        CCLOG("Cannot use hero power now!");
    }
}
void GameScene::updateUI(float dt) {
    // 获取游戏管理器和当前玩家
    auto gameManager = GameManager::getInstance();
    auto currentPlayer = gameManager->getCurrentPlayer();
    auto opponentPlayer = gameManager->getOpponentPlayer();

    if (!currentPlayer || !opponentPlayer) return;

    // 1. 更新玩家信息
    // 生命值
    _playerHealthLabel->setString(std::to_string(currentPlayer->getHealth()));
    if (currentPlayer->getHealth() <= 10) {
        _playerHealthLabel->setTextColor(Color4B::RED);
    }

    // 法力值
    std::string manaText = std::to_string(currentPlayer->getMana()) + "/" +
        std::to_string(currentPlayer->getMaxMana());
    _playerManaLabel->setString(manaText);

    // 2. 更新对手信息
    // 生命值
    if (_opponentHealthLabel) {
        _opponentHealthLabel->setString(std::to_string(opponentPlayer->getHealth()));
        if (opponentPlayer->getHealth() <= 10) {
            _opponentHealthLabel->setTextColor(Color4B::RED);
        }
    }

    // 法力值
    if (_opponentManaLabel) {
        std::string oppManaText = std::to_string(opponentPlayer->getMana()) + "/" +
            std::to_string(opponentPlayer->getMaxMana());
        _opponentManaLabel->setString(oppManaText);
    }

    // 3. 更新回合状态
    _endTurnButton->setEnabled(_isPlayerTurn);
    _endTurnButton->setColor(_isPlayerTurn ? Color3B::WHITE : Color3B::GRAY);

    // 4. 更新英雄技能状态
    bool canUseHeroPower = gameManager->canUseHeroPower(currentPlayer);
    _playerHeroPower->setEnabled(canUseHeroPower && _isPlayerTurn);
    _playerHeroPower->setColor(canUseHeroPower && _isPlayerTurn ? Color3B::WHITE : Color3B::GRAY);

    // 5. 更新手牌显示
    updateHandPositions();

    // 6. 更新场上随从
    updateFieldPositions();

    // 7. 检查游戏状态
    if (gameManager->isGameOver()) {  // 使用提供的 isGameOver() 方法
        showGameOverUI();
    }
}

void GameScene::updateHandPositions() {
    // 更新玩家手牌位置
    for (size_t i = 0; i < _playerHandCards.size(); ++i) {
        auto card = _playerHandCards[i];
        if (!card->getNumberOfRunningActions()) {  // 只在没有运行动画时更新
            float startX = -(((_playerHandCards.size() - 1) * CARD_SPACING) / 2);
            card->setPosition(Vec2(startX + i * CARD_SPACING, 0));
        }
    }

    // 更新对手手牌位置
    for (size_t i = 0; i < _opponentHandCards.size(); ++i) {
        auto card = _opponentHandCards[i];
        if (!card->getNumberOfRunningActions()) {
            float startX = -(((_opponentHandCards.size() - 1) * CARD_SPACING) / 2);
            card->setPosition(Vec2(startX + i * CARD_SPACING, 0));
        }
    }
}

void GameScene::updateFieldPositions() {
    auto gameManager = GameManager::getInstance();
    auto currentPlayer = gameManager->getCurrentPlayer();
    auto opponentPlayer = gameManager->getOpponentPlayer();

    // 更新玩家场上随从
    const auto& playerField = currentPlayer->getField();
    float startX = -((playerField.size() - 1) * CARD_SPACING) / 2;
    for (size_t i = 0; i < playerField.size(); ++i) {
        auto card = playerField[i];
        if (!card) continue;

        auto minionSprite = card->getSprite();
        if (minionSprite) {
            Vec2 targetPos = Vec2(startX + i * CARD_SPACING, 0);
            if (minionSprite->getPosition() != targetPos) {
                minionSprite->runAction(MoveTo::create(0.3f, targetPos));
            }
        }
    }

    // 更新对手场上随从
    const auto& opponentField = opponentPlayer->getField();
    startX = -((opponentField.size() - 1) * CARD_SPACING) / 2;
    for (size_t i = 0; i < opponentField.size(); ++i) {
        auto card = opponentField[i];
        if (!card) continue;

        auto minionSprite = card->getSprite();
        if (minionSprite) {
            Vec2 targetPos = Vec2(startX + i * CARD_SPACING, 0);
            if (minionSprite->getPosition() != targetPos) {
                minionSprite->runAction(MoveTo::create(0.3f, targetPos));
            }
        }
    }
}
void GameScene::showGameOverUI() {
    auto gameManager = GameManager::getInstance();
    bool playerWon = gameManager->getCurrentPlayer()->getHealth() > 0;

    // 创建半透明背景
    auto dimLayer = LayerColor::create(Color4B(0, 0, 0, 128));
    this->addChild(dimLayer, 100);

    // 创建游戏结束标签
    auto winLabel = Label::createWithTTF(
        playerWon ? "Victory!" : "Defeat",
        "fonts/arial.ttf",
        72
    );
    winLabel->setPosition(Director::getInstance()->getVisibleSize() / 2);
    winLabel->setTextColor(playerWon ? Color4B::YELLOW : Color4B::RED);
    dimLayer->addChild(winLabel);

    // 添加返回按钮
    auto backButton = MenuItemImage::create(
        "buttons/back_normal.png",
        "buttons/back_pressed.png",
        [](Ref* sender) {
            Director::getInstance()->popScene();
        }
    );
    auto menu = Menu::create(backButton, nullptr);
    menu->setPosition(Vec2(
        Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 2 - 100
    ));
    dimLayer->addChild(menu);
}
