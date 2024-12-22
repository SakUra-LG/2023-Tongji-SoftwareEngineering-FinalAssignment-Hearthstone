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
    //_playerField->addChild(playerFieldArea);
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
  //  _opponentHand->addChild(opponentHandArea);

    // 对手场地（手牌区下方）
    auto opponentFieldArea = DrawNode::create();
    opponentFieldArea->drawRect(
        Vec2(-550, -100),  // 左下角坐标
        Vec2(550, 100),    // 右上角坐标，总大小1100x200
        Color4F(1, 0.5f, 0, 0.2f)  // 半透明橙色
    );
   // _opponentField->addChild(opponentFieldArea);

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
   // _handLayer->addChild(playerHandLabel);

    auto playerFieldLabel = createLabel("Player Field\n1100x200");  // 更新尺寸信息
    playerFieldLabel->setPosition(Vec2::ZERO);
   // _playerField->addChild(playerFieldLabel);

    auto opponentHandLabel = createLabel("Opponent Hand\n1200x260");  // 更新尺寸信息
    opponentHandLabel->setPosition(Vec2::ZERO);
   // _opponentHand->addChild(opponentHandLabel);

    auto opponentFieldLabel = createLabel("Opponent Field\n1100x200");  // 更新尺寸信息
    opponentFieldLabel->setPosition(Vec2::ZERO);
    //_opponentField->addChild(opponentFieldLabel);
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
       // _playerHealthLabel = Label::createWithTTF("30", "fonts/arial.ttf", 24);
       // _playerHealthLabel->setPosition(Vec2(visibleSize.width / 2, 80));
       // _uiLayer->addChild(_playerHealthLabel);
    }
    // 添加对手头像（正上方）
    auto opponentHeroPortrait = Sprite::create("heroes/opponent_portrait.png");
    if (opponentHeroPortrait) {
        opponentHeroPortrait->setPosition(Vec2(visibleSize.width / 2 + 25, visibleSize.height - 215));  // x=1024, y=904
        opponentHeroPortrait->setScale(0.8f);
        _uiLayer->addChild(opponentHeroPortrait);

        // 对手生命值显示（头像上方）
       // auto opponentHealth = Label::createWithTTF("30", "fonts/arial.ttf", 24);
       // opponentHealth->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 95));
        //_uiLayer->addChild(opponentHealth);
    }

    // 添加对手英雄技能（头像右侧）
    auto opponentHeroPower = Sprite::create("heroes/hero_power_disabled.png");
    if (opponentHeroPower) {
        opponentHeroPower->setPosition(Vec2(visibleSize.width / 2 - 150, visibleSize.height - 170));
        opponentHeroPower->setScale(0.8f);
        _uiLayer->addChild(opponentHeroPower);
    }

    /*
    // 玩家法力水晶显示（头像左侧）
            _playerManaLabel = Label::createWithTTF("0/10", "fonts/arial.ttf", 24);
            _playerManaLabel->setPosition(Vec2(visibleSize.width / 2 - 100, 120));
            _uiLayer->addChild(_playerManaLabel);

            // 对手法力水晶显示（头像左侧）
            auto opponentMana = Label::createWithTTF("0/", "fonts/arial.ttf", 24);
            opponentMana->setPosition(Vec2(1280, visibleSize.height - 95));
            _uiLayer->addChild(opponentMana);
    */
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
            _selectedCard = dynamic_cast<MinionCard*>(card);

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
    float fieldTop = FIELD_Y + 100;  // 场地边界上边
    float fieldBottom = FIELD_Y - 100;  // 场地边界下边

    return position.y >= fieldBottom && position.y <= fieldTop;
}

