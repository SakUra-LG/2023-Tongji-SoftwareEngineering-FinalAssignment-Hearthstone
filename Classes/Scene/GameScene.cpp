// 1. 包含头文件和命名空间
#include "GameScene.h"          // 游戏场景类的声明
#include "Manager/GameManager.h"   // 游戏管理器
#include "Card/Card.h"          // 卡牌类
#include "Player/Player.h"      // 玩家类
#include "Utils/Constants.h"    // 游戏常量
#include "Animation/AnimationManager.h"  // 动画管理器
#include "Card/DeckManager.h"  // 添加这行
#include "Utils/GameLogger.h"  // 修正路径
#pragma execution_character_set("utf-8")
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
    auto logger = GameLogger::getInstance();
    logger->log(LogLevel::INFO, "Starting game initialization");
    
    // 初始化玩家
    _currentPlayer = new Player();
    if (_playerDeck) {
        logger->log(LogLevel::INFO, "Setting player deck");
        _currentPlayer->setDeck(_playerDeck->getCards());
    }
    
    // 发起始手牌
    logger->log(LogLevel::INFO, "Drawing initial cards");
    drawInitialHand();
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
    _handLayer = Node::create();
    _handLayer->setPosition(Vec2(0, -320));
    auto playerHandArea = DrawNode::create();
    playerHandArea->drawRect(
        Vec2(-600, -160),
        Vec2(600, 100),
        Color4F(0, 1, 0, 0.2f)
    );
    _handLayer->addChild(playerHandArea);
    _gameLayer->addChild(_handLayer);

    // 4. 创建玩家场地（手牌区上方）
    _playerField = Node::create();
    _playerField->setPosition(Vec2(0, -70));  // 相对于游戏层中心向下70像素

    // 添加可视化区域（调试用）
    auto playerFieldArea = DrawNode::create();
    playerFieldArea->drawRect(
        Vec2(-550, -100),  // 左下角坐标
        Vec2(550, 100),    // 右上角坐标，总大小1100x200
        Color4F(0, 0, 1, 0.2f)  // 半透明蓝色
    );
    _playerField->addChild(playerFieldArea);
    _gameLayer->addChild(_playerField);

    CCLOG("Player field initialized at y = %f", _playerField->getPositionY());

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
// 添加调试标签（示各区域信息）
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
    _handLayer->addChild(playerHandLabel);

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

