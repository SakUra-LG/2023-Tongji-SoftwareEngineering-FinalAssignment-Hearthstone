// 1. 包含头文件和命名空间
#include "GameScene.h"          // 游戏场景类的声明
#include "Manager/GameManager.h"   // 游戏管理器
#include "Card/Card.h"          // 卡牌类
#include "Player/Player.h"      // 玩家类
#include "Utils/Constants.h"    // 游戏常量
#include "Animation/AnimationManager.h"  // 动画管理器
#include "Card/DeckManager.h"  // 添加这行
#include "Utils/GameLogger.h"  // 修正路径
#include "Battle/CombatManager.h"  // 添加这行
#pragma execution_character_set("utf-8")
// 使用 cocos2d 命名空间
USING_NS_CC;
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
    auto& sprites = isPlayerHand ? _playerHandSprites : _opponentHandSprites;
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
void GameScene::addCardInteraction(Card* card) {
    auto logger = GameLogger::getInstance();
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = [this, card, logger](Touch* touch, Event* event) {
        if (!_isPlayerTurn) return false;
        
        // 记录触摸点的世界坐标
        Vec2 worldTouchPos = touch->getLocation();
        logger->log(LogLevel::DEBUG, "Touch position (world): x=" + 
            std::to_string(worldTouchPos.x) + ", y=" + 
            std::to_string(worldTouchPos.y));

        // 获取卡牌在世界坐标系中的位置和边界
        Vec2 cardWorldPos = card->getParent()->convertToWorldSpace(card->getPosition());
        Size cardSize = card->getContentSize();
        float scale = card->getScale();
        
        // 计算卡牌在世界坐标系中的四个角点
        Vec2 bottomLeft = Vec2(
            cardWorldPos.x - (cardSize.width * scale) / 2,
            cardWorldPos.y - (cardSize.height * scale) / 2
        );
        Vec2 topRight = Vec2(
            cardWorldPos.x + (cardSize.width * scale) / 2,
            cardWorldPos.y + (cardSize.height * scale) / 2
        );

        //// 记录卡牌信息
        //std::string cardInfo = "\nCard bounds info:" +
        //    "\nCard name: " + card->getName() +
        //    "\nCard world position: x=" + std::to_string(cardWorldPos.x) + 
        //    ", y=" + std::to_string(cardWorldPos.y) +
        //    "\nCard size: w=" + std::to_string(cardSize.width) + 
        //    ", h=" + std::to_string(cardSize.height) +
        //    "\nCard scale: " + std::to_string(scale) +
        //    "\nBottom-left corner: x=" + std::to_string(bottomLeft.x) + 
        //    ", y=" + std::to_string(bottomLeft.y) +
        //    "\nTop-right corner: x=" + std::to_string(topRight.x) + 
        //    ", y=" + std::to_string(topRight.y);
        //logger->log(LogLevel::DEBUG, cardInfo);

        // 将触摸点转换为卡牌的本地坐标
        Vec2 localTouchPos = card->convertToNodeSpace(worldTouchPos);
        logger->log(LogLevel::DEBUG, "Touch position (local): x=" + 
            std::to_string(localTouchPos.x) + ", y=" + 
            std::to_string(localTouchPos.y));

        // 获取卡牌的边界框
        Rect cardBounds = card->getBoundingBox();
       /* std::string boundsInfo = "\nCard bounding box:" +
            "\nOrigin: x=" + std::to_string(cardBounds.origin.x) + 
            ", y=" + std::to_string(cardBounds.origin.y) +
            "\nSize: w=" + std::to_string(cardBounds.size.width) + 
            ", h=" + std::to_string(cardBounds.size.height);
        logger->log(LogLevel::DEBUG, boundsInfo);*/

        // 检查触摸点是否在卡牌边界内
        bool isInBounds = cardBounds.containsPoint(localTouchPos);
        logger->log(LogLevel::DEBUG, "Touch is " + 
            std::string(isInBounds ? "inside" : "outside") + " card bounds");

        if (isInBounds) {
            _selectedCard = card;
            
            if (_playerFieldCards.size() < 7) {
                auto visibleSize = Director::getInstance()->getVisibleSize();
                Vec2 fieldPos = Vec2(visibleSize.width / 2, FIELD_Y);
                playCardToField(card, fieldPos);
            }
            return true;
        }
        return false;
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, card);
}

