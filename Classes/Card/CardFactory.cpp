#pragma execution_character_set("utf-8")

#include "CardFactory.h"
#include "Manager/GameManager.h"
#include "Effect/CardEffects.h"
#include "Effect/SpellEffect.h"
#include "Utils/GameLogger.h"

CardFactory* CardFactory::_instance = nullptr;

CardFactory::CardFactory() {
    auto logger = GameLogger::getInstance();
    logger->log(LogLevel::DEBUG, "Creating CardFactory instance");
    
    // 初始化卡牌模板
    initCardTemplates();  // 使用新的方法名
}

CardFactory* CardFactory::getInstance() {
    if (!_instance) {
        _instance = new CardFactory();  // 构造函数中已经调用了 initCardTemplates
        auto logger = GameLogger::getInstance();
        logger->log(LogLevel::INFO, "CardFactory instance created and initialized");
        
        // 只初始化卡组
        _instance->initializeDecks();
        logger->log(LogLevel::INFO, "Decks initialized");
    }
    return _instance;
}

//1开头的卡牌属于任务瞎卡组（时间原因任务没有做）,2开头的是彩虹dk
void CardFactory::initCardTemplates() {
    auto logger = GameLogger::getInstance();
    logger->log(LogLevel::DEBUG, "Starting to initialize card templates");
    
    try {
        CardData cardData;
        int cardCount = 0;
        
        //// 二段跳
        //cardData.id = 1001;
        //cardData.name = "二段跳";
        //cardData.type = CardType::SPELL;
        //cardData.rarity = CardRarity::NORMAL;
        //cardData.cost = 1;
        //cardData.description = "使一个随从获得冲锋";
        //cardData.portraitPath = "cards/portraits/二段跳.png";
        //_cardTemplates[cardData.id] = cardData;
        //cardCount++;
        //
        //// 添加更多卡牌...
        //cardData = CardData();  // 重置数据
        //cardData.id = 1002;
        //// ... 设置其他卡牌数据



        {
            CardData card;
            card.id = 1001;
            card.name = "二段跳";
            card.type = CardType::SPELL;
            card.rarity = CardRarity::NORMAL;
            card.cost = 1;
            card.description = "从你的牌库中抽一张流放牌";
            card.hasBattlecry = true;
            card.framePath = "cards/frame_normal_fashu.png";
            card.portraitPath = "cards/portraits/二段跳.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);

            
        }

        {
            CardData card;
            card.id = 1002;
            card.name = "伊利达雷研习";
            card.type = CardType::SPELL;
            card.rarity = CardRarity::NORMAL;
            card.cost = 1;
            //card.attack = 1;
            //card.health = 2;
            card.description = "发现一张流放牌，你的下一张流放牌法力值减小（1）点";
            card.hasBattlecry = true;                       //释放法术牌简化成战吼效果
            card.framePath = "cards/frame_normal_fashu.png";
            card.portraitPath = "cards/portraits_YiLiDaLeiYanXi.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 1003;
            card.name = "凶猛的外来者";
            card.type = CardType::MINION;
            card.rarity = CardRarity::NORMAL;
            card.cost = 1;
            card.attack = 2;
            card.health = 1;
            card.description = "突袭。流放：你的下一张流放牌法力消耗减小（1）点";
            card.hasRush = true;
            card.hasOutcast = true;
            card.framePath = "cards/frame_normal_suicong.png";
            card.portraitPath = "cards/portraits_XiongMengDeWaiLaiZhe.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 1004;
            card.name = "敏捷咒符";
            card.type = CardType::SPELL;
            card.rarity = CardRarity::RARE;
            card.cost = 1;
            //card.attack = 1;
            //card.health = 2;
            card.description = "在下个回合开始时，抽一张牌，并使其法力消耗减小（1）点";
            card.hasBattlecry = true;                       //释放法术牌简化成战吼效果
            card.framePath = "cards/frame_normal_fashu.png";
            card.portraitPath = "cards/portraits_MinJieZhouFu.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 1005;
            card.name = "法力燃烧";
            card.type = CardType::SPELL;
            card.rarity = CardRarity::RARE;
            card.cost = 1;
            //card.attack = 1;
            //card.health = 2;
            card.description = "下个回合，你的对手减少两个法力值";
            card.hasBattlecry = true;                       //释放法术牌简化成战吼效果
            card.framePath = "cards/frame_normal_fashu.png";
            card.portraitPath = "cards/portraits_FaLiRanShao.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 1006;
            card.name = "火色魔印奔行者";
            card.type = CardType::MINION;
            card.rarity = CardRarity::NORMAL;
            card.cost = 1;
            card.attack = 1;
            card.health = 1;
            card.description = "流放：抽一张牌";
            card.hasOutcast = true;
            card.framePath = "cards/frame_normal_suicong.png";
            card.portraitPath = "cards/portraits_HuoSeMoYinBenXingZhe.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 1007;
            card.name = "邪能学说";
            card.type = CardType::SPELL;
            card.rarity = CardRarity::EPIC;
            card.cost = 1;
            //card.attack = 1;
            //card.health = 2;
            card.description = "复制你手牌中法力值消耗最低的恶魔牌。流放：使这两张恶魔牌获得+1/+1";
            card.hasBattlecry = true;                       //释放法术牌简化成战吼效果
            card.hasOutcast = true;
            card.framePath = "cards/frame_normal_fashu.png";
            card.portraitPath = "cards/portraits_XieNengXueShuo.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 1008;
            card.name = "飞行员帕奇斯";
            card.type = CardType::MINION;
            card.rarity = CardRarity::LEGENDARY;
            card.cost = 1;
            card.attack = 1;
            card.health = 1;
            card.description = "战吼：将六张降落伞洗入��的牌库。降落伞的法力消耗为（0）点，可召唤一个1/1有冲锋的海盗";
            card.hasBattlecry = true;
            card.framePath = "cards/frame_normal_suicong.png";
            card.portraitPath = "cards/portraits_FeiXingYuanPaQiSi.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 1009;
            card.name = "幽灵视觉";
            card.type = CardType::SPELL;
            card.rarity = CardRarity::NORMAL;
            card.cost = 2;
            //card.attack = 1;
            //card.health = 2;
            card.description = "抽一张牌。流放：再抽一张";
            card.hasBattlecry = true;                       //释放法术牌简化成战吼效果
            card.hasOutcast = true;
            card.framePath = "cards/frame_normal_fashu.png";
            card.portraitPath = "cards/portraits_YouLingShiJue.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 1010;
            card.name = "时间咒符";
            card.type = CardType::SPELL;
            card.rarity = CardRarity::NORMAL;
            card.cost = 3;
            //card.attack = 1;
            //card.health = 2;
            card.description = "在下个回合开始时，额外抽三张牌";
            card.hasBattlecry = true;                       //释放法术牌简化成战吼效果
            card.framePath = "cards/frame_normal_fashu.png";
            card.portraitPath = "cards/portraits_ShiJianZhouFu.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 1011;
            card.name = "灰烬元素";
            card.type = CardType::MINION;
            card.rarity = CardRarity::EPIC;
            card.cost = 3;
            card.attack = 2;
            card.health = 4;
            card.description = "在下个回合，对手抽牌后，对手会受到2点伤害";
            card.hasBattlecry = true;
            card.framePath = "cards/frame_normal_suicong.png";
            card.portraitPath = "cards/portraits_HuiJinYuanSu.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 1012;
            card.name = "飞翼滑翔";
            card.type = CardType::SPELL;
            card.rarity = CardRarity::RARE;
            card.cost = 3;
            //card.attack = 1;
            //card.health = 2;
            card.description = "双方玩家抽三张牌。流放：只有你抽牌";
            card.hasBattlecry = true;                       //释放法术牌简化成战吼效果
            card.hasOutcast = true;
            card.framePath = "cards/frame_normal_fashu.png";
            card.portraitPath = "cards/portraits_FeiYiHuaXiang.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 1014;
            card.name = "滑翔";
            card.type = CardType::SPELL;
            card.rarity = CardRarity::RARE;
            card.cost = 4;
            //card.attack = 1;
            //card.health = 2;
            card.description = "将你的手牌洗入你的牌库，抽四张牌。流放：对手做出同样行为";
            card.hasBattlecry = true;                       //释放法术牌简化成战吼效果
            card.hasOutcast = true;
            card.framePath = "cards/frame_normal_fashu.png";
            card.portraitPath = "cards/portraits_HuaXiang.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 1015;
            card.name = "极限追逐者阿兰娜";
            card.type = CardType::MINION;
            card.rarity = CardRarity::LEGENDARY;
            card.cost = 5;
            card.attack = 5;
            card.health = 6;
            card.description = "战吼：本局对战剩余时间内你的英雄在你的回合中受到的伤害会转移给敌方英雄";
            card.hasBattlecry = true;
            card.framePath = "cards/frame_normal_suicong.png";
            card.portraitPath = "cards/portraits_JiXiangZhuiZhuZheALanNa.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 1016;
            card.name = "复仇重击者";
            card.type = CardType::MINION;
            card.rarity = CardRarity::RARE;
            card.cost = 7;
            card.attack = 5;
            card.health = 5;
            card.description = "突袭，在本局对战中你每使用一张流放牌，本随从法力消耗减少（1）点";
            card.hasRush = true;
            card.framePath = "cards/frame_normal_suicong.png";
            card.portraitPath = "cards/portraits_FuChouZhongJiZhe.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 1017;
            card.name = "怒缚蛮兵";
            card.type = CardType::MINION;
            card.rarity = CardRarity::NORMAL;
            card.cost = 7;
            card.attack = 6;
            card.health = 7;
            card.description = "讽。在本回合中你每抽一张牌，本牌法力消耗减少（1）点";
            card.hasTaunt = true;
            card.framePath = "cards/frame_normal_suicong.png";
            card.portraitPath = "cards/portraits_NuFuManBing.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        //从此，任务瞎的卡组组建完成
        // 
        //以下是彩虹dk卡组涉及的卡牌

        {
            CardData card;
            card.id = 2001;
            card.name = "奇迹推销员";
            card.type = CardType::MINION;
            card.rarity = CardRarity::NORMAL;
            card.cost = 1;
            card.attack = 2;
            card.health = 2;
            card.description = "奇迹推销员";
            card.framePath = "cards/frame_normal_suicong.png";
            card.portraitPath = "cards/portraits_QiJiTuiXiaoYuan.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 2002;
            card.name = "黑暗符";
            card.type = CardType::SPELL;
            card.rarity = CardRarity::RARE;
            card.cost = 1;
            //card.attack = 1;
            //card.health = 2;
            card.description = "发现一张武器牌";
            card.hasBattlecry = true;                       //放法术牌简化成战吼效果
            card.framePath = "cards/frame_normal_fashu.png";
            card.portraitPath = "cards/portraits_HeiAnFuWen.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 2003;
            card.name = "冰霜打击";
            card.type = CardType::SPELL;
            card.rarity = CardRarity::RARE;
            card.cost = 2;
            //card.attack = 1;
            //card.health = 2;
            card.description = "对一个随从造成3点伤害，如果其死亡，发现一张武器牌";
            card.hasBattlecry = true;                       //释放法术牌简化成战吼效果
            card.framePath = "cards/frame_normal_fashu.png";
            card.portraitPath = "cards/portraits_BingShuangDaJi.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 2004;
            card.name = "恐惧猎犬训练师";
            card.type = CardType::MINION;
            card.rarity = CardRarity::NORMAL;
            card.cost = 2;
            card.attack = 2;
            card.health = 2;
            card.description = "突袭。亡语：召唤一只1/1的恐惧猎犬";
            card.hasDeathrattle = true;
            card.hasRush = true;
            card.framePath = "cards/frame_normal_suicong.png";
            card.portraitPath = "cards/portraits_KongJuLieQuanXunLianShi.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 2005;
            card.name = "甜筒冰淇淋";
            card.type = CardType::SPELL;
            card.rarity = CardRarity::RARE;
            card.cost = 2;
            //card.attack = 1;
            //card.health = 2;
            card.description = "造成3点伤害，消耗3份残骸，在合结束移回你的手牌";
            card.hasBattlecry = true;                       //释放法术牌简化成战吼效果
            card.framePath = "cards/frame_normal_fashu.png";
            card.portraitPath = "cards/portraits_TianTongBingQiLin.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 2006;
            card.name = "采矿事故";
            card.type = CardType::SPELL;
            card.rarity = CardRarity::RARE;
            card.cost = 2;
            //card.attack = 1;
            //card.health = 2;
            card.description = "召唤两个2/2的白银之手新兵";
            card.hasBattlecry = true;                       //释放法术牌简化成战吼效果
            card.framePath = "cards/frame_normal_fashu.png";
            card.portraitPath = "cards/portraits_CaiKuangShiGu.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 2007;
            card.name = "麦芽糖浆";
            card.type = CardType::SPELL;
            card.rarity = CardRarity::RARE;
            card.cost = 2;
            //card.attack = 1;
            //card.health = 2;
            card.description = "对所有敌方随从造成3点伤害";
            card.hasBattlecry = true;                       //释放法术牌简化成战吼效果
            card.framePath = "cards/frame_normal_fashu.png";
            card.portraitPath = "cards/portraits_MaiYaYanJiang.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 2008;
            card.name = "彩虹裁缝";
            card.type = CardType::MINION;
            card.rarity = CardRarity::NORMAL;
            card.cost = 3;
            card.attack = 3;
            card.health = 3;
            card.description = "战吼：获得突袭、圣盾";
            card.hasDeathrattle = true;
            card.hasRush = true;
            card.hasDivineShield = true;
            card.framePath = "cards/frame_normal_suicong.png";
            card.portraitPath = "cards/portraits_CaiHongCaiFeng.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 2009;
            card.name = "戈贡佐姆";
            card.type = CardType::MINION;
            card.rarity = CardRarity::LEGENDARY;
            card.cost = 7;
            card.attack = 3;
            card.health = 3;
            card.description = "战吼：随机召唤你牌库里的3个随从";
            card.hasBattlecry = true;
            card.framePath = "cards/frame_normal_suicong.png";
            card.portraitPath = "cards/portraits_GeGongZuoMu.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 2010;
            card.name = "伊丽扎-刺刃";
            card.type = CardType::MINION;
            card.rarity = CardRarity::LEGENDARY;
            card.cost = 4;
            card.attack = 4;
            card.health = 3;
            card.description = "亡语：在本局剩余时间内，你召唤的随从获得+1攻击力";
            card.hasDeathrattle = true;
            card.framePath = "cards/frame_normal_suicong.png";
            card.portraitPath = "cards/portraits_YiLiZhaCiRen.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 2011;
            card.name = "大地之末号";
            card.type = CardType::MINION;
            card.rarity = CardRarity::RARE;
            card.cost = 4;
            card.attack = 0;
            card.health = 5;
            card.description = "战吼，亡语：造成5点伤害，随机分配到敌人身上";
            card.hasDeathrattle = true;
            card.hasBattlecry = true;
            card.framePath = "cards/frame_normal_suicong.png";
            card.portraitPath = "cards/portraits_DaDiZhiMoHao.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 2012;
            card.name = "食尸鬼之夜";
            card.type = CardType::SPELL;
            card.rarity = CardRarity::RARE;
            card.cost = 4;
            //card.attack = 1;
            //card.health = 2;
            card.description = "召唤5个1/1的食尸鬼并随即攻击敌人";
            card.hasBattlecry = true;                       //释放法术牌简化成战吼效果
            card.framePath = "cards/frame_normal_fashu.png";
            card.portraitPath = "cards/portraits_ShiShiGuiZhiYe.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 2013;
            card.name = "奇利亚斯豪华版3000型";
            card.type = CardType::MINION;
            card.rarity = CardRarity::LEGENDARY;
            card.cost = 9;
            card.attack = 6;
            card.health = 5;
            card.description = "圣盾，嘲讽，剧毒，突袭。战吼：召唤一个本随从的复制";
            card.hasRush = true;
            card.hasBattlecry = true;
            card.hasDivineShield = true;
            card.hasPoison = true;
            card.hasTaunt = true;
            card.framePath = "cards/frame_normal_suicong.png";
            card.portraitPath = "cards/portraits_QiLiYaSiHaoHuaBan3000Xing.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }

        {
            CardData card;
            card.id = 2014;
            card.name = "矿坑老板雷斯卡";
            card.type = CardType::MINION;
            card.rarity = CardRarity::LEGENDARY;
            card.cost = 25;
            card.attack = 6;
            card.health = 3;
            card.description = "突袭。在本局中每有一个随从死亡本牌法力消耗减少（1）点。亡语：随机夺取一个敌人的控制权";
            card.hasDeathrattle = true;
            card.hasRush = true;
            card.framePath = "cards/frame_normal_suicong.png";
            card.portraitPath = "cards/portraits_KuangKengLaoBanLeiSiKa.png";
            _cardTemplates[card.id] = card;
            cardCount++;
            logger->log(LogLevel::DEBUG, "Created card: " + card.name);
        }








        
        logger->log(LogLevel::INFO, "Card templates initialization completed. Total cards: " + std::to_string(cardCount));
        
        // 初始化 allCards 向量（只在这里做一次）
        allCards.clear();
        for (const auto& pair : _cardTemplates) {
            Card* card = createCardById(pair.first);
            if (card) {
                allCards.push_back(card);
            }
        }
        
    } catch (const std::exception& e) {
        logger->log(LogLevel::ERR, "Exception in initCardTemplates: " + std::string(e.what()));
    }
}


Card* CardFactory::createCardById(int cardId) {
    auto logger = GameLogger::getInstance();
    logger->log(LogLevel::DEBUG, "Creating card with ID: " + std::to_string(cardId));
    
    auto it = _cardTemplates.find(cardId);
    if (it == _cardTemplates.end()) {
        logger->log(LogLevel::ERR, "Card template not found for ID: " + std::to_string(cardId));
        return nullptr;
    }
    
    const CardData& data = it->second;
    Card* card = nullptr;
    
    if (data.type == CardType::SPELL) {
        card = createSpellCard(data);
    } else if (data.type == CardType::MINION) {
        card = createMinionCard(data);
    }
    
    if (card) {
        logger->log(LogLevel::INFO, "Successfully created card: " + data.name);
    }
    
    return card;
}

MinionCard* CardFactory::createMinionCard(const CardData& data) {
    auto logger = GameLogger::getInstance();
    logger->log(LogLevel::DEBUG, "Creating minion card: " + data.name);
    
    auto minion = MinionCard::create(data.id, data.name);
    if (!minion)
    {
        logger->log(LogLevel::ERR, "Failed to create MinionCard base object");
        return nullptr;
    }

    try {
        // 设置基本属性
        minion->setType(CardType::MINION);
        minion->setRarity(data.rarity);
        minion->setCost(data.cost);
        minion->setAttack(data.attack);
        minion->setHealth(data.health);
        minion->setMaxHealth(data.health);
        minion->setCardDescription(data.description);
        
        logger->log(LogLevel::DEBUG, "Basic properties set for minion: " + data.name);
        
        // 检查资源文件是否存在
        if (!FileUtils::getInstance()->isFileExist(data.framePath)) {
            logger->log(LogLevel::ERR, "Frame image not found: " + data.framePath);
            return nullptr;
        }
        
        if (!FileUtils::getInstance()->isFileExist(data.portraitPath)) {
            logger->log(LogLevel::ERR, "Portrait image not found: " + data.portraitPath);
            return nullptr;
        }
        
        // 创建并设置框架精灵
        auto frame = Sprite::create(data.framePath);
        if (!frame) {
            logger->log(LogLevel::ERR, "Failed to create frame sprite");
            return nullptr;
        }
        frame->setAnchorPoint(Vec2(0.5f, 0.5f));
        frame->setPosition(minion->getContentSize() / 2);
        frame->setScale(0.4f);
        minion->addChild(frame, 1);
        
        // 创建并设置立绘精灵
        auto portrait = Sprite::create(data.portraitPath);
        if (!portrait) {
            logger->log(LogLevel::ERR, "Failed to create portrait sprite");
            return nullptr;
        }
        portrait->setAnchorPoint(Vec2(0.5f, 0.5f));
        portrait->setPosition(Vec2(minion->getContentSize().width / 2+5, minion->getContentSize().height / 2+65));
        portrait->setScale(0.4f);
        minion->addChild(portrait, 0);
        
        // 添加特殊效果
        if (data.hasBattlecry) {
            minion->addEffect(std::make_shared<BattlecryEffect>([=]() {
                minion->setAttack(minion->getAttack() + 1);
            }));
        }
        
        if (data.hasDivineShield) {
            minion->addEffect(std::make_shared<DivineShieldEffect>());
        }
        
        if (data.hasTaunt) {
            minion->addEffect(std::make_shared<TauntEffect>());
        }
        
        if (data.hasPoison) {
            minion->addEffect(std::make_shared<PoisonousEffect>());
        }

        if (data.hasOutcast) {
            minion->addEffect(std::make_shared<OutcastEffect>([=]() {
                // 定义流放效果
            }));
        }
        
        logger->log(LogLevel::INFO, "Successfully created minion card: " + data.name);
        return minion;
    } catch (const std::exception& e) {
        logger->log(LogLevel::ERR, "Exception in createMinionCard: " + std::string(e.what()));
        return nullptr;
    }
}

SpellCard* CardFactory::createSpellCard(const CardData& data) {
    auto logger = GameLogger::getInstance();
    logger->log(LogLevel::DEBUG, "Creating spell card: " + data.name);
    
    try {
        // 创建法术卡
        auto spell = SpellCard::create(data.id, data.name);
        if (!spell) {
            logger->log(LogLevel::ERR, "Failed to create SpellCard instance");
            return nullptr;
        }
        
        logger->log(LogLevel::DEBUG, "Setting SpellCard properties");
        
        // 设置基本属性
        spell->setType(CardType::SPELL);
        spell->setRarity(data.rarity);
        spell->setCost(data.cost);
        spell->setCardDescription(data.description);
        
        // 设置精灵图片
        if (!data.portraitPath.empty()) {
            auto sprite = Sprite::create(data.portraitPath);
            if (sprite) {
                spell->setSprite(sprite);
                logger->log(LogLevel::DEBUG, "Sprite set successfully");
            } else {
                logger->log(LogLevel::WARNING, "Failed to create sprite: " + data.portraitPath);
            }
        }
        
        logger->log(LogLevel::DEBUG, "SpellCard created successfully");
        return spell;
        
    } catch (const std::exception& e) {
        logger->log(LogLevel::ERR, "Exception in createSpellCard: " + std::string(e.what()));
        return nullptr;
    } catch (...) {
        logger->log(LogLevel::ERR, "Unknown exception in createSpellCard");
        return nullptr;
    }
}

void CardFactory::initializeDecks() {
    auto logger = GameLogger::getInstance();
    try {
        // 清空现有组
        deck1.clear();
        deck2.clear();
        
        // 先确保 allCards 已经被正确初始化
        if (allCards.empty()) {
            logger->log(LogLevel::WARNING, "allCards is empty in initializeDecks");
            return;
        }

        logger->log(LogLevel::INFO, "Total cards in allCards: " + std::to_string(allCards.size()));

        // 遍历所有卡牌
        for (auto& card : allCards) {
            if (!card) {
                logger->log(LogLevel::WARNING, "Found null card pointer in allCards");
                continue;
            }

            // 获取卡牌ID的第一个数字
            int deckNumber = card->getId() / 1000;
            
            // 根据ID分配到不同卡组
            if (deckNumber == 1) {
                card->setCount(2);  // 设置每张卡的初始数量为2
                deck1.push_back(card);
            } else if (deckNumber == 2) {
                card->setCount(2);
                deck2.push_back(card);
            }
        }
        
        logger->log(LogLevel::INFO, "Deck1 size: " + std::to_string(deck1.size()));
        logger->log(LogLevel::INFO, "Deck2 size: " + std::to_string(deck2.size()));
    }
    catch (const std::exception& e) {
        logger->log(LogLevel::ERR, "Exception in initializeDecks: " + std::string(e.what()));
    }
    catch (...) {
        logger->log(LogLevel::ERR, "Unknown exception in initializeDecks");
    }
}

const std::vector<Card*>& CardFactory::getDeck1() const { return deck1; }
const std::vector<Card*>& CardFactory::getDeck2() const { return deck2; }

void CardFactory::setCardCount(int cardId, int count) {
    for (auto& card : allCards) {
        if (card->getId() == cardId) {
            card->setCount(count);
            break;
        }
    }
} 

Card* CardFactory::createCard(int id, const std::string& name) {
    auto logger = GameLogger::getInstance();
    logger->log(LogLevel::DEBUG, "Creating card: " + name);
    
    // 获取卡牌模板
    auto it = _cardTemplates.find(id);
    if (it == _cardTemplates.end()) {
        logger->log(LogLevel::ERR, "Card template not found for ID: " + std::to_string(id));
        return nullptr;
    }
    
    const CardData& cardData = it->second;  // 使用 CardData 而不是 CardTemplate
    
    // 验证名称匹配
    if (cardData.name != name) {
        logger->log(LogLevel::WARNING, 
            "Card name mismatch. Template: " + cardData.name + ", Requested: " + name);
    }
    
    // 创建卡牌实例
    Card* card = nullptr;
    if (cardData.type == CardType::SPELL) {
        card = createSpellCard(cardData);
    } else if (cardData.type == CardType::MINION) {
        card = createMinionCard(cardData);
    }
    
    if (!card) {
        logger->log(LogLevel::ERR, "Failed to create card instance");
        return nullptr;
    }
    
    logger->log(LogLevel::INFO, "Successfully created card: " + cardData.name);
    return card;
} 