// 初始化UI元素（按钮标签）
void GameScene::initUI() {
    Size visibleSize = Director::getInstance()->getVisibleSize();  // 2048x1024

    // 回合结束按钮（右侧中间）
    _endTurnButton = MenuItemImage::create(
        "buttons/Button_For_EndTheTurn_Normal.png",
        "buttons/Button_For_EndTheTurn_Selected.png",
        CC_CALLBACK_1(GameScene::onEndTurnClicked, this)
    );
    _endTurnButton->setPosition(Vec2(visibleSize.width - 275, visibleSize.height / 2 + 30));  // (1948, 512)

    // 设置按钮（左上角）->现因事件原因，效果改为退出游戏
    _settingsButton = MenuItemImage::create(
        "buttons/Setting_Button_Normal.png",
        "buttons/Setting_Button_Selected.png",
        CC_CALLBACK_1(GameScene::onSettingsClicked, this)
    );
    _settingsButton->setPosition(Vec2(visibleSize.width - _settingsButton->getContentSize().width / 2, _settingsButton->getContentSize().height / 2));

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
    _turnTimerLabel = Label::createWithTTF("30", "fonts/arial.ttf", 32);
    _turnTimerLabel->setPosition(Vec2(visibleSize.width - 275, visibleSize.height / 2 + 80));  // (1948, 562)
    _uiLayer->addChild(_turnTimerLabel);

    // 玩家信息标签（左下角）
    _playerManaLabel = Label::createWithTTF("0/0", "fonts/arial.ttf", 24);     // 法力值
    _playerHealthLabel = Label::createWithTTF("30", "fonts/arial.ttf", 35);    // 生命值
    _playerManaLabel->setPosition(Vec2(1320, 95));      // (100, 100)
    _playerHealthLabel->setPosition(Vec2(visibleSize.width / 2 + 60, 200));
    //_playerManaLabel->setTextColor(Color4B(0, 0, 0, 255));
    //_playerHealthLabel->setTextColor(Color4B(0, 0, 0, 255));
    _uiLayer->addChild(_playerManaLabel, 2);
    _uiLayer->addChild(_playerHealthLabel, 2);

    // 添加玩家头像（正下方）
    auto playerHeroPortrait = Sprite::create("heroes/player_portrait.png");
    if (playerHeroPortrait) {
        playerHeroPortrait->setPosition(Vec2(visibleSize.width / 2 + 20, 265));  // x=1024, y=120
        playerHeroPortrait->setScale(0.8f);
        _uiLayer->addChild(playerHeroPortrait, 0);

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

    //添加卡牌库里的卡牌图片（结束键上下两侧，双方都有）
    float offset = 135.0f;                                          //卡牌库相对于结束回合按钮的偏移量y向
    auto playerCardsLibrary = Sprite::create("Cards/Cards_Library.png");
    playerCardsLibrary->setPosition(Vec2(visibleSize.width - 185, visibleSize.height / 2 + 30 - offset));
    _uiLayer->addChild(playerCardsLibrary, 1);

    auto opponentCardsLibrary = Sprite::create("Cards/Cards_Library.png");
    opponentCardsLibrary->setPosition(Vec2(visibleSize.width - 185, visibleSize.height / 2 + 30 + offset));
    _uiLayer->addChild(opponentCardsLibrary, 1);

    // 添加回合指示器
    _turnIndicatorLabel = Label::createWithTTF("Your Turn", "fonts/arial.ttf", 36);
    _turnIndicatorLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    _turnIndicatorLabel->setTextColor(Color4B::GREEN);
    _uiLayer->addChild(_turnIndicatorLabel);
}
// 4. 卡牌相关方法
// 重新排列手牌
void GameScene::arrangeHandCards(bool isPlayerHand) {
    auto& sprites = isPlayerHand ? _playerHandSprites : _opponentHandCards;
    auto& node = isPlayerHand ? _handLayer : _opponentHand;
    
    if (sprites.empty()) {
        return;
    }
    
    float startX = -((sprites.size() - 1) * CARD_SPACING) / 2;
    for (size_t i = 0; i < sprites.size(); ++i) {
        if (sprites[i]) {
            Vec2 targetPos = Vec2(startX + i * CARD_SPACING, 0);
            sprites[i]->runAction(MoveTo::create(0.3f, targetPos));
        }
    }
}
// 修改卡牌交互逻辑
void GameScene::addCardInteraction(cocos2d::Sprite* cardSprite) {
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);  // 防止触摸事件穿透

    listener->onTouchBegan = [this, cardSprite](Touch* touch, Event* event) {
        CCLOG("Touch began on card");
        if (!_isPlayerTurn) {
            CCLOG("Not player's turn");
            return false;
        }

        Vec2 touchPos = cardSprite->getParent()->convertToNodeSpace(touch->getLocation());
        if (cardSprite->getBoundingBox().containsPoint(touchPos)) {
            CCLOG("Card touched successfully");
            // 检查战场是否已满
            if (_playerFieldCards.size() >= 7) {
                CCLOG("Field is full!");
                return false;
            }
            // 直接打出卡牌
            playCardToField(cardSprite);
            return true;
        }
        return false;
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, cardSprite);
    CCLOG("Card interaction added");
}

void GameScene::playCardToField(cocos2d::Sprite* cardSprite) {
    // 找到精灵在手牌中的位置
    auto it = std::find(_playerHandSprites.begin(), _playerHandSprites.end(), cardSprite);
    if (it != _playerHandSprites.end()) {
        int index = std::distance(_playerHandSprites.begin(), it);
        
        // 获取对应的卡牌
        Card* card = _playerHand[index];
        if (!card) {
            return;
        }

        // 从手牌中移除
        _playerHandSprites.erase(it);
        _playerHand.erase(_playerHand.begin() + index);
        
        // 更新其他卡牌的位置
        updateHandPositions();
        
        // 将卡牌添加到场地
        _playerFieldCards.push_back(cardSprite);
        
        // 设置卡牌在场地上的位置
        float fieldY = _playerField->getPositionY();
        cardSprite->setPosition(Vec2(0, fieldY));
        
        // 更新场地上的卡牌位置
        updateFieldPositions();
        
        // 触发卡牌效果
        if (card) {
            card->onPlay();
        }
    }
}

// 添加战场卡牌的交互
void GameScene::addBattleCardInteraction(cocos2d::Sprite* cardSprite) {
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [this, cardSprite](Touch* touch, Event* event) {
        if (!_isPlayerTurn || _hasAttacked[cardSprite]) {
            CCLOG("Card cannot attack: not player turn or already attacked");
            return false;
        }

        Vec2 touchPos = cardSprite->getParent()->convertToNodeSpace(touch->getLocation());
        if (cardSprite->getBoundingBox().containsPoint(touchPos)) {
            // 选中攻击卡牌
            _selectedCard = cardSprite;
            cardSprite->setColor(Color3B::YELLOW);  // 高亮显示选中的卡牌
            CCLOG("Selected card for attack");
            return true;
        }
        return false;
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, cardSprite);
}

