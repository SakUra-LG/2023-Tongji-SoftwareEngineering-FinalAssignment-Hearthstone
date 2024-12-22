#pragma execution_character_set("utf-8")

#include "DeckBuilderScene.h"
#include "MenuScene.h"
#include "Card/CardFactory.h"

USING_NS_CC;

Scene* DeckBuilderScene::createScene() {
    return DeckBuilderScene::create();
}

bool DeckBuilderScene::init() {
    if (!Scene::init()) {
        return false;
    }

    // 初始化UI组件
    initBackground();               //设置背景
    initMenu();                     //设置按钮交互
    initUI();                       //设置标题等ui组件        

    // 创建卡牌容器
    _cardContainer = Node::create();
    if (_cardContainer) {
        this->addChild(_cardContainer);
        
        // 改用runAction来延迟加载卡牌，这样可以确保场景完全初始化
        auto delay = DelayTime::create(0.1f);
        auto loadCardsFunc = CallFunc::create([this]() {
            loadCards();
        });
        this->runAction(Sequence::create(delay, loadCardsFunc, nullptr));
    }

    return true;
}

void DeckBuilderScene::loadCards() {
    if (!_cardContainer) {
        return;
    }

    // 清理旧卡牌
    for (auto card : _allCards) {
        if (card) {
            card->release();  // 释放之前的卡牌
        }
    }
    _allCards.clear();
    _displayedCards.clear();
    _cardContainer->removeAllChildren();

    // 直接创建卡牌
    auto createCard = [](int id, const std::string& name, const std::string& description, 
                        const std::string& framePath, const std::string& portraitPath,
                        int cost, int attack = 0, int health = 0) -> Card* {
        auto card = Card::create(id, name);
        if (card) {
            // 创建框架精灵
            auto frame = Sprite::create(framePath);
            if (frame) {
                frame->setPosition(card->getContentSize() / 2);
                card->addChild(frame, 1);
            }

            // 创建立绘精灵
            auto portrait = Sprite::create(portraitPath);
            if (portrait) {
                portrait->setPosition(card->getContentSize().width / 2, card->getContentSize().height / 2+120);
                portrait->setScale(1.2f);  // 可以调整立绘大小
                card->addChild(portrait, 0);
            }

            // 法力值消耗标签
            auto costLabel = Label::createWithTTF(std::to_string(cost), "fonts/arial.ttf", 80);
            if (costLabel) {
                costLabel->setPosition(Vec2(-100, card->getContentSize().height+180));
                costLabel->setTextColor(Color4B::WHITE);  // 改为白色
                costLabel->enableOutline(Color4B::BLACK, 2);  // 保持黑色描边
                costLabel->enableShadow(Color4B::BLACK);  // 保持阴影
                card->addChild(costLabel, 2);
            }

            // 名称标签
            auto nameLabel = Label::createWithTTF(name, "fonts/STKAITI.TTF", 55);
            if (nameLabel) {
                nameLabel->setPosition(Vec2(card->getContentSize().width / 2,
                                         card->getContentSize().height - 60));
                nameLabel->setTextColor(Color4B::WHITE);
                nameLabel->enableOutline(Color4B::BLACK, 1);
                //nameLabel->setOverflow(Label::Overflow::SHRINK);
               // nameLabel->setWidth(card->getContentSize().width - 20);
                card->addChild(nameLabel, 2);
            }

            // 如果是随从卡，添加攻击力和生命值
            if (id == 1003 || id == 1006 || id == 1008 || id == 1011 || id == 1016 ||
                id== 2004 ||id== 1013||id== 1015||id== 2001||id==2008||(id>=2009&&id<=2011)||id==2013||id==2014) {
                //// 攻击力标签
                auto attackLabel = Label::createWithTTF(std::to_string(attack), "fonts/arial.ttf", 70);
                attackLabel->setPosition(Vec2(-100, -200));
                attackLabel->setTextColor(Color4B::WHITE);  // 改为白色
                attackLabel->enableOutline(Color4B::BLACK, 2);
                attackLabel->enableShadow(Color4B::BLACK);
                card->addChild(attackLabel, 2);

                //// 生命值标签
                auto healthLabel = Label::createWithTTF(std::to_string(health), "fonts/arial.ttf", 72);
                healthLabel->setPosition(Vec2(card->getContentSize().width +150, -200));
                healthLabel->setTextColor(Color4B::WHITE);  // 改为白色
                healthLabel->enableOutline(Color4B::BLACK, 2);
                healthLabel->enableShadow(Color4B::BLACK);
                card->addChild(healthLabel, 2);
            }

            // 描述文本标签
            auto descLabel = Label::createWithTTF(description, "fonts/STKAITI.TTF", 30);
            if (descLabel) {
                descLabel->setPosition(Vec2(card->getContentSize().width / 2, -100));
                descLabel->setDimensions(card->getContentSize().width +120, 120);//文本框大小
                descLabel->setTextColor(Color4B::BLACK);  // 改为黑色
                descLabel->enableOutline(Color4B::WHITE, 1);  // 改为白色描边
                descLabel->setAlignment(TextHAlignment::CENTER);
                //descLabel->setOverflow(Label::Overflow::SHRINK);
                card->addChild(descLabel, 2);
            }
        }
        return card;
    };

    // 创建一些测试卡牌
    std::vector<std::tuple<int, std::string, std::string, std::string, std::string, int, int, int>> cardData = {
        // 任务瞎卡组
        {1001, "二段跳", "从你的牌库中抽一张流放牌", 
         "cards/frame_normal_fashu.png", "cards/portraits_erduantiao.png", 
         1, 0, 0},  // 1费法术
        {1002, "伊利达雷研习", "发现一张流放牌，你的下一张流放牌法力值减小（1）点", 
         "cards/frame_normal_fashu.png", "cards/portraits_YiLiDaLeiYanXi.png", 
         1, 0, 0},  // 1费法术
        {1003, "凶猛的外来者", "突袭。流放：你的下一张流放牌法力消耗减小（1）点", 
         "cards/frame_normal_suicong.png", "cards/portraits_XiongMengDeWaiLaiZhe.png", 
         1, 2, 1},  // 1费随从，2/1
        {1004, "敏捷咒符", "在下个回合开始时，抽一张牌，并使其法力消耗减小（1）点", 
         "cards/frame_normal_fashu.png", "cards/portraits_MinJieZhouFu.png", 
         1, 0, 0},  // 1费法术
        {1005, "法力燃烧", "下个回合，你的对手减少两个法力值", 
         "cards/frame_normal_fashu.png", "cards/portraits_FaLiRanShao.png", 
         1, 0, 0},  // 1费法术
        {1006, "火色魔印奔行者", "流放：抽一张牌", 
         "cards/frame_normal_suicong.png", "cards/portraits_HuoSeMoYinBenXingZhe.png", 
         1, 1, 1},  // 1费随从，1/1
        {1007, "邪能学说", "复制你手牌中法力值消耗最低的恶魔牌。流放：使两张恶魔牌获得+1/+1", 
         "cards/frame_normal_fashu.png", "cards/portraits_XieNengXueShuo.png", 
         1, 0, 0},  // 1费法术
        {1008, "飞行员帕奇斯", "战吼：将六张降落伞洗入你的牌库。降落伞的法力消耗为（0）点，可召唤一个1/1有冲锋的海盗", 
         "cards/frame_normal_suicong.png", "cards/portraits_FeiXingYuanPaQiSi.png", 
         1, 1, 1},  // 1费随从，1/1
        {1009, "幽灵视觉", "抽一张牌。流放：再抽一张", 
         "cards/frame_normal_fashu.png", "cards/portraits_YouLingShiJue.png", 
         2, 0, 0},  // 2费法术
        {1010, "时间咒符", "在下个回合开始时，额外抽三张牌", 
         "cards/frame_normal_fashu.png", "cards/portraits_ShiJianZhouFu.png", 
         3, 0, 0},  // 3费法术
        {1011, "灰烬元素", "在下回合，对手抽牌后，对手会受到2点伤害", 
         "cards/frame_normal_suicong.png", "cards/portraits_HuiJinYuanSu.png", 
         3, 2, 4},  // 3费随从，2/4
        {1012, "飞翼滑翔", "双方玩家抽三张牌。流放：只有你抽牌", 
         "cards/frame_normal_fashu.png", "cards/portraits_FeiYiHuaXiang.png", 
         3, 0, 0},  // 3费法术
        {1013, "战刃吉他", "亡语：抽一张牌（在你装备期间，每用过一张流放牌，提升此效果）", 
         "cards/frame_RARE_wuqi.png", "cards/portraits_RenWuXiaDao.png", 
         4, 4, 2},  // 4费武器，4/2
        {1014, "滑翔", "将你的手牌洗入你的牌库，抽四张牌。流放：对手做出同样行为", 
         "cards/frame_normal_fashu.png", "cards/portraits_HuaXiang.png", 
         4, 0, 0},  // 4费法术

        // 继续添加剩余的卡牌...
        {1015, "极限追逐者阿兰娜", "战吼：本局对战剩余时间内你的英雄在你的回合中受到的伤害会转移给敌方英雄", 
         "cards/frame_normal_suicong.png", "cards/portraits_JiXiangZhuiZhuZheALanNa.png", 
         5, 5, 5},  // 5费随从，5/5

        {1016, "复仇重击", "战吼：如果你的英雄在本回合受到过伤害，获得+3/+3和突袭", 
         "cards/frame_normal_suicong.png", "cards/portraits_FuChouZhongJiZhe.png", 
         5, 3, 3},  // 5费随从，3/3

        // 彩虹DK卡组
        {2001, "奇迹推销员", "奇迹推销员", 
         "cards/frame_normal_suicong.png", "cards/portraits_QiJiTuiXiaoYuan.png", 
         1, 2, 2},  // 1费随从，2/2

        {2002, "黑暗符文", "发现一张武器牌", 
         "cards/frame_normal_fashu.png", "cards/portraits_HeiAnFuWen.png", 
         1, 0, 0},  // 1费法术

        {2003, "冰霜打击", "对一个随从造成3点伤害，如果其死亡，发现一张武器牌", 
         "cards/frame_normal_fashu.png", "cards/portraits_BingShuangDaJi.png", 
         2, 0, 0},  // 2费法术

        {2004, "恐惧猎犬训练师", "突袭。亡语：召唤一只1/1的恐惧猎犬", 
         "cards/frame_normal_suicong.png", "cards/portraits_KongJuLieQuanXunLianShi.png", 
         2, 2, 2},  // 2费随从，2/2

        {2005, "甜筒冰淇淋", "造成3点伤害，消耗3份残骸，在回合结束移回你的手牌", 
         "cards/frame_normal_fashu.png", "cards/portraits_TianTongBingQiLin.png", 
         2, 0, 0},  // 2费法术

        {2006, "采矿事故", "召唤两个2/2的白银之手新兵", 
         "cards/frame_normal_fashu.png", "cards/portraits_CaiKuangShiGu.png", 
         2, 0, 0},  // 2费法术

        {2007, "麦芽糖浆", "对所有敌方随从造成3点伤害", 
         "cards/frame_normal_fashu.png", "cards/portraits_MaiYaYanJiang.png", 
         2, 0, 0},  // 2费法术

        {2008, "彩虹裁缝", "战吼：获得突袭、圣盾", 
         "cards/frame_normal_suicong.png", "cards/portraits_CaiHongCaiFeng.png", 
         3, 3, 3},  // 3费随从，3/3

        {2009, "戈贡佐姆", "战吼：随机召你牌库里的3个随从", 
         "cards/frame_normal_suicong.png", "cards/portraits_GeGongZuoMu.png", 
         7, 4, 4},  // 7费随从，4/4

        {2010, "伊丽扎-刺刃", "亡语：在本局剩余时间内，你召唤的随从获得+1攻击力", 
         "cards/frame_normal_suicong.png", "cards/portraits_YiLiZhaCiRen.png", 
         4, 4, 3},  // 4费随从，4/3

        {2011, "大地之末号", "战吼，亡语：造成5点伤害，随机分配到敌人身上", 
         "cards/frame_normal_suicong.png", "cards/portraits_DaDiZhiMoHao.png", 
         4, 0, 5},  // 4费随从，0/5

        {2012, "食尸鬼之夜", "召唤5个1/1的食尸鬼并随即攻击敌人", 
         "cards/frame_normal_fashu.png", "cards/portraits_ShiShiGuiZhiYe.png", 
         5, 0, 0},  // 5费法术

        {2013, "奇利亚斯豪华版3000型", "圣盾，嘲讽，剧毒，突袭。战吼：召唤一个本随从的复制", 
         "cards/frame_normal_suicong.png", "cards/portraits_QiLiYaSiHaoHuaBan3000Xing.png", 
         9, 6, 5},  // 9费随从，6/5

        {2014, "矿坑老板雷斯卡", "突袭。在本局中每有一个随从死亡本牌法力消耗减少（1）点。亡语：随机夺取一个敌人的控制权", 
         "cards/frame_normal_suicong.png", "cards/portraits_KuangKengLaoBanLeiSiKa.png", 
         25, 6, 3}  // 25费随从，6/3
    };

    // 创建新卡牌时保持引用
    for (const auto& data : cardData) {
        auto card = createCard(
            std::get<0>(data),
            std::get<1>(data),
            std::get<2>(data),
            std::get<3>(data),
            std::get<4>(data),
            std::get<5>(data),
            std::get<6>(data),
            std::get<7>(data)
        );
        if (card) {
            card->retain();  // 增加引用计数
            card->setScale(CARD_SCALE);
            _allCards.push_back(card);
        }
    }

    // 显示第一页卡牌
    if (!_allCards.empty()) {
        _currentPage = 0;
        displayCardsForCurrentPage();
    }
}

