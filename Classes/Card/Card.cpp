// Card.cpp
// 卡牌基类的实现文件，包含卡牌的基础功能实现
#pragma execution_character_set("utf-8")
#include "Card.h"
#include "Manager/GameManager.h"
#include "Animation/AnimationManager.h"
#include "Utils/ResourceManager.h"
#include "Player/Player.h"
#include "Effect/IEffect.h"  // 在实现文件中包含完整定义
// 静态工厂方法：创建卡牌实例
// @param id: 卡牌唯一标识符
// @param name: 卡牌名称
// @return: 返回创建的卡牌指针，失败返回nullptr
Card* Card::create(int id, const std::string& name) {
    auto logger = GameLogger::getInstance();
    logger->log(LogLevel::DEBUG, "Creating Card instance");
    
    Card* card = new (std::nothrow) Card();
    if (card && card->init(id, name)) {
        card->autorelease();
        logger->log(LogLevel::DEBUG, "Card created successfully");
        return card;
    }
    
    logger->log(LogLevel::ERR, "Failed to create Card");
    CC_SAFE_DELETE(card);
    return nullptr;
}

// 初始化卡牌
// @param id: 卡牌ID
// @param name: 卡牌名称
// @return: 初始化成功返回true，失败返回false
bool Card::init(int id, const std::string& name) {
    if (!Node::init()) return false;
    
    _id = id;
    _name = name;
    
    // 设置卡牌的默认大小
    this->setContentSize(Size(80, 100));  // 调整合适的大小
    
    return true;
}

// 初始化卡牌UI元素
//void Card::initUI() {
//    auto logger = GameLogger::getInstance();
//    logger->log(LogLevel::DEBUG, "Starting Card UI initialization");
//    
//    try {
//        // 创建并设置卡牌名称标签
//        _nameLabel = Label::createWithTTF(_name, "fonts/STKAITI.TTF", 20);
//        if (_nameLabel) {
//            _nameLabel->setPosition(Vec2(this->getContentSize().width / 2,
//                this->getContentSize().height - 55));
//            _nameLabel->setTextColor(Color4B::WHITE);
//            _nameLabel->enableOutline(Color4B::BLACK, 1);
//            this->addChild(_nameLabel,2);
//            logger->log(LogLevel::DEBUG, "Name label created and added");
//        } else {
//            logger->log(LogLevel::WARNING, "Failed to create name label");
//        }
//
//        // 创建并设置法力值消耗标签
//        _costLabel = Label::createWithTTF(std::to_string(_cost), "fonts/arial.ttf", 30);//!!！！！！！！！！！！！！！！！！!!!!!!!!!!!
//        if (_costLabel) {
//            _costLabel->setPosition(Vec2(-23, this->getContentSize().height + 60));
//            _costLabel->setTextColor(Color4B::WHITE);  // 改为白色
//            _costLabel->enableOutline(Color4B::BLACK, 2);  // 保持黑色描边
//            _costLabel->enableShadow(Color4B::BLACK);  // 保持阴影
//            this->addChild(_costLabel,2);
//            logger->log(LogLevel::DEBUG, "Cost label created and added");
//        } else {
//            logger->log(LogLevel::WARNING, "Failed to create cost label");
//        }
//
//        // 创建并设置卡牌描述标签                                                                         待修改！！！！！！！
//        _descriptionLabel = Label::createWithTTF(_description, "fonts/STKAITI.TTF", 50);
//        if (_descriptionLabel) {
//            _descriptionLabel->setPosition(Vec2(this->getContentSize().width / 2, +530));
//            _descriptionLabel->setDimensions(this->getContentSize().width + 120, 120);//文本框大小
//            _descriptionLabel->setTextColor(Color4B::BLACK);  // 改为黑色
//            _descriptionLabel->enableOutline(Color4B::WHITE, 1);  // 改为白色描边
//            _descriptionLabel->setAlignment(TextHAlignment::CENTER);
//            this->addChild(_descriptionLabel,2);
//            logger->log(LogLevel::DEBUG, "Description label created and added");
//        } else {
//            logger->log(LogLevel::WARNING, "Failed to create description label");
//        }
//        
//        if (_id == 1003 || _id == 1006 || _id == 1008 || _id == 1011 || _id == 1016 ||
//            _id == 2004 || _id == 1013 || _id == 1015 || _id == 2001 || _id == 2008 || (_id >= 2009 && _id <= 2011) || _id == 2013 || _id == 2014) {
//            //// 攻击力标签
//            //auto attackLabel = Label::createWithTTF(std::to_string(attack), "fonts/arial.ttf", 70);
//            //attackLabel->setPosition(Vec2(-60, -140));
//            //attackLabel->setTextColor(Color4B::WHITE);  // 改为白色
//            //attackLabel->enableOutline(Color4B::BLACK, 2);
//            //attackLabel->enableShadow(Color4B::BLACK);
//            //this->addChild(attackLabel, 2);
//
//            //// 生命值标签
//            //auto healthLabel = Label::createWithTTF(std::to_string(health), "fonts/arial.ttf", 72);
//            //healthLabel->setPosition(Vec2(this->getContentSize().width + 110, -140));
//            //healthLabel->setTextColor(Color4B::WHITE);  // 改为白色
//            //healthLabel->enableOutline(Color4B::BLACK, 2);
//            //healthLabel->enableShadow(Color4B::BLACK);
//            //this->addChild(healthLabel, 2);
//        }
//
//        logger->log(LogLevel::DEBUG, "Card UI initialization complete");
//    } catch (const std::exception& e) {
//        logger->log(LogLevel::ERR, "Exception in Card UI initialization: " + std::string(e.what()));
//        throw;
//    }
//}

