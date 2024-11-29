#include "Settingscene1.h"
#include "GameScene.h"
#include "Utils/Constants.h"
#include "Audio/AudioManager.h"
#include"MenuScene.h"

USING_NS_CC;
// ���������ľ�̬����
Scene* Settingscene1::createScene() {
    return Settingscene1::create();
}
// ������ʼ������
bool Settingscene1::init() {
    if (!Scene::init()) {
        return false;
    }
    // ��ʼ��������������Ҫ���
    Settingscene1::initBackground1();// ��ʼ������
    Settingscene1::initMenu1(); // ��ʼ���˵���ť
    Settingscene1::initUI1();// ��ʼ��UIԪ��
    return true;
}

void Settingscene1::initBackground1() { // ��ȡ��Ļ�ߴ��ԭ��λ��
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // ���������ñ�������
    auto background = Sprite::create(GameConstants::Resources::BACKGROUND_SETTING);
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x,
        visibleSize.height / 2 + origin.y));
    this->addChild(background);
}

void Settingscene1::initMenu1() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // �����ĸ���Ҫ�˵����ʼ��Ϸ�����鹹�������ú��˳�
       // �������ǵĴ�ֱ����λ��
       // ��ӵ���ص�����
       // ��������Ч��

    float upOffset = 70.0f;  // ��������ƫ����
    float spacing = 80.0f;

    // ���ñ���ͼ
    auto settingBackground = Sprite::create("menu/SettingBackground.png");
    settingBackground->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + upOffset));

    this->addChild(settingBackground, 0);

    // ���ò˵���ť
    auto selectionItem = MenuItemImage::create(
        "menu/Selection_Button_Normal.png",
        "menu/Selection_Button_Selected.png",
        CC_CALLBACK_1(Settingscene1::onSelection1, this));

    auto quitItem = MenuItemImage::create(
        "menu/Quite_Button_normal.png",
        "menu/Quite_Button_Selected.png",
        CC_CALLBACK_1(Settingscene1::onQuit1, this));

    //��Ϊ���ð�ť��Ӧ��Ϊ���ð�ť�����ݲ��ģ�ͼƬ������λ�ø�����
    auto settingsItem = MenuItemImage::create(
        "menu/Setting_Button_Normal.png",
        "menu/Setting_Button_Selected.png",
        CC_CALLBACK_1(Settingscene1::onSettings1, this));

    selectionItem->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + upOffset + 58));
    quitItem->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + upOffset - 84));
    settingsItem->setPosition(Vec2(visibleSize.width - settingsItem->getContentSize().width / 2, settingsItem->getContentSize().height / 2));
    auto menu2 = Menu::create(selectionItem, quitItem, settingsItem, nullptr);
    menu2->setPosition(Vec2(0, 0));

    this->addChild(menu2,1);
    
    
    // ���Ų˵�����
    //playMenuAnimation();
}

void Settingscene1::initUI1() {
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
        versionLabel->setPosition(Vec2(versionLabel->getContentSize().width / 2,
            visibleSize.height - versionLabel->getContentSize().height / 2));
        this->addChild(versionLabel, 1);
    }

    
    // �˵���ص�����
}

void Settingscene1::onSelection1(Ref* sender)
{
    //�˹��ܿ��ǵ�ʵ�����ò�����ʱ��������
}

void Settingscene1::onQuit1(Ref* sender) {
    AudioManager::getInstance()->playEffect(GameConstants::Sound::MAIN_MENU_THREEBUTTON_CLICK);
    Director::getInstance()->end();    // ���Ű�ť�����Ч
    // �˳���Ϸ
}

void Settingscene1::onSettings1(Ref* sender)
{
    AudioManager::getInstance()->playEffect(GameConstants::Sound::CHANGE_HELP_SCENE);

    // �л�������1����
    auto scene = MenuScene::createScene();
    Director::getInstance()->replaceScene(scene);
}