bool GameScene::isValidFieldPosition(const Vec2& position) const {
    // 检查位置是否在场地范围内
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float fieldTop = FIELD_Y + 100;  // 场地上边界
    float fieldBottom = FIELD_Y - 100;  // 场地边界
    
    return position.y >= fieldBottom && position.y <= fieldTop;
}

void GameScene::playCardToField(Card* card, const Vec2& position) {
    auto logger = GameLogger::getInstance();
    logger->log(LogLevel::DEBUG, "Starting playCardToField for card: " + card->getName());
    
    // 检查是否可以放置卡牌
    if (!card || _playerFieldCards.size() >= 4) {
        logger->log(LogLevel::WARNING, "Cannot play card: field full or invalid card");
        return;
    }
    
    // 从手牌移除
    auto it = std::find(_playerHand.begin(), _playerHand.end(), card);
    if (it != _playerHand.end()) {
        _playerHand.erase(it);
        logger->log(LogLevel::DEBUG, "Removed card from hand vector: " + card->getName());
    }
    
    // 添加到场上卡牌数组
    _playerFieldCards.push_back(card);
    logger->log(LogLevel::DEBUG, "Added card to field array. Field size: " + 
        std::to_string(_playerFieldCards.size()));

    // 重要：检查节点状态
    Node* oldParent = card->getParent();
    logger->log(LogLevel::DEBUG, "Card parent check - Has parent: " + 
        std::string(oldParent ? "yes" : "no"));
    
    //auto card2 = card;

    if (oldParent) {
        logger->log(LogLevel::DEBUG, "Current parent retain count: " + 
            std::to_string(oldParent->getReferenceCount()));
        
        // 记录在旧父节点中的位置和缩放
        Vec2 oldPos = card->getPosition();
        float oldScale = card->getScale();
        logger->log(LogLevel::DEBUG, "Old position: x=" + std::to_string(oldPos.x) + 
            ", y=" + std::to_string(oldPos.y) + ", scale=" + std::to_string(oldScale));
        
        try {
            // 从旧父节点中移除
            logger->log(LogLevel::DEBUG, "Attempting to remove card from old parent");
            //oldParent->removeChild(card, true);
            logger->log(LogLevel::DEBUG, "Successfully removed card from old parent");
        } catch (const std::exception& e) {
            logger->log(LogLevel::ERR, "Exception while removing card: " + std::string(e.what()));
            return;
        }
    }

    try {
        // 检查游戏层是否有效
        if (!_gameLayer) {
            logger->log(LogLevel::ERR, "Game layer is null!");
            return;
        }
        
        logger->log(LogLevel::DEBUG, "Attempting to add card to game layer");
        //_gameLayer->addChild(card, 5);
        logger->log(LogLevel::DEBUG, "Successfully added card to game layer");
        
        // 设置新的位置和缩放
        auto visibleSize = Director::getInstance()->getVisibleSize();

        float centerX = visibleSize.width / 2;
        float fieldY = visibleSize.height * 0.4f;
        float cardSpacing = 150.0f;
        float startX = centerX - ((_playerFieldCards.size() - 1) * cardSpacing) / 2;
        int cardIndex = _playerFieldCards.size() - 1;
        float targetX = startX + cardIndex * cardSpacing;
        
        //card->setScale(0.8f);
        card->setPosition(Vec2(targetX-800, fieldY-100));
        logger->log(LogLevel::DEBUG, "Set new position: x=" + std::to_string(targetX) + 
            ", y=" + std::to_string(fieldY));
        
    } catch (const std::exception& e) {
        logger->log(LogLevel::ERR, "Exception while adding card to game layer: " + 
            std::string(e.what()));
        return;
    }
    
    // 更新位置
    updateFieldCardPositions();
    updateHandPositions();
    
    logger->log(LogLevel::DEBUG, "Completed playCardToField successfully");
}