void DeckBuilderScene::initBackground() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto background = Sprite::create("backgrounds/Background_For_DeckBuilder.png");
    if (background) {
        background->setPosition(Vec2(visibleSize.width/2 + origin.x, 
                                   visibleSize.height/2 + origin.y));
        this->addChild(background);
        CCLOG("Background created successfully");
    } else {
        CCLOG("Failed to create background");
    }
}

void DeckBuilderScene::initMenu() {
    CCLOG("Starting initMenu");
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    // 创建菜单项，但先不设置位置
    auto pageRight = MenuItemImage::create(
        "buttons/Button_For_DeckBuidler_Right.png",
        "buttons/Button_For_DeckBuidler_Right.png",
        [this](Ref* sender) {
            CCLOG("Right button clicked");
            this->onPageRight(sender);
        });
    
    auto pageLeft = MenuItemImage::create(
        "buttons/Button_For_DeckBuilder_Left.png",
        "buttons/Button_For_DeckBuilder_Left.png",
        [this](Ref* sender) {
            CCLOG("Left button clicked");
            this->onPageLeft(sender);
        });
    
    auto returnMenu = MenuItemImage::create(
        "buttons/Button_Back_Normal.png",
        "buttons/Button_Back_Selected.png",
        [this](Ref* sender) {
            CCLOG("Return button clicked");
            this->onReturnMenu(sender);
        });
    
    // 检查是否所有按钮都创建成功
    if (!pageRight || !pageLeft || !returnMenu) {
        CCLOG("Failed to create one or more menu items");
        return;
    }
    
    // 设置按钮位置
    pageRight->setPosition(Vec2(1850 - visibleSize.width/2, 0));
    pageLeft->setPosition(Vec2(230 - visibleSize.width/2, 0));
    returnMenu->setPosition(Vec2(visibleSize.width/2 - returnMenu->getContentSize().width/2, 
                                -visibleSize.height/2 + returnMenu->getContentSize().height/2));
    
    // 创建菜单并添加到场景
    auto menu = Menu::create();
    if (!menu) {
        CCLOG("Failed to create menu");
        return;
    }
    
    menu->addChild(pageRight);
    menu->addChild(pageLeft);
    menu->addChild(returnMenu);
    
    menu->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(menu);
    
    CCLOG("Menu initialization complete");
}

