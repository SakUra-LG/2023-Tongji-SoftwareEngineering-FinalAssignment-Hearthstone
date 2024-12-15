#ifndef __DECK_BUILDER_SCENE_H__
#define __DECK_BUILDER_SCENE_H__

#include "cocos2d.h"
#include "Card/Card.h"
#include "Card/CardFactory.h"
#include <vector>

class DeckBuilderScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(DeckBuilderScene);

    virtual ~DeckBuilderScene() {
        for (auto card : _allCards) {
            if (card) {
                card->release();
            }
        }
        _allCards.clear();
        _displayedCards.clear();
    }

private:
    void initBackground();
    void initMenu();
    void initUI();
    void initCards();  // 初始化卡牌展示
    
    void displayCardsForCurrentPage();  // 显示当前页的卡牌
    void updatePageDisplay();           // 更新页码显示
    
    void onPageRight(cocos2d::Ref* sender);  // 下一页
    void onPageLeft(cocos2d::Ref* sender);   // 上一页
    void onReturnMenu(cocos2d::Ref* sender); // 返回菜单
    void DeckBuilderScene::loadCards();

    // 卡牌相关变量
    std::vector<Card*> _allCards;       // 所有可用卡牌
    std::vector<Card*> _displayedCards; // 当前显示的卡牌
    
    int _currentPage = 0;               // 当前页码
    const int CARDS_PER_PAGE = 8;       // 每页显示的卡牌数量
    const float CARD_SCALE = 0.8f;      // 卡牌缩放比例
    
    cocos2d::Node* _cardContainer;      // 卡牌容器节点
    cocos2d::Label* _pageLabel;         // 页码显示标签
};

#endif // __DECK_BUILDER_SCENE_H__
