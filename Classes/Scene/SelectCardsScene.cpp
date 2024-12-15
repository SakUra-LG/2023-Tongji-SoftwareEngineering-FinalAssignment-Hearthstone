#include "SelectCardsScene.h"
#include "GameScene.h"
#include "Utils/Constants.h"
#include "Audio/AudioManager.h"
#include"MenuScene.h"

USING_NS_CC;
// ���������ľ�̬����
Scene* SelectCardsScene::createScene() {
    return SelectCardsScene::create();
}
// ������ʼ������
bool SelectCardsScene::init() {
    if (!Scene::init()) {
        return false;
    }
    // ��ʼ��������������Ҫ���
    SelectCardsScene::initBackground3();// ��ʼ������
    SelectCardsScene::initMenu3(); // ��ʼ���˵���ť
    SelectCardsScene::initUI3();// ��ʼ��UIԪ��
    return true;
}

void SelectCardsScene::initBackground3() { // ��ȡ��Ļ�ߴ��ԭ��λ��
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // ���������ñ�������
    auto background = Sprite::create(GameConstants::Resources::BACKGROUND_SELECTCARDS);
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x,
        visibleSize.height / 2 + origin.y));
    this->addChild(background);
}

void SelectCardsScene::initMenu3() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // �����ĸ���Ҫ�˵����ʼ��Ϸ�����鹹�������ú��˳�
       // �������ǵĴ�ֱ����λ��
       // ���ӵ���ص�����
       // ��������Ч��

    float upOffset = 70.0f;  // ��������ƫ����
    float spacing = 80.0f;


    // ���ò˵���ť
    auto backItem = MenuItemImage::create(
        "buttons/Button_Back_Normal.png",
        "buttons/Button_Back_Selected.png",
        CC_CALLBACK_1(SelectCardsScene::onBack, this));

    //TODO-����Ϲ����
    auto cards1Item= MenuItemImage::create(
        "buttons/Button_SelectCards_RENWUXIA.png",
        "buttons/Button_SelectCards_RENWUXIA.png",
        CC_CALLBACK_1(SelectCardsScene::onCards1, this));
    //TODO-DK����
    auto cards2Item = MenuItemImage::create(
        "buttons/Button_SelectCards_DK.png",
        "buttons/Button_SelectCards_DK.png",
        CC_CALLBACK_1(SelectCardsScene::onCards2, this));


    backItem->setPosition(Vec2(visibleSize.width  + origin.x-backItem->getContentSize().width,  origin.y + backItem->getContentSize().height));
    cards1Item->setPosition(Vec2(visibleSize.width / 2 + origin.x-650, visibleSize.height / 2 + origin.y + upOffset + 175));
    cards2Item->setPosition(Vec2(visibleSize.width / 2 + origin.x-200, visibleSize.height / 2 + origin.y + upOffset + 175));
    auto menu2 = Menu::create(backItem, cards1Item, cards2Item, nullptr);
    menu2->setPosition(Vec2(0, 0));

    this->addChild(menu2, 1);


    // ���Ų˵�����
    //playMenuAnimation();
}

void SelectCardsScene::initUI3() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ���ӱ���
    auto titleLabel = Label::createWithTTF("SelectYourCards", "fonts/arial.ttf", 28);
    if (titleLabel) {
        titleLabel->setPosition(Vec2(origin.x + visibleSize.width / 2-350,
            origin.y + visibleSize.height - titleLabel->getContentSize().height-15));
        this->addChild(titleLabel, 1);
    }

    // ���Ӱ汾��
    auto versionLabel = Label::createWithTTF("Version 1.0", "fonts/arial.ttf", 24);
    if (versionLabel) {
        versionLabel->setPosition(Vec2(versionLabel->getContentSize().width / 2,
            visibleSize.height - versionLabel->getContentSize().height / 2));
        this->addChild(versionLabel, 1);
    }


    // �˵���ص�����
}


void SelectCardsScene::onCards1(Ref* sender) {
    auto deckManager = DeckManager::getInstance();
    if (!deckManager) {
        CCLOG("Failed to get DeckManager instance");
        return;
    }

    auto deck = deckManager->createQuestDemonHunterDeck();
    if (!deck) {
        CCLOG("Failed to create Quest Demon Hunter deck");
        return;
    }

    auto scene = GameScene::createWithDeck(deck);
    if (scene) {
        Director::getInstance()->replaceScene(
            TransitionFade::create(0.5f, scene));
    }
}

void SelectCardsScene::onCards2(Ref* sender) {
    auto deckManager = DeckManager::getInstance();
    if (!deckManager) {
        CCLOG("Failed to get DeckManager instance");
        return;
    }

    auto deck = deckManager->createRainbowDKDeck();
    if (!deck) {
        CCLOG("Failed to create Rainbow DK deck");
        return;
    }

    auto scene = GameScene::createWithDeck(deck);
    if (scene) {
        Director::getInstance()->replaceScene(
            TransitionFade::create(0.5f, scene));
    }
}

void SelectCardsScene::onBack(Ref* sender)
{
    //л˵
    auto scene = MenuScene::createScene();
    Director::getInstance()->replaceScene(scene);
}