void DeckBuilderScene::initUI() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    // 添加标题
    auto titleLabel = Label::createWithTTF("My Collection", "fonts/arial.ttf", 24);
    if (!titleLabel) {
        CCLOG("Failed to create title label!");
        return;
    }
    
    titleLabel->setPosition(Vec2(visibleSize.width/2, visibleSize.height - 50));
    this->addChild(titleLabel);

    // 初始化页码标签
    _pageLabel = Label::createWithTTF("Page 1/1", "fonts/arial.ttf", 24);
    if (_pageLabel) {
        _pageLabel->setPosition(Vec2(visibleSize.width/2, 50));
        this->addChild(_pageLabel);
    }
}

void DeckBuilderScene::displayCardsForCurrentPage() {
    if (!_cardContainer) {
        return;
    }

    _cardContainer->removeAllChildren();
    _displayedCards.clear();

    Size visibleSize = Director::getInstance()->getVisibleSize();
    const int CARDS_PER_ROW = 4;
    const float CARD_PADDING_X = 300;
    const float CARD_PADDING_Y = 400;
    const float START_X = visibleSize.width / 2 - ((CARDS_PER_ROW - 1) * CARD_PADDING_X / 2);
    const float START_Y = visibleSize.height - 250;

    int startIndex = _currentPage * CARDS_PER_PAGE;
    int endIndex = std::min(startIndex + CARDS_PER_PAGE, (int)_allCards.size());

    for (int i = startIndex; i < endIndex; ++i) {
        Card* card = _allCards[i];
        if (card) {
            int row = (i - startIndex) / CARDS_PER_ROW;
            int col = (i - startIndex) % CARDS_PER_ROW;
            float x = START_X + col * CARD_PADDING_X;
            float y = START_Y - row * CARD_PADDING_Y;

            card->setPosition(Vec2(x, y));
            card->setScale(0.5f);
            _cardContainer->addChild(card);  // 添加到容器时会自动增加引用计数
            _displayedCards.push_back(card);
        }
    }

    updatePageDisplay();
}

