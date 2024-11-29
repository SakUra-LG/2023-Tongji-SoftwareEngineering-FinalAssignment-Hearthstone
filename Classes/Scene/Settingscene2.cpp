#include "Settingscene1.h"
#include "Settingscene2.h"
#include "GameScene.h"
#include "Utils/Constants.h"
#include "Audio/AudioManager.h"
#include"MenuScene.h"

USING_NS_CC;
// ���������ľ�̬����
Scene* Settingscene2::createScene() {
    return Settingscene2::create();
}
// ������ʼ������
bool Settingscene2::init() {
    if (!Scene::init()) {
        return false;
    }
    // ��ʼ��������������Ҫ���
    Settingscene2::initBackground2();// ��ʼ������
    Settingscene2::initMenu2(); // ��ʼ���˵���ť
    Settingscene2::initUI2();// ��ʼ��UIԪ��
    return true;
}

void Settingscene2::initBackground2() { // ��ȡ��Ļ�ߴ��ԭ��λ��
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // ���������ñ�������
    auto background = Sprite::create(GameConstants::Resources::BACKGROUND_SETTING);
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x,
        visibleSize.height / 2 + origin.y));
    this->addChild(background);
}

void Settingscene2::initMenu2()
{

}

void Settingscene2::initUI2()
{

}