void GameScene::updateFieldCardPositions() {
    if (_playerFieldCards.empty()) return;
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float centerX = visibleSize.width / 2-1000;
    float fieldY = visibleSize.height * 0.4f-250;
    float cardSpacing = 150.0f;
    float startX = centerX - ((_playerFieldCards.size() - 1) * cardSpacing) / 2;
    
    for (size_t i = 0; i < _playerFieldCards.size(); ++i) {
        auto card = _playerFieldCards[i];
        if (card) 
        {
            float targetX = startX + i * cardSpacing;
            card->setPosition(Vec2(targetX, fieldY));
        }
    }
}

// 添加战场卡牌的交互
void GameScene::addBattleCardInteraction(Card* card) {
    auto listener = EventListenerTouchOneByOne::create();
    
    // 保存卡牌的初始位置
    //card->setOriginalPosition(card->getPosition());
    
    listener->onTouchBegan = [this](Touch* touch, Event* event) {
        if (!_isPlayerTurn) return false;  // 只在玩家回合可以操作
        
        auto target = static_cast<Node*>(event->getCurrentTarget());
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        
        if (rect.containsPoint(locationInNode)) {
            _selectedCard = dynamic_cast<Card*>(target->getParent());
            if (_selectedCard && !_hasAttacked[_selectedCard]) {  // 检查是否已经攻击过
                return true;
            }
        }
        return false;
    };
    
    listener->onTouchMoved = [](Touch* touch, Event* event) {
        auto target = static_cast<Node*>(event->getCurrentTarget());
        target->getParent()->setPosition(target->getParent()->getPosition() + touch->getDelta());
    };
    
    listener->onTouchEnded = [this](Touch* touch, Event* event) {
        if (!_selectedCard) return;
        
        auto location = touch->getLocation();
        Card* targetCard = nullptr;
        
        // 检查是否有效的攻击目标
        for (auto card : _opponentFieldCards) {
            if (card->getBoundingBox().containsPoint(location)) {
                targetCard = card;
                break;
            }
        }
        
        if (targetCard) {
            attackCard(_selectedCard, targetCard);
            _hasAttacked[_selectedCard] = true;  // 标记已经攻击过
        }
        
        // 无论是否攻击成功，都返回原位置
        //_selectedCard->setPosition(_selectedCard->getOriginalPosition());
        _selectedCard = nullptr;
    };
    
    // 将监听器添加到牌的精灵上
    if (card->getSprite()) {
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, card->getSprite());
    }
}

// 添加对手战场卡牌的交互
void GameScene::initListeners() {
    // ... 其他初始化代码 ...

    // 为场上每张卡牌添加战斗交互
    for (auto card : _playerFieldCards) {
        addBattleCardInteraction(card);
    }
}