// 添加对手战场卡牌的交互
void GameScene::initListeners() {
    // ... 其他初始化代码 ...

    // 为对手战场添加触摸监听
    auto opponentFieldListener = EventListenerTouchOneByOne::create();
    opponentFieldListener->setSwallowTouches(true);

    opponentFieldListener->onTouchBegan = [this](Touch* touch, Event* event) {
        if (!_selectedCard) {
            return false;
        }

        // 将触摸位置转换到对手战场坐标系
        Vec2 touchPos = _opponentField->convertToNodeSpace(touch->getLocation());

        // 检查是否击到对手的卡牌
        for (auto targetCard : _opponentFieldCards) {
            if (targetCard->getBoundingBox().containsPoint(touchPos)) {
                // 执行攻击
                attackCard(_selectedCard, targetCard);

                // 重置选中状态
                _selectedCard->setColor(Color3B::GRAY);  // 攻击后变灰
                _hasAttacked[_selectedCard] = true;      // 标记为已攻击
                _selectedCard = nullptr;

                CCLOG("Attack completed");
                return true;
            }
        }

        // 如果没有点击到目标，取消攻击选择
        if (_selectedCard) {
            _selectedCard->setColor(Color3B::WHITE);
            _selectedCard = nullptr;
            CCLOG("Attack cancelled");
        }
        return false;
        };

    _opponentField->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
        opponentFieldListener,
        _opponentField
    );
}

// 添加攻击处理函数
void GameScene::attackCard(cocos2d::Sprite* attacker, cocos2d::Sprite* target) {
    CCLOG("Attacking card");

    // 从对手战场移除被攻击的卡牌
    auto it = std::find(_opponentFieldCards.begin(), _opponentFieldCards.end(), target);
    if (it != _opponentFieldCards.end()) {
        _opponentFieldCards.erase(it);
        target->removeFromParent();
        CCLOG("Target card removed from field");
    }

    // 重新排列对手战场
    float startX = -((_opponentFieldCards.size() - 1) * CARD_SPACING) / 2;
    for (size_t i = 0; i < _opponentFieldCards.size(); ++i) {
        _opponentFieldCards[i]->setPosition(Vec2(startX + i * CARD_SPACING, 0));
    }
}

// 在回合开始时重置攻击状态
void GameScene::startTurn() {
    _turnTimeRemaining = TURN_TIME;
    _isPlayerTurn = !_isPlayerTurn;
    _endTurnButton->setEnabled(_isPlayerTurn);

    if (_isPlayerTurn) {
        // 重置所有卡牌的攻击状态
        for (auto card : _playerFieldCards) {
            _hasAttacked[card] = false;
            card->setColor(Color3B::WHITE);  // 恢复正常颜色
        }
        _endTurnButton->setColor(Color3B::WHITE);
    }
    else {
        _endTurnButton->setColor(Color3B::GRAY);
    }
}

// 结束当前回合
void GameScene::endTurn() {
    _isPlayerTurn = !_isPlayerTurn;  // 切换回合
    _turnTimeRemaining = TURN_TIME;   // 重置回合时间

    // 更新英雄技能状态
    if (_playerHeroPower) {
        if (_isPlayerTurn) {
            _playerHeroPower->setEnabled(true);  // 我方回合启用英雄技能
            _playerHeroPower->setNormalImage(Sprite::create("heroes/hero_power_normal.png"));
            _playerHeroPower->setSelectedImage(Sprite::create("heroes/hero_power_selected.png"));
        }
        else {
            _playerHeroPower->setEnabled(false);  // 对方回合禁用英雄技能
            _playerHeroPower->setNormalImage(Sprite::create("heroes/hero_power_disabled.png"));
            _playerHeroPower->setSelectedImage(Sprite::create("heroes/hero_power_disabled.png"));
        }
    }

    // 重置所有卡牌的攻击状态
    _hasAttacked.clear();

    // 更新UI显示
    if (_isPlayerTurn) {
        // 我方回合开始
        _endTurnButton->setEnabled(true);  // 启用结束回合按钮
    }
    else {
        // 对方回合开始
        _endTurnButton->setEnabled(false);  // 禁用结束回合按钮

        // 模拟AI行动（这里可以添加AI逻辑）
        //this->scheduleOnce([this](float dt) {
            //endTurn();  // AI回合结束后自动切换到玩家回合
           // }, 3.0f);  // 3秒后结束AI回合
    }

    // 更新回合指示器
    updateTurnIndicator();
}

