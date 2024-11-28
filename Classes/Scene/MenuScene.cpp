#include "MenuScene.h"
#include "GameScene.h"
#include "Utils/Constants.h"
#include "Audio/AudioManager.h"

USING_NS_CC;
// ���������ľ�̬����
Scene* MenuScene::createScene() {
    return MenuScene::create();
}
// ������ʼ������
bool MenuScene::init() {
    if (!Scene::init()) {
        return false;
    }
    // ��ʼ��������������Ҫ���
    initBackground();// ��ʼ������
    initMenu(); // ��ʼ���˵���ť
    initUI();// ��ʼ��UIԪ��
    return true;
}

void MenuScene::initBackground() { // ��ȡ��Ļ�ߴ��ԭ��λ��
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // ���������ñ�������
    auto background = Sprite::create(GameConstants::Resources::BACKGROUND);
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x,
        visibleSize.height / 2 + origin.y));
    this->addChild(background);
}
// ��ʼ���˵���ť
void MenuScene::initMenu() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // �����ĸ���Ҫ�˵����ʼ��Ϸ�����鹹�������ú��˳�
       // �������ǵĴ�ֱ����λ��
       // ��ӵ���ص�����
       // ��������Ч��
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

    // ����λ��
    float spacing = 80.0f;
    playItem->setPosition(Vec2(0, spacing * 1.5f));
    deckItem->setPosition(Vec2(0, spacing * 0.5f));
    settingsItem->setPosition(Vec2(0, -spacing * 0.5f));
    quitItem->setPosition(Vec2(0, -spacing * 1.5f));

    // �����˵�
    auto menu = Menu::create(playItem, deckItem, settingsItem, quitItem, nullptr);
    menu->setPosition(Vec2(visibleSize.width / 2 + origin.x,
        visibleSize.height / 2 + origin.y));

    this->addChild(menu);

    // ���Ų˵�����
    playMenuAnimation();
}
// ��ʼ��UIԪ�أ�����Ͱ汾�ţ�
void MenuScene::initUI() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ��ӱ���
    auto titleLabel = Label::createWithTTF("HearthStone", "fonts/arial.ttf", 48);
    if (titleLabel) {
        titleLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - titleLabel->getContentSize().height));
        this->addChild(titleLabel, 1);
    }

    // ��Ӱ汾��
    auto versionLabel = Label::createWithTTF("Version 1.0", "fonts/arial.ttf", 24);
    if (versionLabel) {
        versionLabel->setPosition(Vec2(origin.x + visibleSize.width - 100,
            origin.y + 30));
        this->addChild(versionLabel, 1);
    }
    // �˵���ص�����
}void MenuScene::onDeckBuilder(Ref* sender) {
    AudioManager::getInstance()->playEffect(GameConstants::Sound::BUTTON_CLICK);
    // TODO: ʵ�ֿ��鹹�������л�
}

void MenuScene::onSettings(Ref* sender) {
    AudioManager::getInstance()->playEffect(GameConstants::Sound::BUTTON_CLICK);
    // TODO: ʵ�����ó����л�
}

void MenuScene::onQuit(Ref* sender) {
    AudioManager::getInstance()->playEffect(GameConstants::Sound::BUTTON_CLICK);
    Director::getInstance()->end();    // ���Ű�ť�����Ч
    // �˳���Ϸ
}

void MenuScene::playMenuAnimation() {
    // Ϊ�˵�����ӽ��붯��
    for (const auto& child : this->getChildren()) {
        child->setOpacity(0);
        child->runAction(FadeIn::create(0.5f));
    }
}

void MenuScene::onPlayGame(Ref* sender) {
    AudioManager::getInstance()->playEffect(GameConstants::Sound::BUTTON_CLICK);

    // �л�����Ϸ����
    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
}
void GameScene::initListeners() {
    // ��ʼ���¼�������
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [](Touch* touch, Event* event) {
        return true;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameScene::updateUI(float dt) {
    // ����UI�߼�
    // ������Ը�������ֵ������ˮ������ʾ
}

void GameScene::onEndTurnClicked(Ref* sender) {
    // ��������غϰ�ť���
    AudioManager::getInstance()->playEffect(GameConstants::Sound::BUTTON_CLICK);
    // TODO: ��ӽ����غ��߼�
}

void GameScene::onSettingsClicked(Ref* sender) {
    // �������ð�ť���
    AudioManager::getInstance()->playEffect(GameConstants::Sound::BUTTON_CLICK);
    // TODO: ��ʾ���ò˵�
}