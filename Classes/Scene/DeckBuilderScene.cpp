#include "DeckBuilderScene.h"
#include "Utils/Constants.h"
#include "Audio/AudioManager.h"
#include"MenuScene.h"

USING_NS_CC;
// 创建场景的静态方法
Scene* DeckBuilderScene::createScene() {
    return DeckBuilderScene::create();
}
// 场景初始化方法
bool DeckBuilderScene::init() {
    if (!Scene::init()) {
        return false;
    }
    // 初始化场景的三个主要组件
    DeckBuilderScene::initBackground1();// 初始化背景
    DeckBuilderScene::initMenu1(); // 初始化菜单按钮
    DeckBuilderScene::initUI1();// 初始化UI元素
    return true;
}

void DeckBuilderScene::initBackground1() { // 获取屏幕尺寸和原点位置
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // 创建并设置背景精灵
    auto background = Sprite::create(GameConstants::Resources::BACKGROUND_DECKBUILDER);
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x,
        visibleSize.height / 2 + origin.y));
    this->addChild(background);
}

void DeckBuilderScene::initMenu1()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //翻页按钮，下一页
    auto pageRight = MenuItemImage::create(
        "buttons/Button_For_DeckBuidler_Right.png",
        "buttons/Button_For_DeckBuidler_Right.png",
        CC_CALLBACK_1(DeckBuilderScene::onPageRight, this));

    //翻页按钮，上一页
    auto pageLeft = MenuItemImage::create(
        "buttons/Button_For_DeckBuilder_Left.png",
        "buttons/Button_For_DeckBuilder_Left.png",
        CC_CALLBACK_1(DeckBuilderScene::onPageLeft, this));

    //返回按钮，返回主菜单
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

    // 添加标题
    auto titleLabel = Label::createWithTTF("My Collections", "fonts/Marker Felt.ttf", 43);
    if (titleLabel) {
        titleLabel->setPosition(Vec2(origin.x + visibleSize.width / 2+75,
            origin.y + visibleSize.height - titleLabel->getContentSize().height-26));
        titleLabel->setTextColor(Color4B(0, 0, 0, 255));
        this->addChild(titleLabel, 1);
    }

}

//翻页-下一页函数
void DeckBuilderScene::onPageRight(cocos2d::Ref* sender)
{

}

//翻页-上一页函数
void DeckBuilderScene::onPageLeft(cocos2d::Ref* sender)
{

}

//返回主菜单函数
void DeckBuilderScene::onReturnMenu(cocos2d::Ref* sender)
{
    auto scene = MenuScene::createScene();
    Director::getInstance()->replaceScene(scene);
}