// 添加回合指示器更新方法
void GameScene::updateTurnIndicator() {
    if (_turnIndicatorLabel) {
        _turnIndicatorLabel->setString(_isPlayerTurn ? "Your Turn" : "Opponent's Turn");
        _turnIndicatorLabel->setTextColor(_isPlayerTurn ? Color4B::GREEN : Color4B::RED);
    }
}

// 修改英雄技能点击回调
void GameScene::onHeroPowerClicked(Ref* sender) {
    if (!_isPlayerTurn) {
        // 如果不是玩家回合，直接返回
        return;
    }

    auto gameManager = GameManager::getInstance();
    auto currentPlayer = gameManager->getCurrentPlayer();
}

// 更新回合计时器
void GameScene::updateTurnTimer(float dt) {
    if (_turnTimeRemaining > 0) {
        _turnTimeRemaining--;
        _turnTimerLabel->setString(std::to_string(_turnTimeRemaining));

        // 时间不多时改变颜色提示（调整为5秒）
        if (_turnTimeRemaining <= 5) {  // 由于总时间更短，建议将警告时间也调短
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
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float cardSpacing = CARD_SPACING;
    float startX = (visibleSize.width - (_playerHandSprites.size() - 1) * cardSpacing) / 2;
    float cardY = 150.0f;  // 手牌Y轴位置

    for (size_t i = 0; i < _playerHandSprites.size(); ++i) {
        auto sprite = _playerHandSprites[i];
        if (sprite) {
            auto moveAction = MoveTo::create(CARD_MOVE_DURATION, 
                Vec2(startX + i * cardSpacing, cardY));
            sprite->runAction(moveAction);
        }
    }
}

void GameScene::updateFieldPositions() {
    auto gameManager = GameManager::getInstance();
    auto currentPlayer = gameManager->getCurrentPlayer();
    auto opponentPlayer = gameManager->getOpponentPlayer();

    // 更新玩家场上从
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

void GameScene::onEndTurnClicked(cocos2d::Ref* sender) {
    if (_isPlayerTurn) {
        endTurn();
    }
}

Scene* GameScene::createWithDeck(Deck* deck) {
    GameScene* scene = new (std::nothrow) GameScene();
    if (scene && scene->initWithDeck(deck)) {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return nullptr;
}

bool GameScene::initWithDeck(Deck* deck) {
    auto logger = GameLogger::getInstance();
    logger->log(LogLevel::INFO, "Initializing game scene with deck");
    
    if (!Scene::init()) {
        logger->log(LogLevel::ERR, "Failed to initialize base Scene");
        return false;
    }

    // 设置玩家卡组
    _playerDeck = deck;  // 直接使用传入的 Deck 指针
    logger->log(LogLevel::INFO, "Player deck set successfully");

    // 初始化游戏场景
    initLayers();
    initBackground();
    initUI();
    initListeners();
    
    // 抽取起始手牌
    drawInitialHand();
    
    logger->log(LogLevel::INFO, "Game initialized");
    return true;
}

void GameScene::drawInitialHand() {
    auto logger = GameLogger::getInstance();
    logger->log(LogLevel::DEBUG, "Drawing initial hand");
    
    // 获取可见区域大小
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    // 抽取3张起始手牌
    for (int i = 0; i < 3; ++i) {
        if (!_playerDeck) {
            logger->log(LogLevel::WARNING, "Player deck is null");
            break;
        }
        
        // 获取牌库引用并修改
        auto& deckCards = const_cast<std::vector<Card*>&>(_playerDeck->getCards());
        if (deckCards.empty()) {
            logger->log(LogLevel::WARNING, "Deck is empty, cannot draw more cards");
            break;
        }
        
        // 从牌库顶部抽一张牌
        Card* card = deckCards.back();
        deckCards.pop_back();
        
        if (!card) {
            logger->log(LogLevel::ERR, "Drew null card");
            continue;
        }
        
        // 设置牌位置
        float cardSpacing = 150.0f;
        float startX = origin.x   - cardSpacing-500;
        float cardY = origin.y -150;
        
        card->setPosition(Vec2(startX + i * cardSpacing, cardY));
        card->setScale(0.8f);
        
        // 添加到手牌区域
        _playerHand.push_back(card);
        if (card->getSprite()) {
            _playerHandSprites.push_back(card->getSprite());
        }
        _handLayer->addChild(card);
        
        logger->log(LogLevel::DEBUG, "Added card to hand: " + card->getName());
    }
    
    logger->log(LogLevel::INFO, "Initial hand drawn successfully");
}

// 在析构函数中释放卡组
GameScene::~GameScene() {
    if (_playerDeck) {
        _playerDeck->release();
    }
}

void GameScene::initBackground() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    // 创建纯色背景
    auto background = LayerColor::create(Color4B(45, 45, 45, 255));  // 深灰色背景
    this->addChild(background, -10);
}