// 检查卡牌是否可以打出
// @return: 可以打出返回true，否则返回false
bool Card::canPlay() const {
    // 获取游戏管理器实例
    auto gameManager = GameManager::getInstance();

    // 获取当前玩家
    Player* currentPlayer = gameManager->getCurrentPlayer();
    if (!currentPlayer) {
        return false;  // 安全检查：如果没有当前玩家，返回false
    }

    // 检查多个条件
    bool isCardPlayable = _isPlayable;                  // 卡牌是否可用
    bool hasSufficientMana = currentPlayer->getMana() >= _cost;  // 法力值是否足够
    bool isPlayersTurn = (_owner == currentPlayer);     // 是否是玩家的回合

    // 所有条件都满足才能打出卡牌
    return isCardPlayable && hasSufficientMana && isPlayersTurn;
}

// 卡牌打出时的处理
void Card::onPlay() {
    // 播放卡牌使用动画
    AnimationManager::getInstance()->playCardAnimation(this);

    // 触发卡牌上的所有效果
    triggerEffects();
}

// 触发卡牌上的所有效果
void Card::triggerEffects() {
    // 遍历所有效果并尝试触发
    for (auto& effect : _effects) {
        if (effect->canActivate()) {
            effect->onActivate();
        }
    }
}

// 触摸开始事件处理
// @param touch: 触摸信息
// @param event: 事件信息
// @return: 是否处理了触摸事件
bool Card::onTouchBegan(Touch* touch, Event* event) {
    // 将触摸点转换为节点坐标系
    Vec2 locationInNode = convertToNodeSpace(touch->getLocation());
    Size s = getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);

    // 检查触摸点是否在卡牌范围内
    if (rect.containsPoint(locationInNode)) {
        _isSelected = true;           // 设置选中状态
        this->setScale(1.2f);         // 放大显示选中的卡牌
        return true;
    }
    return false;
}
void Card::onDiscard() {
    // 卡牌被丢弃时的处理
    if (_owner) {
        _owner->removeCardFromHand(this);
    }
    // 播放丢弃动画
    AnimationManager::getInstance()->playCardAnimation(this);
}

void Card::heal(int amount) {
    int newHealth = getHealth() + amount;
    setHealth(std::min(newHealth, getMaxHealth()));
}

void Card::onDraw() {
    // 卡牌被抽到时的处理
    if (_owner) {
        // 可以添加抽牌动画或效果
        AnimationManager::getInstance()->playCardDrawAnimation(this);

        // 触发抽牌相关效果
        triggerEffects();
    }
}

void Card::onTouchMoved(Touch* touch, Event* event) {
    if (_isSelected) {
        // 更新卡牌位置
        Vec2 delta = touch->getDelta();
        this->setPosition(this->getPosition() + delta);

        // 检查是否可以打出
        if (canPlay()) {
            // 显示可打出的提示效果
            showPlayableHighlight();
        }
    }
}

void Card::onTouchEnded(Touch* touch, Event* event) {
    if (_isSelected) {
        _isSelected = false;
        this->setScale(1.0);  // 恢复原始大小

        // 检查是否在可打出的位置
        if (canPlay() && isInPlayableArea(touch->getLocation())) {
            onPlay();
        }
        else {
            // 返回原始位置
            returnToOriginalPosition();
        }
    }
}

// 辅助函数
void Card::showPlayableHighlight() {
    // 添加高亮效果
    if (!_highlightSprite) {
        _highlightSprite = Sprite::create("effects/card_highlight.png");
        this->addChild(_highlightSprite, -1);
    }
    _highlightSprite->setVisible(true);
}

void Card::returnToOriginalPosition() {
    // 使用动作返回原始位置
    auto moveTo = MoveTo::create(0.2f, _originalPosition);
    this->runAction(moveTo);
}

bool Card::isInPlayableArea(const Vec2& position) {
    // 检查位置是否在可打出区域
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Rect playableArea(visibleSize.width * 0.2f, visibleSize.height * 0.3f,
        visibleSize.width * 0.6f, visibleSize.height * 0.4f);
    return playableArea.containsPoint(position);
}

void Card::addEffect(std::shared_ptr<IEffect> effect) {
    if (effect) {
        effect->setOwner(this);
        _effects.push_back(effect);
    }
}

void Card::takeDamage(int amount) {
    if (amount <= 0) return;  // 伤害必须为正数
    
    // 计算新的生命值
    int newHealth = _health - amount;
    setHealth(std::max(0, newHealth));  // 生命值不能低于0
    
    // 如果生命值降到0或以下，卡牌死亡
    if (_health <= 0) {
        // 通知游戏管理器处理卡牌死亡
        GameManager::getInstance()->handleCardDeath(this);
    }
}

bool Card::initSprite() {
    auto logger = GameLogger::getInstance();
    
    // 使用卡牌名称作为图片文件名
    std::string imagePath = "cards/portraits/" + _name + ".png";
    logger->log(LogLevel::DEBUG, "Loading card image: " + imagePath);
    
    // 检查文件是否存在
    if (!FileUtils::getInstance()->isFileExist(imagePath)) {
        logger->log(LogLevel::ERR, "Card image not found: " + imagePath);
        return false;
    }
    
    _sprite = Sprite::create(imagePath);
    if (!_sprite) {
        logger->log(LogLevel::ERR, "Failed to load card image: " + imagePath);
        return false;
    }
    
    // 设置精灵属性
    _sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    _sprite->setPosition(this->getContentSize() / 2);
    this->addChild(_sprite);
    
    logger->log(LogLevel::DEBUG, "Card sprite initialized successfully");
    return true;
}
