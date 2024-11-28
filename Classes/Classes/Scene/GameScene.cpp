// ���������ͷ�ļ�
#include "GameScene.h"          // ��Ϸ�����������
#include "Game/GameManager.h"   // ��Ϸ������
#include "Card/Card.h"          // ������
#include "Player/Player.h"      // �����
#include "Utils/Constants.h"    // ��Ϸ����
#include "Animation/AnimationManager.h"  // ����������

// ʹ�� cocos2d �����ռ�
USING_NS_CC;

// ���������ľ�̬����
Scene* GameScene::createScene() {
    return GameScene::create();  // ���������س���ʵ��
}

// ������ʼ������
bool GameScene::init() {
    // ���ø���ĳ�ʼ������
    if (!Scene::init()) {
        return false;  // ��������ʼ��ʧ�ܣ�����false
    }

    // ��ʼ�������ĸ������
    initLayers();     // ��ʼ����Ϸ�㼶
    initUI();         // ��ʼ���û�����
    initListeners();  // ��ʼ���¼�������

    // ����UI���¶�ʱ����ÿ�����һ��updateUI����
    this->schedule(CC_SCHEDULE_SELECTOR(GameScene::updateUI), 1.0f);

    return true;  // ��ʼ���ɹ�
}

// ��ʼ����Ϸ�㼶
void GameScene::initLayers() {
    // ��ȡ��Ļ�ɼ������С
    Size visibleSize = Director::getInstance()->getVisibleSize();

    // ������Ϸ����
    _gameLayer = Node::create();  // ������Ϸ��ڵ�
    _gameLayer->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));  // ����λ��Ϊ��Ļ����
    this->addChild(_gameLayer);   // ����Ϸ����ӵ�����

    // ����UI��
    _uiLayer = Node::create();    // ����UI��ڵ�
    this->addChild(_uiLayer);     // ��UI����ӵ�����

    // �������������
    _playerHand = Node::create();  // ����������ƽڵ�
    _playerHand->setPosition(Vec2(visibleSize.width / 2, 100));  // ����λ���ڵײ�
    _gameLayer->addChild(_playerHand);  // ��ӵ���Ϸ��

    // ������ҳ���
    _playerField = Node::create();  // ������ҳ��ؽڵ�
    _playerField->setPosition(Vec2(visibleSize.width / 2, 300));  // ����λ��
    _gameLayer->addChild(_playerField);  // ��ӵ���Ϸ��

    // ���������������ͳ���
    _opponentHand = Node::create();    // �����������ƽڵ�
    _opponentField = Node::create();   // �������ֳ��ؽڵ�
    // ���ö�������λ�ã�����Ļ�Ϸ���
    _opponentHand->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 100));
    _opponentField->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 300));
    _gameLayer->addChild(_opponentHand);    // ��Ӷ�������������Ϸ��
    _gameLayer->addChild(_opponentField);   // ��Ӷ��ֳ��ص���Ϸ��
}

// ��ʼ��UIԪ��
void GameScene::initUI() {
    // ��ȡ��Ļ�ɼ������С
    Size visibleSize = Director::getInstance()->getVisibleSize();

    // �����غϽ�����ť
    _endTurnButton = MenuItemImage::create(
        "buttons/end_turn_normal.png",      // ��ť����״̬��ͼƬ
        "buttons/end_turn_pressed.png",     // ��ť����״̬��ͼƬ
        CC_CALLBACK_1(GameScene::onEndTurnClicked, this)  // ����ص�����
    );
    // ���ûغϽ�����ťλ��
    _endTurnButton->setPosition(Vec2(visibleSize.width - 100, visibleSize.height / 2));

    // �������ð�ť
    _settingsButton = MenuItemImage::create(
        "buttons/settings_normal.png",      // ��ť����״̬��ͼƬ
        "buttons/settings_pressed.png",     // ��ť����״̬��ͼƬ
        CC_CALLBACK_1(GameScene::onSettingsClicked, this)  // ����ص�����
    );
    // �������ð�ťλ��
    _settingsButton->setPosition(Vec2(50, visibleSize.height - 50));

    // �����������а�ť�Ĳ˵�
    auto menu = Menu::create(_endTurnButton, _settingsButton, nullptr);
    menu->setPosition(Vec2::ZERO);  // ���ò˵�λ��Ϊԭ��
    _uiLayer->addChild(menu);       // ���˵���ӵ�UI��

    // �����غϼ�ʱ����ǩ
    _turnTimerLabel = Label::createWithTTF("90", "fonts/arial.ttf", 32);  // ������ʱ���ı�
    _turnTimerLabel->setPosition(Vec2(visibleSize.width - 100, visibleSize.height / 2 + 50));  // ����λ��
    _uiLayer->addChild(_turnTimerLabel);  // ��ӵ�UI��

    // ���������Ϣ��ǩ
    _playerManaLabel = Label::createWithTTF("0/0", "fonts/arial.ttf", 24);     // ��������ֵ��ǩ
    _playerHealthLabel = Label::createWithTTF("30", "fonts/arial.ttf", 24);    // ��������ֵ��ǩ
    _playerManaLabel->setPosition(Vec2(100, 100));     // ���÷���ֵ��ǩλ��
    _playerHealthLabel->setPosition(Vec2(100, 150));   // ��������ֵ��ǩλ��
    _uiLayer->addChild(_playerManaLabel);     // ��ӷ���ֵ��ǩ��UI��
    _uiLayer->addChild(_playerHealthLabel);   // �������ֵ��ǩ��UI��
}