// GameManager.h
// ��Ϸ�������� - ���������Ϸ�ĺ����߼���״̬
// ʹ�õ���ģʽȷ��ȫ��ֻ��һ��ʵ��

#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "cocos2d.h"
#include <vector>
#include <queue>

// ǰ������������ѭ������
class Card;
class Player;

// ��Ϸ��������
// ����
// 1. ��Ϸ���̿���
// 2. ��ҹ���
// 3. ���Ʋ���
// 4. ��Ϸ״̬ά��
class GameManager {
public:
    // ��ȡ��Ϸ�������ĵ���ʵ��
    // @return: ���� GameManager ��Ψһʵ��
    static GameManager* getInstance();

    // ====== ��Ϸ���̿��� ======

    // ��ʼ����Ϸ��������Դ
    // ����Ϸ��ʼǰ���ã����ó�ʼ״̬
    void initGame();

    // ��ʼ����Ϸ
    // ������ϴ�ơ��ַ���ʼ���ơ��������ֵ�
    void startGame();

    // ������ǰ��Ϸ
    // ������Դ����ʾ�������
    void endGame();

    // ====== ��ҹ��� ======

    // ���õ�ǰ�ж������
    // @param player: Ҫ����Ϊ��ǰ��ҵ�ָ��
    void setCurrentPlayer(Player* player);

    // ��ȡ��ǰ�ж������
    // @return: ��ǰ��ҵ�ָ��
    Player* getCurrentPlayer() const;

    // ��ȡ�������
    // @return: ������ҵ�ָ��
    Player* getOpponentPlayer() const;

    // ====== ���Ʋ��� ======
    void discardCard(Card* card);//����
    // Ϊָ����ҳ�һ����
    // @param player: Ҫ���Ƶ����
    void drawCard(Player* player);

    // ��ָ��λ�ô��һ�ſ���
    // @param card: Ҫ����Ŀ���
    // @param position: ���Ʒ��õ�Ŀ��λ��
    void playCard(Card* card, const cocos2d::Vec2& position);

    // ������ƿ���ӿ���
    // @param player: Ŀ�����
    // @param card: Ҫ��ӵĿ���
    void addCardToDeck(Player* player, Card* card);

    // Ŀ��ѡ�����
    void setSelectedTarget(Card* target);  // ���õ�ǰѡ�е�Ŀ��
    Card* getSelectedTarget() const;       // ��ȡ��ǰѡ�е�Ŀ��
    void clearSelectedTarget();            // �����ǰѡ�е�Ŀ��
    // ====== ��Ϸ״̬ ======

    // �����Ϸ�Ƿ����
    // @return: �����Ϸ�������� true�����򷵻� false
    bool isGameOver() const;

    // �����Ϸ״̬
    // ���������ʤ���������غ����Ƶ�
    void checkGameState();

private:
    // ˽�й��캯������ֹ�ⲿֱ�Ӵ���ʵ��
    GameManager();

    // ˽������������ȷ����ȷ������Դ
    ~GameManager();

    // ����ʵ��ָ��
    static GameManager* _instance;

    // ��ǰ�ж����
    Player* _currentPlayer;

    // �������
    Player* _opponentPlayer;

    // ��Ϸ�Ƿ�����ı�־
    bool _isGameOver;

    // ��ǰѡ�е�Ŀ�꿨��
    Card* _selectedTarget;

    // ====== ���ÿ������� ======

    // ���ÿ������캯��
    // ȷ���������ⴴ�����󸱱�
    GameManager(const GameManager&) = delete;

    // ���ø�ֵ�����
    // ȷ���������⸴�ƶ���
    GameManager& operator=(const GameManager&) = delete;
};

#endif // __GAME_MANAGER_H__

/*
ʹ��ʾ����

// ��ȡ��Ϸ������ʵ��
auto gameManager = GameManager::getInstance();

// ��ʼ������ʼ����Ϸ
gameManager->initGame();
gameManager->startGame();

// ִ����Ϸ����
gameManager->drawCard(gameManager->getCurrentPlayer());

// �����Ϸ״̬
if (gameManager->isGameOver()) {
    gameManager->endGame();
}
*/