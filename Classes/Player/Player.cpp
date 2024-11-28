#include "Player.h"
#include "Card/Card.h"

Player::Player()
    : _health(30)
    , _maxHealth(30)
    , _armor(0)
    , _fatigueDamage(1)
    , _mana(0)          // 初始法力值
    , _maxMana(0)       // 初始最大法力值
    , _tempMana(0)      // 初始临时法力值 
{
}

Player::~Player() {
    // 清理卡牌
    for (auto card : _deck) delete card;
    for (auto card : _hand) delete card;
    for (auto card : _field) delete card;
}

void Player::takeDamage(int amount) {
    // 先扣护甲
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

    // 扣生命值
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
// 消耗法力值
void Player::spendMana(int amount) {
    // 优先使用临时法力值
    int tempUsed = std::min(_tempMana, amount);
    _tempMana -= tempUsed;
    amount -= tempUsed;

    // 使用普通法力值
    _mana = std::max(0, _mana - amount);
}

// 回合开始时恢复法力值
void Player::restoreMana() {
    _mana = _maxMana;
    _tempMana = 0;  // 清除临时法力值
}

// 获得临时法力值
void Player::gainMana(int amount) {
    _tempMana += amount;
}

// 增加最大法力值（每回合）
void Player::incrementMaxMana() {
    if (_maxMana < 10) {  // 最大法力值上限为10
        _maxMana++;
        _mana = _maxMana;
    }
}

// 检查法力值是否足够
bool Player::hasEnoughMana(int cost) const {
    return (_mana + _tempMana) >= cost;
}

void Player::removeFromField(Card* card) {
    // 从场上移除卡牌
    auto it = std::find(_field.begin(), _field.end(), card);
    if (it != _field.end()) {
        _field.erase(it);
    }
}