void GameScene::playCardToField(Card* card, const Vec2& position) {
    auto logger = GameLogger::getInstance();
    logger->log(LogLevel::DEBUG, "Starting playCardToField for card: " + card->getName());

    // 检查是否可放卡牌
    if (!card || _playerFieldCards.size() >= 4) {
        logger->log(LogLevel::WARNING, "Cannot play card: field full or invalid card");
        return;
    }

    if (card->_isDead)
    {
        return;
    }

    if (!card->getOnField())
    {
        logger->log(LogLevel::WARNING, "_isOnField没有正确更改！！！！！");
        return;
    }


    // 获取当前玩家
    auto gameManager = GameManager::getInstance();
    auto currentPlayer = gameManager->getCurrentPlayer();
    
    // 检查当前玩家是否有效
    if (!currentPlayer) {
        logger->log(LogLevel::ERR, "Current player is null!");
        return;
    }

    // 检查法力值是否够用
    if (!currentPlayer->hasEnoughMana(card->getCost())) {
        logger->log(LogLevel::WARNING, "Not enough mana to play card!");
        showTemporaryMessage("法力值不足！");
        return;
    }

    // 消耗法力值
    currentPlayer->spendMana(card->getCost());
    logger->log(LogLevel::DEBUG, "Spent " + std::to_string(card->getCost()) + " mana");

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
        }
        catch (const std::exception& e) {
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
        card->setPosition(Vec2(targetX - 800, fieldY - 100));
        logger->log(LogLevel::DEBUG, "Set new position: x=" + std::to_string(targetX) +
            ", y=" + std::to_string(fieldY));

    }
    catch (const std::exception& e) {
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
    float centerX = visibleSize.width / 2 - 1000;
    float fieldY = visibleSize.height * 0.4f - 250;
    float cardSpacing = 150.0f;
    float startX = centerX - ((_playerFieldCards.size() - 1) * cardSpacing) / 2;

    for (size_t i = 0; i < _playerFieldCards.size(); ++i) {
        auto card = _playerFieldCards[i];
        if (card)
        {
            float targetX = startX + i * cardSpacing;
            card->setPosition(Vec2(targetX, fieldY));
            if (card->_isDead)
            {
                card->setPosition(Vec2(20000, 20000));
            }
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
            _selectedCard = dynamic_cast<MinionCard*>(target->getParent());
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
    auto logger = GameLogger::getInstance();
    logger->log(LogLevel::DEBUG, "Starting new turn");
    
    _turnTimeRemaining = TURN_TIME;
    _isPlayerTurn = !_isPlayerTurn;  // 切换回合
    _endTurnButton->setEnabled(_isPlayerTurn);
    
    //drawEnemyCard();
    
    auto gameManager = GameManager::getInstance();
    auto currentPlayer = gameManager->getCurrentPlayer();
    
    if (currentPlayer) {
        auto logger = GameLogger::getInstance();
        logger->log(LogLevel::DEBUG, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
        // 增加最大法力值（上限为10）
        int currentMaxMana = currentPlayer->getMaxMana();
        if (currentMaxMana < 10) {  // 只在未达到上限时增加
            int newMaxMana = std::min(currentMaxMana + 2, 10);
            currentPlayer->setMaxMana(newMaxMana);
        }

        // 回合开始时将法力值补满到最大值
        currentPlayer->setMana(currentPlayer->getMaxMana());

        // 记录日志
        std::string manaInfo = "Turn started: Mana restored to " +
            std::to_string(currentPlayer->getMana()) + "/" +
            std::to_string(currentPlayer->getMaxMana());
        GameLogger::getInstance()->log(LogLevel::INFO, manaInfo);
    }

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

    logger->log(LogLevel::DEBUG, "Turn started successfully");
}

void GameScene::autoEndTurn(float delay) {



    // 创建延时动作
    auto delayAction = DelayTime::create(delay);

    // 创建结束回合的回调
    auto endTurnCallback = CallFunc::create([this]() {
        endTurn();
        });

    // 停止之前可能存在的自动结束回合动作
    this->stopActionByTag(999);  // 使用标签999来标识自动结束回合的动作

    // 创建动作序列并执行
    auto sequence = Sequence::create(delayAction, endTurnCallback, nullptr);
    sequence->setTag(999);  // 设置标签以便后续可以停止该动作
    this->runAction(sequence);
};
// 结束当前回合
void GameScene::endTurn() {
    auto logger = GameLogger::getInstance();
    logger->log(LogLevel::DEBUG, "Ending current turn");
    
    // 结束当前回合
    _isPlayerTurn = !_isPlayerTurn;  // 切换回合
    _turnTimeRemaining = TURN_TIME;   // 重置回合时间
    
    // 开始新回合
    
    
    auto gameManager = GameManager::getInstance();
    auto currentPlayer = gameManager->getCurrentPlayer();
    
    // 更新英雄技能状态
    if (_playerHeroPower) {
        if (_isPlayerTurn) {
            if (currentPlayer) {
                int newMaxMana = std::min(currentPlayer->getMaxMana() + 2, 10);  // 上限为10
                currentPlayer->setMaxMana(newMaxMana);
                currentPlayer->setMana(newMaxMana);  // 回满法力值
            }
            // 我方回合开始，重置为可点击的按钮
            _playerHeroPower->setEnabled(true);
            _playerHeroPower->setNormalImage(Sprite::create("heroes/hero_power_normal.png"));
            _playerHeroPower->setSelectedImage(Sprite::create("heroes/hero_power_pressed.png"));  // 使用pressed而不是selected
            _playerHeroPower->setScale(0.7f);  // 保持原有缩放
        }
        else {
            // 对方回合开始，设置为禁用状态
            //drawEnemyCard();
            _playerHeroPower->setEnabled(false);
            _playerHeroPower->setNormalImage(Sprite::create("heroes/hero_power_disabled.png"));
            _playerHeroPower->setSelectedImage(Sprite::create("heroes/hero_power_disabled.png"));
            _playerHeroPower->setScale(0.7f);  // 保持原有缩放
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
        autoEndTurn(1);
        
    }

    // 更新回合指示器
    updateTurnIndicator();

    logger->log(LogLevel::DEBUG, "Turn ended successfully");
}

// 添加回合指示器更新方法
void GameScene::updateTurnIndicator() {
    _turnIndicatorLabel->setString(_isPlayerTurn ? "Your Turn" : "Opponent's Turn");
    _turnIndicatorLabel->setTextColor(_isPlayerTurn ? Color4B::GREEN : Color4B::RED);
    auto logger = GameLogger::getInstance();
    logger->log(LogLevel::DEBUG, "Starting updateTurnIndicator");
    
    if (!_isPlayerTurn) {
        int sizeOfEnemyLeft;
        sizeOfEnemyLeft = _opponentHandCards.size();
        if (sizeOfEnemyLeft == 0)
            return;
        
        Card* card = _opponentHandCards.back();
        _opponentHandCards.pop_back();
        
        auto logger = GameLogger::getInstance();
        logger->log(LogLevel::DEBUG, "放置敌方随从 --------");
        logger->log(LogLevel::DEBUG, "剩余手牌数量: " + std::to_string(sizeOfEnemyLeft));
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        float fieldY = visibleSize.height * 0.6f;  // 在屏幕上方60%的位置
        Vec2 position;

        if (sizeOfEnemyLeft == 3) {
            position = Vec2(806.003662f-50, fieldY);
            logger->log(LogLevel::DEBUG, "位置1: x=" + std::to_string(position.x) + ", y=" + std::to_string(position.y));
            card->serDeathrattle(card,true);
        }
        else if (sizeOfEnemyLeft == 2) {
            position = Vec2(950.001343f-45, fieldY);
            logger->log(LogLevel::DEBUG, "位置2: x=" + std::to_string(position.x) + ", y=" + std::to_string(position.y));
        }
        else if (sizeOfEnemyLeft == 1) {
            position = Vec2(1105.002441f-50, fieldY);
            logger->log(LogLevel::DEBUG, "位置3: x=" + std::to_string(position.x) + ", y=" + std::to_string(position.y));
        }
        
        card->setPosition(position);
        card->setScale(0.8f);

        MinionCard* minion = dynamic_cast<MinionCard*>(card);
        if (minion) {
            logger->log(LogLevel::DEBUG, "放置随从: " + minion->getName());
            logger->log(LogLevel::DEBUG, "随从属性 - 攻击力: " + std::to_string(minion->getAttack()) + 
                ", 生命值: " + std::to_string(minion->getHealth()));
            
            // 计算并记录碰撞箱
            Size cardSize = minion->getContentSize();
            float scale = minion->getScale();
            Rect cardRect(
                position.x - (cardSize.width * scale) / 2,
                position.y - (cardSize.height * scale) / 2,
                cardSize.width * scale,
                cardSize.height * scale
            );
            
            logger->log(LogLevel::DEBUG, "随从碰撞箱 - x=" + std::to_string(cardRect.origin.x) + 
                ", y=" + std::to_string(cardRect.origin.y) + 
                ", width=" + std::to_string(cardRect.size.width) + 
                ", height=" + std::to_string(cardRect.size.height));
        }

        _opponentFieldCards2.push_back(minion);
        isStart = true;
        this->addChild(minion, 5);
    }
}

// 修改英雄技能点击回调-所有牌+1攻击力
void GameScene::onHeroPowerClicked(Ref* sender) {
    if (!_isPlayerTurn) {
        // 如果不是玩家回合，直接返回
        return;
    }

    auto gameManager = GameManager::getInstance();
    auto currentPlayer = gameManager->getCurrentPlayer();


    _playerHand2[0]->setAttack(_playerHand2[0]->getAttack() + 1);
    _playerHand2[0]->initUI2();
    _playerHand2[1]->setAttack(_playerHand2[1]->getAttack() + 1);
    _playerHand2[1]->initUI2();
    _playerHand2[2]->setAttack(_playerHand2[2]->getAttack() + 1);
    _playerHand2[2]->initUI2();
    if (currentPlayer->hasEnoughMana(2))
    {
        currentPlayer->spendMana(2);
    }
    
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
    auto gameManager = GameManager::getInstance();
    auto currentPlayer = gameManager->getCurrentPlayer();

    if (!currentPlayer) {
        CCLOG("Current player is null in updateUI");
        return;
    }

    // 更新法力值显示
    std::string manaText = std::to_string(currentPlayer->getMana()) + "/" +
        std::to_string(currentPlayer->getMaxMana());
    if (_playerManaLabel) {
        _playerManaLabel->setString(manaText);
        CCLOG("Updating mana display: %s", manaText.c_str());

        // 根据法力值状态改变颜色
        if (currentPlayer->getMana() == 0) {
            _playerManaLabel->setTextColor(Color4B(150, 150, 150, 255));  // 灰色
        }
        else if (currentPlayer->getMana() < currentPlayer->getMaxMana()) {
            _playerManaLabel->setTextColor(Color4B(0, 162, 232, 255));    // 蓝色
        }
        else {
            _playerManaLabel->setTextColor(Color4B(0, 200, 255, 255));    // 亮蓝色
        }
    }
    else {
        CCLOG("Mana label is null");
    }

    // 更新生命值显示
    _playerHealthLabel->setString(std::to_string(currentPlayer->getHealth()));
    if (currentPlayer->getHealth() <= 10) {
        _playerHealthLabel->setTextColor(Color4B::RED);
    }

    // 更新对手信息
    auto opponentPlayer = gameManager->getOpponentPlayer();
    if (_opponentHealthLabel) {
        _opponentHealthLabel->setString(std::to_string(opponentPlayer->getHealth()));
        if (opponentPlayer->getHealth() <= 10) {
            _opponentHealthLabel->setTextColor(Color4B::RED);
        }
    }

    // 更新回合状态
    _endTurnButton->setEnabled(_isPlayerTurn);
    _endTurnButton->setColor(_isPlayerTurn ? Color3B::WHITE : Color3B::GRAY);

    // 更新英雄技能状态
    bool canUseHeroPower = gameManager->canUseHeroPower(currentPlayer);
    _playerHeroPower->setEnabled(canUseHeroPower && _isPlayerTurn);
    _playerHeroPower->setColor(canUseHeroPower && _isPlayerTurn ? Color3B::WHITE : Color3B::GRAY);

    // 更新手牌显示
    updateHandPositions();

    // 更新场上随从
    updateFieldPositions();

    // 检查游戏状态
    if (isStart)
    {
        if (gameManager->isGameOver()) {  // 使用提供的 isGameOver() 方法
            showGameOverUI();
        }
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

    // 更新玩家场上
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
    auto result = gameManager->getGameResult();  // 使用游戏结果枚举
    
    // 创建半透明背景
    auto dimLayer = LayerColor::create(Color4B(0, 0, 0, 128));
    this->addChild(dimLayer, 100);

    // 创建游戏结束标签
    auto winLabel = Label::createWithTTF(
        (result == GameResult::VICTORY) ? "Victory!" : "Defeat",
        "fonts/arial.ttf",
        72
    );
    winLabel->setPosition(Director::getInstance()->getVisibleSize() / 2);
    winLabel->setTextColor((result == GameResult::VICTORY) ? Color4B::YELLOW : Color4B::RED);
    dimLayer->addChild(winLabel);

    // 创建渐变黑屏效果
    auto fadeToBlack = Sequence::create(
        DelayTime::create(2.0f),  // 等待2秒让玩家看清结果
        CallFunc::create([dimLayer]() {
            // 开始渐变到全黑
            dimLayer->runAction(FadeTo::create(3.0f, 255));
        }),
        DelayTime::create(3.0f),  // 等待渐变完成
        CallFunc::create([]() {
            // 退出程序
            Director::getInstance()->end();
            #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                exit(0);
            #endif
        }),
        nullptr
    );

    // 运行动作序列
    this->runAction(fadeToBlack);

    // 禁用所有输入
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](Touch* touch, Event* event) {
        return true;  // 吞掉所有触摸事件
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameScene::onEndTurnClicked(cocos2d::Ref* sender) {
    if (_isPlayerTurn) {
        endTurn();
    }
}

Scene* GameScene::createWithDeck(Deck* deck, Deck* deckenemy2) {
    GameScene* scene = new (std::nothrow) GameScene();
    scene->init();
    if (scene && scene->initWithDeck(deck, deckenemy2)) {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return nullptr;
}

bool GameScene::initWithDeck(Deck* deck, Deck* deckenemy2) {
    auto logger = GameLogger::getInstance();
    logger->log(LogLevel::INFO, "Initializing game scene with deck");

    if (!Scene::init()) {
        logger->log(LogLevel::ERR, "Failed to initialize base Scene");
        return false;
    }

    // 确保 GameManager 和 Player 已经初始化
    GameManager* gameManager = GameManager::getInstance();
    if (!gameManager->getCurrentPlayer()) {
        // 如果当前玩家为空，创建新的玩家
        auto player = new Player();
        // 设置初始法力值为2，最大法力值为10
        player->setMaxMana(5);  // 设置最大法力值上限
        player->setMana(5);      // 设置初始法力值
        gameManager->setCurrentPlayer(player);
        
        // 创建对手玩家
        auto opponent = new Player();
        opponent->setMaxMana(5);  // 设置最大法力值上限
        opponent->setMana(5);      // 设置初始法力值
        gameManager->setOpponentPlayer(opponent);
        
        logger->log(LogLevel::DEBUG, "Created new players with initial mana (2/10)");
    }

    // 设置玩家卡组
    _playerDeck = deck;
    _enemyDeck = deckenemy2;
    // 初始化场景的各个组件
    initLayers();
    initUI();
    initListeners();
    drawInitialHand();

    // 立即更新一次UI确保显示正确
    updateUI(0);

    this->schedule(CC_SCHEDULE_SELECTOR(GameScene::updateUI), 1.0f);
    this->scheduleUpdate();

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

    _playerHand2 = convertToMinionCards(_playerHand);

    //可以成功创建！！！
    auto& deckCards = const_cast<std::vector<Card*>&>(_enemyDeck->getCards());
    for (int i = 0; i < 3; i++) {
        Card* card = deckCards.back();
        card->retain();  // 增加引用计数
        
        logger->log(LogLevel::DEBUG, "Drawing enemy card: " + 
            (card ? card->getName() : "null") + 
            " at index " + std::to_string(i) + 
            " Address: " + std::to_string(reinterpret_cast<intptr_t>(card)) +
            " Initial ref count: " + std::to_string(card->getReferenceCount()));
            
        deckCards.pop_back();
        float cardSpacing = 150.0f;
        float startX = origin.x - cardSpacing - 500;
        float cardY = origin.y - 150;
        card->setPosition(Vec2(startX + 3 * cardSpacing, cardY));
        card->setScale(0.8f);

        // 在添加到手牌前检查卡牌
        logger->log(LogLevel::DEBUG, "Adding card to opponent hand. Card address: " + 
            std::to_string(reinterpret_cast<intptr_t>(card)));
            
        _opponentHandCards.push_back(card);
        
        // 验证添加后的状态
        logger->log(LogLevel::DEBUG, "After adding to hand - ref count: " + 
            std::to_string(card->getReferenceCount()));
    }
    
    // 所有卡牌添加完成后，初始化手牌区域的交互
    initHandInteraction();

    logger->log(LogLevel::INFO, "Initial hand drawn successfully");
}

//转换卡组类型
std::vector<MinionCard*> GameScene::convertToMinionCards(const std::vector<Card*>& cards) {
    std::vector<MinionCard*> minionCards;
    auto logger = GameLogger::getInstance();

    for (Card* card : cards) {
        MinionCard* minion = dynamic_cast<MinionCard*>(card);
        if (minion) {
            minionCards.push_back(minion);
            logger->log(LogLevel::DEBUG, "Successfully converted card to MinionCard");
        }
    }

    return minionCards;
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

void GameScene::initHandInteraction() {
    auto logger = GameLogger::getInstance();
    auto listener = EventListenerTouchOneByOne::create();

    listener->onTouchBegan = [this, logger](Touch* touch, Event* event) {
        if (!_isPlayerTurn) return false;

        Vec2 worldTouchPos = touch->getLocation();
        logger->log(LogLevel::DEBUG, "Touch position (world): x=" + 
            std::to_string(worldTouchPos.x) + ", y=" + std::to_string(worldTouchPos.y));
        
        // 如果已经有选中的卡牌，尝试选择目标
        if (_selectedCard) {
            //handleTargetSelection(worldTouchPos);
            return true;
        }
        
        // 检查场上的友方随从
        for (auto card : _playerFieldCards) {
            if (!card) continue;
            
            // 获取卡牌在世界坐标系中的位置和边界
            Vec2 cardWorldPos = card->getParent()->convertToWorldSpace(card->getPosition());
            Size cardSize = card->getContentSize();
            float scale = card->getScale();
            
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
            
            // 检查点击是否在卡牌范围内且卡��在场上
            if (cardRect.containsPoint(worldTouchPos) && card->getOnField()) {
                logger->log(LogLevel::DEBUG, "Card selected: " + card->getName());
                handleCardSelection(card, worldTouchPos);
                return true;
            }
        }
        
        return false;
    };

    // 将监听器添加到手牌层
    _handLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _handLayer);
}

// 增加法力值的方法
void GameScene::addMana(int amount) {
    auto gameManager = GameManager::getInstance();
    auto currentPlayer = gameManager->getCurrentPlayer();

    if (currentPlayer) {
        int newMana = std::min(currentPlayer->getMana() + amount, currentPlayer->getMaxMana());
        currentPlayer->setMana(newMana);
        // UI 会在下一帧自动更新
    }
}

// 消耗法力值的方法
bool GameScene::useMana(int amount) {
    auto gameManager = GameManager::getInstance();
    auto currentPlayer = gameManager->getCurrentPlayer();

    if (currentPlayer && currentPlayer->getMana() >= amount) {
        currentPlayer->setMana(currentPlayer->getMana() - amount);
        // UI 会在下一帧自动更新
        return true;
    }
    return false;
}

// 检查是否有足够的法力值
bool GameScene::hasEnoughMana(int amount) {
    auto gameManager = GameManager::getInstance();
    auto currentPlayer = gameManager->getCurrentPlayer();

    return currentPlayer && currentPlayer->getMana() >= amount;
}

void GameScene::showTemporaryMessage(const std::string& message, float duration) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    // 创建提示标签
    auto label = Label::createWithTTF(message, "fonts/STKAITI.TTF", 50);
    label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2-20));
    label->setTextColor(Color4B::RED);
    this->addChild(label, 10);  // 使用高 Z-order 确保显示在最上层
    
    // 创建渐变消失的动作序列
    auto fadeOut = FadeOut::create(0.2f);  // 0.2秒渐隐
    auto delay = DelayTime::create(duration - 0.2f);  // 持续时间减去渐隐时间
    auto remove = RemoveSelf::create();  // 移除节点
    
    // 运行动作序列
    label->runAction(Sequence::create(delay, fadeOut, remove, nullptr));
}

void GameScene::drawEnemyCard() {
    auto logger = GameLogger::getInstance();
    
    //可以成功创建！！！
    /*auto& deckCards = const_cast<std::vector<Card*>&>(_enemyDeck->getCards());
    Card* card = deckCards.back();
    deckCards.pop_back();
    float cardSpacing = 150.0f;
    float startX = origin.x - cardSpacing - 500;
    float cardY = origin.y - 150;

    card->setPosition(Vec2(startX + 3 * cardSpacing, cardY));
    card->setScale(0.8f);

    _playerHand.push_back(card);
    _handLayer->addChild(card);*/

    if (!_enemyDeck) {
        logger->log(LogLevel::WARNING, "Enemy deck is null");
        return;
    }
    
    // 获牌库引用并修改
    auto& deckCards = const_cast<std::vector<Card*>&>(_enemyDeck->getCards());
    if (deckCards.empty()) {
        logger->log(LogLevel::WARNING, "Enemy deck is empty");
        return;
    }
    
    try {
        // 从牌库顶部抽一张牌
        Card* card = deckCards.back();
        deckCards.pop_back();
        
        if (!card) {
            logger->log(LogLevel::ERR, "Drew null card from enemy deck");
            return;
        }
        
        // 将卡牌添加到手牌容器
        _opponentHandCards.push_back(card);
        
        // 转换为随从卡并添加到随从容器
        MinionCard* minion = dynamic_cast<MinionCard*>(card);
        if (minion) {
            // 先初始化UI
            minion->initUI2();
            _opponentHandCards2.push_back(minion);
            logger->log(LogLevel::DEBUG, "Added enemy minion card: " + minion->getName());
            
            // 直接打出到场上
            if (_gameLayer) {
                // 设置卡牌位置和大小
                auto visibleSize = Director::getInstance()->getVisibleSize();
                float centerX = visibleSize.width / 2;
                float fieldY = visibleSize.height * 0.6f;  // 在屏幕上方60%的位置
                
                minion->setScale(0.8f);
                minion->setPosition(Vec2(centerX, fieldY));
                
                // 添加到场上卡牌数组
                _opponentFieldCards.push_back(minion);
                
                // 添加到游戏层
                _gameLayer->addChild(minion, 5);
                
                // 更新场上卡牌位置
                updateEnemyFieldPositions();
                
                logger->log(LogLevel::DEBUG, "Enemy card played to field: " + minion->getName());
            } else {
                logger->log(LogLevel::ERR, "Game layer is null");
            }
        } else {
            logger->log(LogLevel::WARNING, "Card is not a MinionCard");
        }
    } catch (const std::exception& e) {
        logger->log(LogLevel::ERR, "Exception in drawEnemyCard: " + std::string(e.what()));
    }
}

void GameScene::updateEnemyFieldPositions() {
    if (_opponentFieldCards.empty()) return;
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float centerX = visibleSize.width / 2;
    float fieldY = visibleSize.height * 0.6f;
    float cardSpacing = 150.0f;
    float startX = centerX - ((_opponentFieldCards.size() - 1) * cardSpacing) / 2;
    
    for (size_t i = 0; i < _opponentFieldCards.size(); ++i) {
        auto card = _opponentFieldCards[i];
        if (card) {
            float targetX = startX + i * cardSpacing;
            card->setPosition(Vec2(targetX, fieldY));
        }
    }
}

void GameScene::handleCardSelection(Card* card, const Vec2& touchPos) {
    auto logger = GameLogger::getInstance();
    logger->log(LogLevel::DEBUG, "Handling card selection");
    
    if (!card) {
        logger->log(LogLevel::ERR, "Invalid card pointer");
        return;
    }
    
    // 添加更多卡牌信息的日志
    logger->log(LogLevel::DEBUG, "Card info - Address: " + std::to_string((intptr_t)card));
    logger->log(LogLevel::DEBUG, "Card name: " + (card->getName().empty() ? "EMPTY" : card->getName()));
    
    // 检查卡牌的父节点
    auto parent = card->getParent();
    logger->log(LogLevel::DEBUG, "Card parent: " + 
        (parent ? "YES - " + std::to_string((intptr_t)parent) : "NO"));
    
    // 获取世界坐标和本地坐标
    Vec2 localPos = card->getPosition();
    Vec2 worldPos = parent ? parent->convertToWorldSpace(localPos) : localPos;
    logger->log(LogLevel::DEBUG, "Card local position: " + 
        std::to_string(localPos.x) + "," + std::to_string(localPos.y));
    logger->log(LogLevel::DEBUG, "Card world position: " + 
        std::to_string(worldPos.x) + "," + std::to_string(worldPos.y));
    
    // 检查卡牌状态
    logger->log(LogLevel::DEBUG, "Card on field: " + std::to_string(card->getOnField()));
    
    // 清除之前的选择效果
    if (_selectionSprite) {
        logger->log(LogLevel::DEBUG, "Removing old selection effect");
        _selectionSprite->removeFromParent();
        _selectionSprite = nullptr;
    }
    
    _selectedCard = dynamic_cast<MinionCard*>(card);
    
    // 创建选择效果精灵
    _selectionSprite = Sprite::create("images/selection_effect.png");
    if (!_selectionSprite) {
        logger->log(LogLevel::ERR, "Failed to create selection sprite");
        return;
    }
    
    // 设置选择效果的位置和大小
    _selectionSprite->setPosition(card->getPosition().x+1000, card->getPosition().y+200);
    _selectionSprite->setContentSize(card->getContentSize());
    _selectionSprite->setScale(card->getScale());
    
    // 记录选择效果的信息

    logger->log(LogLevel::DEBUG, "Selection effect info - Position: " + 
        std::to_string(_selectionSprite->getPosition().x) + "," + 
        std::to_string(_selectionSprite->getPosition().y));
    logger->log(LogLevel::DEBUG, "Selection effect size: " + 
        std::to_string(_selectionSprite->getContentSize().width) + "x" + 
        std::to_string(_selectionSprite->getContentSize().height));
    logger->log(LogLevel::DEBUG, "Selection effect scale: " + 
        std::to_string(_selectionSprite->getScale()));
    
    // 将选择效果添加到场景
    this->addChild(_selectionSprite, 100);
    
    logger->log(LogLevel::DEBUG, "Selection effect added successfully");
}


//初始选择目标函数，目前无用
//void GameScene::handleTargetSelection(const Vec2& touchPos) {
//    auto logger = GameLogger::getInstance();
//    logger->log(LogLevel::DEBUG, "Handling target selection at position: x=" + 
//        std::to_string(touchPos.x) + ", y=" + std::to_string(touchPos.y));
//
//    if (!_selectedCard) {
//        logger->log(LogLevel::DEBUG, "No card selected, cannot handle target selection");
//        return;
//    }
//
//    logger->log(LogLevel::DEBUG, "Selected card: " + _selectedCard->getName());
//
//    // 检查对手场上的随从
//    for (auto targetCard : _opponentFieldCards) {
//        if (!targetCard) {
//            logger->log(LogLevel::DEBUG, "Found null card in opponent field, skipping");
//            continue;
//        }
//
//        // 获取目标卡牌的位置和边界
//        Vec2 targetWorldPos = targetCard->getParent()->convertToWorldSpace(targetCard->getPosition());
//        Size targetSize = targetCard->getContentSize();
//        float targetScale = targetCard->getScale();
//
//        Rect targetRect(
//            targetWorldPos.x - (targetSize.width * targetScale) / 2,
//            targetWorldPos.y - (targetSize.height * targetScale) / 2,
//            targetSize.width * targetScale,
//            targetSize.height * targetScale
//        );
//
//        logger->log(LogLevel::DEBUG, "Checking target card: " + targetCard->getName() + 
//            " at position: x=" + std::to_string(targetWorldPos.x) + 
//            ", y=" + std::to_string(targetWorldPos.y));
//
//        if (targetRect.containsPoint(touchPos)) {
//            logger->log(LogLevel::DEBUG, "Target found: " + targetCard->getName());
//            
//            // 如果是随从卡牌，执行攻击
//            auto attackingMinion = dynamic_cast<MinionCard*>(_selectedCard);
//            auto targetMinion = dynamic_cast<MinionCard*>(targetCard);
//            
//            if (attackingMinion && targetMinion) {
//                logger->log(LogLevel::DEBUG, "Attacking minion: " + attackingMinion->getName() + 
//                    " -> " + targetMinion->getName());
//                attackCard(_selectedCard, targetCard);
//            } else {
//                logger->log(LogLevel::DEBUG, "Invalid target type - attacker is minion: " + 
//                    std::to_string(attackingMinion != nullptr) + 
//                    ", target is minion: " + std::to_string(targetMinion != nullptr));
//            }
//
//            clearSelection();
//            return;
//        }
//    }
//
//    logger->log(LogLevel::DEBUG, "No valid target found at touch position");
//}

void GameScene::clearSelection() {
    if (_selectionSprite) {
        _selectionSprite->removeFromParent();
        _selectionSprite = nullptr;
    }
    if (_targetSprite) {
        _targetSprite->removeFromParent();
        _targetSprite = nullptr;
    }
    _selectedCard = nullptr;
}

// 修改触摸事件处理
bool GameScene::onTouchBegan(Touch* touch, Event* event) {
    if (!_isPlayerTurn) {
        return false;  // 不是玩家回合时不处理触摸
    }
    
    Vec2 touchPos = touch->getLocation();
    
    // 如果已经有选中的卡牌，尝试选择目标
    if (_selectedCard) {
        //handleTargetSelection(touchPos);  // 在这里调用 handleTargetSelection
        return true;
    }
    
    // 否则尝试选择场上的卡牌
    for (auto& card : _playerFieldCards) {
        if (card && card->getBoundingBox().containsPoint(touchPos)) {
            handleCardSelection(card, touchPos);  // 在这里调用 handleCardSelection
            return true;
        }
    }
    
    return false;
}

void GameScene::onTouchMoved(Touch* touch, Event* event) {
    // 可以添加拖动效果
}

void GameScene::onTouchEnded(Touch* touch, Event* event) {
    // 可以添加额外的清理逻辑
}

bool GameScene::init() {
    if (!Scene::init()) {
        return false;
    }
    
    auto logger = GameLogger::getInstance();
    
    // 初始化选择相关的变量
    _selectedCard = nullptr;
    _selectionSprite = nullptr;
    _targetSprite = nullptr;
    
    // 设置手牌触摸监听器（用于出牌）
    auto handListener = EventListenerTouchOneByOne::create();
    handListener->setSwallowTouches(false);  // 如果处理了触摸事件，阻止事件继续传递
    
    handListener->onTouchBegan = [this, logger](Touch* touch, Event* event) {
        Vec2 worldTouchPos = touch->getLocation();
        logger->log(LogLevel::DEBUG, "手牌区域触摸检测开始 --------");
        
        // 遍历所有手牌检查点击
        for (auto card : _playerHand) {
            if (!card) {
                logger->log(LogLevel::DEBUG, "跳过空卡牌");
                continue;
            }

            // 获取卡牌在世界坐标系中的位置和边界
            Vec2 cardWorldPos = card->getParent()->convertToWorldSpace(card->getPosition());
            Size cardSize = card->getContentSize();
            float scale = card->getScale();
            
            Rect cardRect(
                cardWorldPos.x - (cardSize.width * scale) / 2,
                cardWorldPos.y - (cardSize.height * scale) / 2,
                cardSize.width * scale,
                cardSize.height * scale
            );
            
            if (cardRect.containsPoint(worldTouchPos)) {
                logger->log(LogLevel::INFO, "选中手牌: " + card->getName());
                logger->log(LogLevel::INFO, "卡牌信息 --------");
                logger->log(LogLevel::INFO, "费用: " + std::to_string(card->getCost()));
                
                if (_playerFieldCards.size() < 7) {
                    logger->log(LogLevel::INFO, "尝试打出卡牌到场上");
                    auto visibleSize = Director::getInstance()->getVisibleSize();
                    Vec2 fieldPos = Vec2(visibleSize.width / 2, FIELD_Y);
                    
                    // 记录场上随从数量
                    logger->log(LogLevel::INFO, "当前场上随从数量: " + 
                        std::to_string(_playerFieldCards.size()));
                    
                    card->onField();
                    playCardToField(card, fieldPos);
                    
                    logger->log(LogLevel::INFO, "卡牌已成功打出到场上");
                } else {
                    logger->log(LogLevel::WARNING, "场上随从已满，无法打出更多随从");
                }
                return true;
            }
        }
        
        logger->log(LogLevel::DEBUG, "手牌区域触摸检测结束 --------");
        return true;
    };
    
    // 设置场上随从触摸监听器（用于选择攻击）
    auto fieldListener = EventListenerTouchOneByOne::create();
    fieldListener->setSwallowTouches(false);  // 不吞噬事件，允许事件继续传递
    
    fieldListener->onTouchBegan = [this, logger](Touch* touch, Event* event) {
        if (!_isPlayerTurn) {
            logger->log(LogLevel::DEBUG, "Not player's turn, skipping touch");
            return false;
        }

        Vec2 worldTouchPos = touch->getLocation();
        logger->log(LogLevel::DEBUG, "Field touch position: x=" + std::to_string(worldTouchPos.x) + 
            ", y=" + std::to_string(worldTouchPos.y));

        // 如果已经选中了我方随从，检查是否点击敌方随从
        if (_selectedCard) {
            logger->log(LogLevel::DEBUG, "检查敌方随从位置 --------");
            // 检查是否点击了敌方随从
            for (auto enemyCard : _opponentFieldCards2) {
                if (!enemyCard) {
                    logger->log(LogLevel::DEBUG, "跳过空的敌方随从");
                    continue;
                }
                
                logger->log(LogLevel::DEBUG, "检查敌方随从: " + enemyCard->getName());
                
                // 获取敌方卡牌在世界坐标系中的位置和边界
                Vec2 cardWorldPos = enemyCard->getParent()->convertToWorldSpace(enemyCard->getPosition());
                Size cardSize = enemyCard->getContentSize();
                float scale = enemyCard->getScale();
                
                Rect cardRect(
                    cardWorldPos.x - (cardSize.width * scale) / 2,
                    cardWorldPos.y - (cardSize.height * scale) / 2,
                    cardSize.width * scale,
                    cardSize.height * scale
                );

                logger->log(LogLevel::DEBUG, "敌方随从位置 - 世界坐标: x=" + std::to_string(cardWorldPos.x) + 
                    ", y=" + std::to_string(cardWorldPos.y));
                logger->log(LogLevel::DEBUG, "敌方随从碰撞箱 - x=" + std::to_string(cardRect.origin.x) + 
                    ", y=" + std::to_string(cardRect.origin.y) + 
                    ", width=" + std::to_string(cardRect.size.width) + 
                    ", height=" + std::to_string(cardRect.size.height));

                _targetSprite = Sprite::create("images/target_effect.png");
                

                // 设置选择效果的位置和大小
                _targetSprite->setPosition(enemyCard->getPosition().x , enemyCard->getPosition().y );
                _targetSprite->setContentSize(enemyCard->getContentSize());
                _targetSprite->setScale(0.5);

                // 记录选择效果的信息

                logger->log(LogLevel::DEBUG, "Selection effect info - Position: " +
                    std::to_string(_targetSprite->getPosition().x) + "," +
                    std::to_string(_targetSprite->getPosition().y));
                logger->log(LogLevel::DEBUG, "Selection effect size: " +
                    std::to_string(_targetSprite->getContentSize().width) + "x" +
                    std::to_string(_targetSprite->getContentSize().height));
                logger->log(LogLevel::DEBUG, "Selection effect scale: " +
                    std::to_string(_targetSprite->getScale()));

                // 将选择效果添加到场景
                

                if (cardRect.containsPoint(worldTouchPos)) {
                    logger->log(LogLevel::INFO, "点击命中敌方随从: " + enemyCard->getName());
                    this->addChild(_targetSprite, 100);
                    _selectedCard->attackTarget(enemyCard);
                    

                    auto gameManager = GameManager::getInstance();
                    auto currentPlayer = gameManager->getCurrentPlayer();
                    if (isStart)
                    {
                        if (gameManager->isGameOver()) {  // 使用提供的 isGameOver() 方法
                            showGameOverUI();
                        }
                    }

                    clearSelection();
                } else {
                    logger->log(LogLevel::DEBUG, "未命中此敌方随从");
                }
            }
        }

        // 检查是否点击我方随从
        logger->log(LogLevel::DEBUG, "检查我方随从位置 --------");
        for (auto card : _playerHand2) {
            if (!card) {
                logger->log(LogLevel::DEBUG, "跳过空的我方随从");
                continue;
            }
            
            logger->log(LogLevel::DEBUG, "检查我方随从: " + card->getName());
            
            Vec2 cardWorldPos = card->getParent()->convertToWorldSpace(card->getPosition());
            Size cardSize = card->getContentSize();
            float scale = card->getScale();
            
            Rect cardRect(
                cardWorldPos.x - (cardSize.width * scale) / 2,
                cardWorldPos.y - (cardSize.height * scale) / 2,
                cardSize.width * scale,
                cardSize.height * scale
            );

            logger->log(LogLevel::DEBUG, "我方随从位置 - 世界坐标: x=" + std::to_string(cardWorldPos.x) + 
                ", y=" + std::to_string(cardWorldPos.y));
            logger->log(LogLevel::DEBUG, "我方随从碰撞箱 - x=" + std::to_string(cardRect.origin.x) + 
                ", y=" + std::to_string(cardRect.origin.y) + 
                ", width=" + std::to_string(cardRect.size.width) + 
                ", height=" + std::to_string(cardRect.size.height));

            if (cardRect.containsPoint(worldTouchPos)) {
                logger->log(LogLevel::INFO, "点击命中我方随从: " + card->getName());
                handleCardSelection(card, worldTouchPos);
                return true;
            }
        }
        
        logger->log(LogLevel::DEBUG, "未找到任何可选中的随从");
        return false;
    };
    
    // 添加监听器，设置不同的优先级
    _eventDispatcher->addEventListenerWithSceneGraphPriority(handListener, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(fieldListener, this);
    
    return true;
}

void GameScene::removeCardFromField(Card* card) {
    auto logger = GameLogger::getInstance();
    
    // 从玩家场上移除
    auto it = std::find(_playerFieldCards.begin(), _playerFieldCards.end(), card);
    if (it != _playerFieldCards.end()) {
        _playerFieldCards.erase(it);
        logger->log(LogLevel::DEBUG, "Card removed from player field");
        return;
    }
    
    // 从对手场上移除
    auto it2 = std::find(_opponentFieldCards.begin(), _opponentFieldCards.end(), card);
    if (it2 != _opponentFieldCards.end()) {
        _opponentFieldCards.erase(it2);
        logger->log(LogLevel::DEBUG, "Card removed from opponent field");
    }
    
    // 更新场上卡牌的位置
    updateFieldCardPositions();
}

void GameScene::onSettingsClicked(Ref* sender)
{
    showGameOverUI2();
}

void GameScene::showGameOverUI2() {
    auto gameManager = GameManager::getInstance();
    auto result = gameManager->getGameResult();  // 使用游戏结果枚举

    // 创建半透明背景
    auto dimLayer = LayerColor::create(Color4B(0, 0, 0, 128));
    this->addChild(dimLayer, 100);

    // 创建游戏结束标签
    auto winLabel = Label::createWithTTF( "Defeat", "fonts/arial.ttf", 72 );
    winLabel->setPosition(Director::getInstance()->getVisibleSize() / 2);
    winLabel->setTextColor((result == GameResult::VICTORY) ? Color4B::YELLOW : Color4B::RED);
    dimLayer->addChild(winLabel);

    // 创建渐变黑屏效果
    auto fadeToBlack = Sequence::create(
        DelayTime::create(2.0f),  // 等待2秒让玩家看清结果
        CallFunc::create([dimLayer]() {
            // 开始渐变到全黑
            dimLayer->runAction(FadeTo::create(3.0f, 255));
            }),
        DelayTime::create(3.0f),  // 等待渐变完成
        CallFunc::create([]() {
            // 退出程序
            Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            exit(0);
#endif
            }),
        nullptr
    );

    // 运行动作序列
    this->runAction(fadeToBlack);

    // 禁用所有输入
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](Touch* touch, Event* event) {
        return true;  // 吞掉所有触摸事件
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}