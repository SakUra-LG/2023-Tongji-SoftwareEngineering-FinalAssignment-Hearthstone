#include "MenuScene.h"
#include "GameScene.h"
#include "Utils/Constants.h"
#include "Audio/AudioManager.h"
#include "Settingscene1.h"

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
    
    //����ƫ����
    float upOffset = 70.0f;                                 //��������ƫ����

    //���濪ʼ����������
    auto spriteTable = Sprite::create("menu/Table.png");
    // position the sprite on the center of the screen    
    spriteTable->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + upOffset));
    // add the sprite as a child to this layer
    this->addChild(spriteTable, 0);

    //���濪ʼ��ѡ�����
    auto spriteCircle = Sprite::create("menu/Button_Circle.png");
    // position the sprite on the center of the screen
    spriteCircle->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + upOffset));
    // add the sprite as a child to this layer
    this->addChild(spriteCircle, 1);

     //���濪ʼ�������ղش���ѡ�
    auto bottomSelectionBackground = Sprite::create("menu/BottomSelection_Background.png");
    // position the sprite on the center of the screen
    bottomSelectionBackground->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - spriteTable->getContentSize().height / 2 + upOffset / 2));
    // add the sprite as a child to this layer
    this->addChild(bottomSelectionBackground, 1);

    //��ս��ť
    auto playItem = MenuItemImage::create(
        "menu/Button_First_Normal.png",
        "menu/Button_First_Selected.png",
        CC_CALLBACK_1(MenuScene::onPlayGame, this));

    //�ƹ�ս�찴ť
    auto battleItem = MenuItemImage::create(
        "menu/Button_Second_Normal.png",
        "menu/Button_Second_Selected.png",
        CC_CALLBACK_1(MenuScene::onBattleGronds, this));

    //ð��ģʽ��ť
    auto adventureItem = MenuItemImage::create(
        "menu/Button_Third_Normal.png",
        "menu/Button_Third_Selected.png",
        CC_CALLBACK_1(MenuScene::onAdventureMode, this));

    //��Ϊ���ð�ť��Ӧ��Ϊ���ð�ť�����ݲ��ģ�ͼƬ������λ�ø�����
    auto settingsItem = MenuItemImage::create(
        "menu/Setting_Button_Normal.png",
        "menu/Setting_Button_Selected.png",
        CC_CALLBACK_1(MenuScene::onSettings, this));

    //�����ղذ�ť
    auto deckItem = MenuItemImage::create(
        "menu/MyCollaction_Normal.png",
        "menu/MyCollaction_Selected.png",
        CC_CALLBACK_1(MenuScene::onDeckBuilder, this));

    // ����λ��
    float spacing = 80.0f;
    playItem->setPosition(Vec2(0, spacing * 1.2f + upOffset));
    battleItem->setPosition(Vec2(0, spacing * 0.45f + upOffset));
    adventureItem->setPosition(Vec2(0, -spacing * 0.35f + upOffset));
    settingsItem->setPosition(Vec2(visibleSize.width/2- settingsItem->getContentSize().width/2, -visibleSize.height/2+ settingsItem->getContentSize().height / 2 ));
    Rect boundingBox = bottomSelectionBackground->getBoundingBox();              // ��ȡ bottomSelectionBackground �ı߽����
    Vec2 deckItemPos = Vec2(boundingBox.getMaxX()-50-visibleSize.width/2-deckItem->getContentSize().width / 2, boundingBox.getMinY()+30-visibleSize.height/2+ deckItem->getContentSize().height/2);   // ���� bottomSelectionBackground ���½ǵ�λ��,�����������λ��
    deckItem->setPosition(deckItemPos);

    // �����˵�
    auto menu = Menu::create(playItem, battleItem, adventureItem, settingsItem, deckItem, nullptr);
    menu->setPosition(Vec2(visibleSize.width / 2 + origin.x,
        visibleSize.height / 2 + origin.y ));

    this->addChild(menu,2);

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
        versionLabel->setPosition(Vec2(versionLabel->getContentSize().width/2,
           visibleSize.height- versionLabel->getContentSize().height / 2));
        this->addChild(versionLabel, 1);
    }

    //����λ�õĲ���
    float upOffset = 70.0f;                     //��������ƫ����
    float spacing = 80.0f;
    //��Ӳ˵���ť������ʾ
    auto playStartLable = Label::createWithTTF("Hearthstone", "fonts/Marker Felt.ttf", 24);
    if (playStartLable) {
        playStartLable->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + upOffset + spacing * 1.2f));
        playStartLable->setTextColor(Color4B(0, 0, 0, 255));
        this->addChild(playStartLable, 3);
    }

    auto battleGroundsLable = Label::createWithTTF("Battlegrounds", "fonts/Marker Felt.ttf", 24);
    if (battleGroundsLable) {
        battleGroundsLable->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 +spacing * 0.45f + upOffset));
        battleGroundsLable->setTextColor(Color4B(0, 0, 0, 255));
        this->addChild(battleGroundsLable, 3);
    }

    auto adventureModeLable = Label::createWithTTF("Adventuremode", "fonts/Marker Felt.ttf", 24);
    if (adventureModeLable) {
        adventureModeLable->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - spacing * 0.35f + upOffset));
        adventureModeLable->setTextColor(Color4B(0, 0, 0, 255));
        this->addChild(adventureModeLable, 3);
    }

    // �˵���ص�����
}

void MenuScene::onDeckBuilder(Ref* sender) {
    AudioManager::getInstance()->playEffect(GameConstants::Sound::MAIN_MENU_THREEBUTTON_CLICK);
    AudioManager::getInstance()->playEffect(GameConstants::Sound::ENTER_MYCOLLECTION);
    // TODO: ʵ�ֿ��鹹�������л�


}

void MenuScene::onSettings(Ref* sender) {
    AudioManager::getInstance()->playEffect(GameConstants::Sound::CHANGE_HELP_SCENE);

    // �л�������1����
    auto scene = Settingscene1::createScene();
    Director::getInstance()->replaceScene(scene);

}

void MenuScene::onQuit(Ref* sender) {
    AudioManager::getInstance()->playEffect(GameConstants::Sound::MAIN_MENU_THREEBUTTON_CLICK);
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
    AudioManager::getInstance()->playEffect(GameConstants::Sound::MAIN_MENU_THREEBUTTON_CLICK);
    AudioManager::getInstance()->playEffect(GameConstants::Sound::CHANGE_SCENE_FROM_MAIN);
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
    AudioManager::getInstance()->playEffect(GameConstants::Sound::MAIN_MENU_THREEBUTTON_CLICK);
    // TODO: ��ӽ����غ��߼�
}

void GameScene::onSettingsClicked(Ref* sender) {
    // �������ð�ť���
    AudioManager::getInstance()->playEffect(GameConstants::Sound::MAIN_MENU_THREEBUTTON_CLICK);
    // TODO: ��ʾ���ò˵�
}


//����Ϊ��չ���ݣ���ʱ������ѡ��
//ð��ģʽ
void MenuScene::onAdventureMode(Ref* sender)
{
    // ��������غϰ�ť���
    AudioManager::getInstance()->playEffect(GameConstants::Sound::MAIN_MENU_THREEBUTTON_CLICK);
    // TODO: ���ð��ģʽ�߼�
}

//�ƹ�ս��
void MenuScene::onBattleGronds(Ref* sender)
{
    // ��������غϰ�ť���
    AudioManager::getInstance()->playEffect(GameConstants::Sound::MAIN_MENU_THREEBUTTON_CLICK);
    // TODO: ���ð��ģʽ�߼�
}