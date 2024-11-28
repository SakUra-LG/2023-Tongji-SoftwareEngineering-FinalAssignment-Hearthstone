#include "Player.h"
#include "Card/Card.h"

Player::Player()
    : _health(30)
    , _maxHealth(30)
    , _armor(0)
    , _fatigueDamage(1)
    , _mana(0)          // ��ʼ����ֵ
    , _maxMana(0)       // ��ʼ�����ֵ
    , _tempMana(0)      // ��ʼ��ʱ����ֵ 
{
}

Player::~Player() {
    // ������
    for (auto card : _deck) delete card;
    for (auto card : _hand) delete card;
    for (auto card : _field) delete card;
}

void Player::takeDamage(int amount) {
    // �ȿۻ���
    if (_armor > 0) {
        if (_armor >= amount) {
            _armor -= amount;
            return;
        }
        else {
            amount -= _armor;
            _armor = 0;
        }
    }

    // ������ֵ
    _health = std::max(0, _health - amount);
}
void Player::removeCardFromHand(Card* card) {
    auto it = std::find(_hand.begin(), _hand.end(), card);
    if (it != _hand.end()) {
        _hand.erase(it);
    }
}
void Player::heal(int amount) {
    _health = std::min(_maxHealth, _health + amount);
}

void Player::addArmor(int amount) {
    _armor += amount;
}

void Player::increaseFatigueDamage() {
    _fatigueDamage++;
}
// ���ķ���ֵ
void Player::spendMana(int amount) {
    // ����ʹ����ʱ����ֵ
    int tempUsed = std::min(_tempMana, amount);
    _tempMana -= tempUsed;
    amount -= tempUsed;

    // ʹ����ͨ����ֵ
    _mana = std::max(0, _mana - amount);
}

// �غϿ�ʼʱ�ָ�����ֵ
void Player::restoreMana() {
    _mana = _maxMana;
    _tempMana = 0;  // �����ʱ����ֵ
}

// �����ʱ����ֵ
void Player::gainMana(int amount) {
    _tempMana += amount;
}

// ���������ֵ��ÿ�غϣ�
void Player::incrementMaxMana() {
    if (_maxMana < 10) {  // �����ֵ����Ϊ10
        _maxMana++;
        _mana = _maxMana;
    }
}

// ��鷨��ֵ�Ƿ��㹻
bool Player::hasEnoughMana(int cost) const {
    return (_mana + _tempMana) >= cost;
}

void Player::removeFromField(Card* card) {
    // �ӳ����Ƴ�����
    auto it = std::find(_field.begin(), _field.end(), card);
    if (it != _field.end()) {
        _field.erase(it);
    }
}