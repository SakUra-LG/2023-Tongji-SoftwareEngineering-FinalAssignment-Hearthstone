// Card.cpp
// ���ƻ����ʵ���ļ����������ƵĻ�������ʵ��

#include "Card.h"
#include "Manager/GameManager.h"
#include "Animation/AnimationManager.h"
#include "Utils/ResourceManager.h"
#include "Player/Player.h"
#include "Effect/IEffect.h"  // ��ʵ���ļ��а�����������
// ��̬������������������ʵ��
// @param id: ����Ψһ��ʶ��
// @param name: ��������
// @return: ���ش����Ŀ���ָ�룬ʧ�ܷ���nullptr
Card* Card::create(int id, const std::string& name) {
    Card* card = new (std::nothrow) Card();  // ʹ��nothrow�����쳣
    if (card && card->init(id, name)) {
        card->autorelease();  // �����Զ��ͷų�
        return card;
    }
    CC_SAFE_DELETE(card);    // ��ʼ��ʧ��ʱ��ȫɾ��
    return nullptr;
}

// ��ʼ������
// @param id: ����ID
// @param name: ��������
// @return: ��ʼ���ɹ�����true��ʧ�ܷ���false
bool Card::init(int id, const std::string& name) {
    // ���þ�����ĳ�ʼ��
    if (!Sprite::init()) {
        return false;
    }

    // ��ʼ����������
    _id = id;
    _cardName = name;
    _isPlayable = false;     // Ĭ�ϲ��ɴ��
    _isSelected = false;     // Ĭ��δѡ��
    _hasEffect = false;      // Ĭ������Ч
    _owner = nullptr;        // ��ʼ��������Ϊ��

    // ��ʼ������ֵ�������
    _health = 0;             // ��������Ĭ������ֵΪ0
    _maxHealth = 0;          // ��������Ĭ���������ֵΪ0

    // ��ʼ��UI���
    initUI();

    // ���ô����¼�������
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(Card::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(Card::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(Card::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

// ��ʼ������UIԪ��
void Card::initUI() {
    // ���������ÿ��ƿ�ܾ���
    _cardFrame = Sprite::create("cards/frame.png");
    this->addChild(_cardFrame);

    // ���������ÿ������Ʊ�ǩ
    _nameLabel = Label::createWithTTF(_cardName, "fonts/arial.ttf", 24);  // ʹ�� _cardName
    _nameLabel->setPosition(Vec2(0, 50));
    this->addChild(_nameLabel);

    // ���������÷���ֵ���ı�ǩ
    _costLabel = Label::createWithTTF(std::to_string(_cost), "fonts/arial.ttf", 32);
    _costLabel->setPosition(Vec2(-80, 80));
    this->addChild(_costLabel);

    // ���������ÿ���������ǩ
    _descriptionLabel = Label::createWithTTF(_description, "fonts/arial.ttf", 18);
    _descriptionLabel->setPosition(Vec2(0, -50));
    this->addChild(_descriptionLabel);
}

// ��鿨���Ƿ���Դ��
// @return: ���Դ������true�����򷵻�false
bool Card::canPlay() const {
    // ��ȡ��Ϸ������ʵ��
    auto gameManager = GameManager::getInstance();

    // ��ȡ��ǰ���
    Player* currentPlayer = gameManager->getCurrentPlayer();
    if (!currentPlayer) {
        return false;  // ��ȫ��飺���û�е�ǰ��ң�����false
    }

    // ���������
    bool isCardPlayable = _isPlayable;                  // �����Ƿ����
    bool hasSufficientMana = currentPlayer->getMana() >= _cost;  // ����ֵ�Ƿ��㹻
    bool isPlayersTurn = (_owner == currentPlayer);     // �Ƿ�����ҵĻغ�

    // ����������������ܴ������
    return isCardPlayable && hasSufficientMana && isPlayersTurn;
}

// ���ƴ��ʱ�Ĵ���
void Card::onPlay() {
    // ���ſ���ʹ�ö���
    AnimationManager::getInstance()->playCardAnimation(this);

    // ���������ϵ�����Ч��
    triggerEffects();
}

// ���������ϵ�����Ч��
void Card::triggerEffects() {
    // ��������Ч�������Դ���
    for (auto& effect : _effects) {
        if (effect->canActivate()) {
            effect->onActivate();
        }
    }
}

// ������ʼ�¼�����
// @param touch: ������Ϣ
// @param event: �¼���Ϣ
// @return: �Ƿ����˴����¼�
bool Card::onTouchBegan(Touch* touch, Event* event) {
    // ��������ת��Ϊ�ڵ�����ϵ
    Vec2 locationInNode = convertToNodeSpace(touch->getLocation());
    Size s = getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);

    // ��鴥�����Ƿ��ڿ��Ʒ�Χ��
    if (rect.containsPoint(locationInNode)) {
        _isSelected = true;           // ����ѡ��״̬
        this->setScale(1.2);         // �Ŵ���ʾѡ�еĿ���
        return true;
    }
    return false;
}
void Card::onDiscard() {
    // ���Ʊ�����ʱ�Ĵ���
    if (_owner) {
        _owner->removeCardFromHand(this);
    }
    // ���Ŷ�������
    AnimationManager::getInstance()->playCardAnimation(this);
}

void Card::heal(int amount) {
    int newHealth = getHealth() + amount;
    setHealth(std::min(newHealth, getMaxHealth()));
}

void Card::onDraw() {
    // ���Ʊ��鵽ʱ�Ĵ���
    if (_owner) {
        // ������ӳ��ƶ�����Ч��
        AnimationManager::getInstance()->playCardDrawAnimation(this);

        // �����������Ч��
        triggerEffects();
    }
}

bool Card::isActive() const {
    // ��鿨���Ƿ��ڿ���״̬
    return getIsActive() &&
        !getIsFrozen() &&
        !getIsSilenced();
}
void Card::onTouchMoved(Touch* touch, Event* event) {
    if (_isSelected) {
        // ���¿���λ��
        Vec2 delta = touch->getDelta();
        this->setPosition(this->getPosition() + delta);

        // ����Ƿ���Դ��
        if (canPlay()) {
            // ��ʾ�ɴ������ʾЧ��
            showPlayableHighlight();
        }
    }
}

void Card::onTouchEnded(Touch* touch, Event* event) {
    if (_isSelected) {
        _isSelected = false;
        this->setScale(1.0);  // �ָ�ԭʼ��С

        // ����Ƿ��ڿɴ����λ��
        if (canPlay() && isInPlayableArea(touch->getLocation())) {
            onPlay();
        }
        else {
            // ����ԭʼλ��
            returnToOriginalPosition();
        }
    }
}

// ��������
void Card::showPlayableHighlight() {
    // ��Ӹ���Ч��
    if (!_highlightSprite) {
        _highlightSprite = Sprite::create("effects/card_highlight.png");
        this->addChild(_highlightSprite, -1);
    }
    _highlightSprite->setVisible(true);
}

void Card::returnToOriginalPosition() {
    // ʹ�ö�������ԭʼλ��
    auto moveTo = MoveTo::create(0.2f, _originalPosition);
    this->runAction(moveTo);
}

bool Card::isInPlayableArea(const Vec2& position) {
    // ���λ���Ƿ��ڿɴ������
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Rect playableArea(visibleSize.width * 0.2f, visibleSize.height * 0.3f,
        visibleSize.width * 0.6f, visibleSize.height * 0.4f);
    return playableArea.containsPoint(position);
}