#include "SelectCardsScene.h"
#include "GameScene.h"
#include "Utils/Constants.h"
#include "Audio/AudioManager.h"
#include "MenuScene.h"
#include "Card/CardFactory.h"
#include "Utils/GameLogger.h"
#include"Card/DeckManager.h"
#pragma execution_character_set("utf-8")
USING_NS_CC;
// ľ̬
Scene* SelectCardsScene::createScene() {
    return SelectCardsScene::create();
}
// ʼ
bool SelectCardsScene::init() {
    if (!Scene::init()) {
        return false;
    }
    // ʼҪ
    SelectCardsScene::initBackground3();// ʼ
    SelectCardsScene::initMenu3(); // ʼ˵ť
    SelectCardsScene::initUI3();// ʼUIԪ
    return true;
}

void SelectCardsScene::initBackground3() { // ȡĻߴԭλ
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // ñ
    auto background = Sprite::create(GameConstants::Resources::BACKGROUND_SELECTCARDS);
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x,
        visibleSize.height / 2 + origin.y));
    this->addChild(background);
}

void SelectCardsScene::initMenu3() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // ĸҪ˵ʼϷ鹹ú˳
       // Ĵֱλ
       // ӵص
       // 

    float upOffset = 70.0f;  // ƫ
    float spacing = 80.0f;


    // ò˵ť
    auto backItem = MenuItemImage::create(
        "buttons/Button_Back_Normal.png",
        "buttons/Button_Back_Selected.png",
        CC_CALLBACK_1(SelectCardsScene::onBack, this));

    //TODO-Ϲ
    auto cards1Item= MenuItemImage::create(
        "buttons/Button_SelectCards_RENWUXIA.png",
        "buttons/Button_SelectCards_RENWUXIA.png",
        CC_CALLBACK_1(SelectCardsScene::onCards1, this));
    //TODO-DK
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


    // ˵
    //playMenuAnimation();
}

void SelectCardsScene::initUI3() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ӱ
    auto titleLabel = Label::createWithTTF("SelectYourCards", "fonts/arial.ttf", 28);
    if (titleLabel) {
        titleLabel->setPosition(Vec2(origin.x + visibleSize.width / 2-350,
            origin.y + visibleSize.height - titleLabel->getContentSize().height-15));
        this->addChild(titleLabel, 1);
    }

    // Ӱ汾
    auto versionLabel = Label::createWithTTF("Version 1.0", "fonts/arial.ttf", 24);
    if (versionLabel) {
        versionLabel->setPosition(Vec2(versionLabel->getContentSize().width / 2,
            visibleSize.height - versionLabel->getContentSize().height / 2));
        this->addChild(versionLabel, 1);
    }


    // ˵ص
}


