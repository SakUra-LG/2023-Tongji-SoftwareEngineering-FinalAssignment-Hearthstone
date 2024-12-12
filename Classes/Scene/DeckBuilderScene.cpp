#include "DeckBuilderScene.h"
#include "Utils/Constants.h"
#include "Audio/AudioManager.h"
#include"MenuScene.h"

USING_NS_CC;
// ���������ľ�̬����
Scene* DeckBuilderScene::createScene() {
    return DeckBuilderScene::create();
}
// ������ʼ������
bool DeckBuilderScene::init() {
    if (!Scene::init()) {
        return false;
    }
    // ��ʼ��������������Ҫ���
    DeckBuilderScene::initBackground1();// ��ʼ������
    DeckBuilderScene::initMenu1(); // ��ʼ���˵���ť
    DeckBuilderScene::initUI1();// ��ʼ��UIԪ��
    return true;
}

void DeckBuilderScene::initBackground1() { // ��ȡ��Ļ�ߴ��ԭ��λ��
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // ���������ñ�������
    auto background = Sprite::create(GameConstants::Resources::BACKGROUND_DECKBUILDER);
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x,
        visibleSize.height / 2 + origin.y));
    this->addChild(background);
}

void DeckBuilderScene::initMenu1()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //��ҳ��ť����һҳ
    auto pageRight = MenuItemImage::create(
        "buttons/Button_For_DeckBuidler_Right.png",
        "buttons/Button_For_DeckBuidler_Right.png",
        CC_CALLBACK_1(DeckBuilderScene::onPageRight, this));

    //��ҳ��ť����һҳ
    auto pageLeft = MenuItemImage::create(
        "buttons/Button_For_DeckBuilder_Left.png",
        "buttons/Button_For_DeckBuilder_Left.png",
        CC_CALLBACK_1(DeckBuilderScene::onPageLeft, this));

    //���ذ�ť���������˵�
    auto returnMenu = MenuItemImage::create(
        "buttons/Button_Back_Normal.png",
        "buttons/Button_Back_Selected.png",
        CC_CALLBACK_1(DeckBuilderScene::onReturnMenu, this));

    pageRight->setPosition(Vec2(1850, visibleSize.height/2));
    pageLeft->setPosition(Vec2(230, visibleSize.height / 2));
    returnMenu->setPosition(Vec2(visibleSize.width - returnMenu->getContentSize().width / 2,  returnMenu->getContentSize().height / 2));

    auto menu = Menu::create(pageRight, pageLeft, returnMenu, nullptr);
    menu->setPosition(Vec2(0,0));

    this->addChild(menu, 0);

}

void DeckBuilderScene::initUI1()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ���ӱ���
    auto titleLabel = Label::createWithTTF("My Collections", "fonts/Marker Felt.ttf", 43);
    if (titleLabel) {
        titleLabel->setPosition(Vec2(origin.x + visibleSize.width / 2+75,
            origin.y + visibleSize.height - titleLabel->getContentSize().height-26));
        titleLabel->setTextColor(Color4B(0, 0, 0, 255));
        this->addChild(titleLabel, 1);
    }

}

//��ҳ-��һҳ����
void DeckBuilderScene::onPageRight(cocos2d::Ref* sender)
{

}

//��ҳ-��һҳ����
void DeckBuilderScene::onPageLeft(cocos2d::Ref* sender)
{

}

//�������˵�����
void DeckBuilderScene::onReturnMenu(cocos2d::Ref* sender)
{
    auto scene = MenuScene::createScene();
    Director::getInstance()->replaceScene(scene);
}