// 修改攻击处理函数
void GameScene::attackCard(Card* attacker, Card* target) {
    if (!attacker || !target) return;
    
    CCLOG("Attacking card");

    auto combatMgr = CombatManager::getInstance();
    if (combatMgr->canAttack(attacker, target)) {
        combatMgr->performAttack(attacker, target);
        
        // 如果目标死亡，从战场移除
        if (target->getHealth() <= 0) {
            auto it = std::find(_opponentFieldCards.begin(), _opponentFieldCards.end(), target);
            if (it != _opponentFieldCards.end()) {
                _opponentFieldCards.erase(it);
                target->removeFromParent();
                CCLOG("Target card removed from field");
            }
        }

        // 重新排列对手战场
        updateFieldPositions();
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

    // 重置所有随从的攻击状态
    for (auto& pair : _hasAttacked) {
        pair.second = false;
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
    
    // 调用父类的初始化方法
    if (!Scene::init()) {
        logger->log(LogLevel::ERR, "Failed to initialize base Scene");
        return false;
    }

    // 设置玩家卡组
    _playerDeck = deck;  // 直接使用传入的 Deck 指针
    logger->log(LogLevel::INFO, "Player deck set successfully");

    // 初始化场景的各个组件
    initLayers();     // 初始化游戏层级
    initUI();         // 初始化用户界面
    initListeners();  // 初始化事件监听器
    
    // 抽取起始手牌
    drawInitialHand();
    
    // 启动更新定时器
    this->schedule(CC_SCHEDULE_SELECTOR(GameScene::updateUI), 1.0f);  // UI更新（每秒）
    this->scheduleUpdate();  // 游戏逻辑更新（每帧）
    
    logger->log(LogLevel::INFO, "Game scene initialized successfully");
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
        
        auto& deckCards = const_cast<std::vector<Card*>&>(_playerDeck->getCards());
        if (deckCards.empty()) {
            logger->log(LogLevel::WARNING, "Deck is empty, cannot draw more cards");
            break;
        }
        
        Card* card = deckCards.back();
        deckCards.pop_back();
        
        if (!card) {
            logger->log(LogLevel::ERR, "Drew null card");
            continue;
        }
        
        float cardSpacing = 150.0f;
        float startX = origin.x - cardSpacing - 500;
        float cardY = origin.y - 150;
        
        card->setPosition(Vec2(startX + i * cardSpacing, cardY));
        card->setScale(0.8f);
        
        _playerHand.push_back(card);
        _handLayer->addChild(card);
        
        logger->log(LogLevel::DEBUG, "Added card to hand: " + card->getName());
    }
    
    // 所有卡牌添加完成后，初始化手牌区域的交互
    initHandInteraction();
    
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
    
    // 创建纯色背��
    auto background = LayerColor::create(Color4B(45, 45, 45, 255));  // 深灰色背景
    this->addChild(background, -10);
}

void GameScene::initHandInteraction() {
    auto logger = GameLogger::getInstance();
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = [this, logger](Touch* touch, Event* event) {
        if (!_isPlayerTurn) return false;
        
        Vec2 worldTouchPos = touch->getLocation();
        logger->log(LogLevel::DEBUG, "Touch position (world): x=" + 
            std::to_string(worldTouchPos.x) + ", y=" + 
            std::to_string(worldTouchPos.y));

        // 遍历所有手牌检查点击
        for (auto card : _playerHand) {
            // 获取卡牌在世界坐标系中的位置和边界
            Vec2 cardWorldPos = card->getParent()->convertToWorldSpace(card->getPosition());
            Size cardSize = card->getContentSize();
            float scale = card->getScale();
            
            // 计算卡牌碰撞箱
            Rect cardRect(
                cardWorldPos.x - (cardSize.width * scale) / 2,
                cardWorldPos.y - (cardSize.height * scale) / 2,
                cardSize.width * scale,
                cardSize.height * scale
            );
            
            // 记录卡牌信息
            std::string cardInfo = "\nChecking card: " + card->getName() +
                "\nCard world position: x=" + std::to_string(cardWorldPos.x) + 
                ", y=" + std::to_string(cardWorldPos.y) +
                "\nCard rect: x=" + std::to_string(cardRect.origin.x) + 
                ", y=" + std::to_string(cardRect.origin.y) +
                ", w=" + std::to_string(cardRect.size.width) + 
                ", h=" + std::to_string(cardRect.size.height);
            logger->log(LogLevel::DEBUG, cardInfo);

            // 检查点击是否在卡牌范围内
            if (cardRect.containsPoint(worldTouchPos)) {
                logger->log(LogLevel::DEBUG, "Card selected: " + card->getName());
                _selectedCard = card;
                
                if (_playerFieldCards.size() < 7) {
                    auto visibleSize = Director::getInstance()->getVisibleSize();
                    Vec2 fieldPos = Vec2(visibleSize.width / 2, FIELD_Y);
                    playCardToField(card, fieldPos);
                }
                return true;
            }
        }
        return false;
    };
    
    // 将监听器添加到手牌层
    _handLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _handLayer);
}