void SelectCardsScene::onCards1(Ref* sender) {
    auto logger = GameLogger::getInstance();
    logger->log(LogLevel::INFO, "Starting onCards1...");
    
    auto factory = CardFactory::getInstance();
    if (!factory) {
        logger->log(LogLevel::ERR, "Failed to get CardFactory instance");
        return;
    }

    const auto& deck1 = factory->getDeck1();
    logger->log(LogLevel::INFO, "Deck1 size: " + std::to_string(deck1.size()));
    
    auto deckManager = DeckManager::getInstance();
    if (!deckManager) {
        CCLOG("Failed to get DeckManager instance");
        return;
    }

    auto deck = deckManager->createDeckFromTemplate(deck1);
    if (!deck) {
        CCLOG("Failed to create deck from template");
        return;
    }
    CCLOG("Successfully created deck");

    //添加对手卡组
    
        //添加对手卡牌
        auto& deckEnemy = factory->getDeckEnemy();
        logger->log(LogLevel::INFO, "DeckEnemy size: " + std::to_string(deckEnemy.size()));
        auto deckenemy2 = Deck::create("EnemyDeck");              //创建一个名字是EnemyDeck的对手卡组(和deck用法一致
        if (deckenemy2) {
            auto factory = CardFactory::getInstance();
            auto logger = GameLogger::getInstance();

            // 遍历所有卡牌模板
            for (const auto& templateCard : deckEnemy) {
                if (!templateCard) continue;

                int cardId = templateCard->getId();
                // 确保卡牌ID与卡组类型匹配
                Card* newCard = factory->createCardById(cardId);
                if (newCard) {
                    // 设置卡牌数量（从模板中获取）
                    newCard->setCount(templateCard->getCount());
                    // 添加到卡组
                    deckenemy2->addCardInstance(newCard);

                    logger->log(LogLevel::DEBUG,
                        "Added card to deck: " + newCard->getName() +
                        " (ID: " + std::to_string(cardId) +
                        ", Count: " + std::to_string(templateCard->getCount()) + ")");
                }

            }

            logger->log(LogLevel::INFO,
                "Created deck: " + deckenemy2->getName() +
                " with " + std::to_string(deckenemy2->getTotalCards()) + " cards");
        }
    

    auto scene = GameScene::createWithDeck(deck, deckenemy2);
    if (!scene) {
        CCLOG("Failed to create game scene");
        return;
    }
    CCLOG("Successfully created game scene");

    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5f, scene));
}

void SelectCardsScene::onCards2(Ref* sender) {
    auto logger = GameLogger::getInstance();
    logger->log(LogLevel::INFO, "Starting onCards1...");

    auto factory = CardFactory::getInstance();
    if (!factory) {
        logger->log(LogLevel::ERR, "Failed to get CardFactory instance");
        return;
    }

    const auto& deck2 = factory->getDeck2();
    logger->log(LogLevel::INFO, "Deck2 size: " + std::to_string(deck2.size()));

    auto deckManager = DeckManager::getInstance();
    if (!deckManager) {
        CCLOG("Failed to get DeckManager instance");
        return;
    }

    auto deck = deckManager->createDeckFromTemplate(deck2);
    if (!deck) {
        CCLOG("Failed to create deck from template");
        return;
    }
    CCLOG("Successfully created deck");

    //添加对手卡组

        //添加对手卡牌
    auto& deckEnemy = factory->getDeckEnemy();
    logger->log(LogLevel::INFO, "DeckEnemy size: " + std::to_string(deckEnemy.size()));
    auto deckenemy2 = Deck::create("EnemyDeck");              //创建一个名字是EnemyDeck的对手卡组(和deck用法一致
    if (deckenemy2) {
        auto factory = CardFactory::getInstance();
        auto logger = GameLogger::getInstance();

        // 遍历所有卡牌模板
        for (const auto& templateCard : deckEnemy) {
            if (!templateCard) continue;

            int cardId = templateCard->getId();
            // 确保卡牌ID与卡组类型匹配
            Card* newCard = factory->createCardById(cardId);
            if (newCard) {
                // 设置卡牌数量（从模板中获取）
                newCard->setCount(templateCard->getCount());
                // 添加到卡组
                deckenemy2->addCardInstance(newCard);

                logger->log(LogLevel::DEBUG,
                    "Added card to deck: " + newCard->getName() +
                    " (ID: " + std::to_string(cardId) +
                    ", Count: " + std::to_string(templateCard->getCount()) + ")");
            }

        }

        logger->log(LogLevel::INFO,
            "Created deck: " + deckenemy2->getName() +
            " with " + std::to_string(deckenemy2->getTotalCards()) + " cards");
    }

    auto scene = GameScene::createWithDeck(deck, deckenemy2);
    if (!scene) {
        CCLOG("Failed to create game scene");
        return;
    }
    CCLOG("Successfully created game scene");

    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5f, scene));
}

void SelectCardsScene::onBack(Ref* sender)
{
    //л˵
    auto scene = MenuScene::createScene();
    Director::getInstance()->replaceScene(scene);
}