void DeckBuilderScene::onReturnMenu(Ref* sender) {
    auto scene = MenuScene::createScene();
    if (scene) {
        // 使用 TransitionFade 来平滑切换场景
        auto transition = TransitionFade::create(0.5f, scene);
        Director::getInstance()->replaceScene(transition);
    }
}

void DeckBuilderScene::onPageRight(Ref* sender) {
    CCLOG("Page right callback triggered");
    if (_allCards.empty()) {
        CCLOG("No cards available");
        return;
    }
    
    int totalPages = (_allCards.size() + CARDS_PER_PAGE - 1) / CARDS_PER_PAGE;
    if (_currentPage < totalPages - 1) {
        _currentPage++;
        displayCardsForCurrentPage();
    }
}

void DeckBuilderScene::onPageLeft(Ref* sender) {
    CCLOG("Page left callback triggered");
    if (_currentPage > 0) {
        _currentPage--;
        displayCardsForCurrentPage();
    }
}

void DeckBuilderScene::updatePageDisplay() {
    if (!_pageLabel) {
        // 如果页码标签不存在，创建它
        _pageLabel = Label::createWithTTF("", "fonts/arial.ttf", 24);
        if (_pageLabel) {
            _pageLabel->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, 50));
            this->addChild(_pageLabel);
        }
    }

    if (_pageLabel) {
        // 计算总页数
        int totalPages = (_allCards.empty()) ? 1 : ((_allCards.size() + CARDS_PER_PAGE - 1) / CARDS_PER_PAGE);
        // 更新页码显示
        std::string pageText = StringUtils::format("Page %d/%d", _currentPage + 1, totalPages);
        _pageLabel->setString(pageText);
    }
}