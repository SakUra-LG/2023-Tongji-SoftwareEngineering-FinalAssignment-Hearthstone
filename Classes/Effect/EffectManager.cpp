// EffectManager.cpp
// Ч����������ʵ���ļ����������ʹ�����Ϸ�еĸ���Ч��

#include "EffectManager.h"
#include "Card/Card.h"

// ��̬ʵ��ָ���ʼ��
EffectManager* EffectManager::_instance = nullptr;

// ��ȡЧ���������ĵ���ʵ��
// @return: ���� EffectManager ��Ψһʵ��
EffectManager* EffectManager::getInstance() {
    if (!_instance) {
        _instance = new EffectManager();  // ����ʽ����ģʽ
    }
    return _instance;
}

// ���캯��
// ��ʼ��Ч������״̬Ϊδ����
EffectManager::EffectManager()
    : _isProcessingEffects(false) {  // ��ֹЧ������ʱ�ĵݹ����
}

// ע��һ��Ч����������
// @param card: Ч�������Ŀ���
// @param effect: Ҫע���Ч��
// @param trigger: Ч���Ĵ�������
void EffectManager::registerEffect(Card* card, std::shared_ptr<IEffect> effect, TriggerType trigger) {
    // ʹ�� multimap �洢Ч����������Ч��ʹ����ͬ�Ĵ�������
    _effects.insert({ trigger, {card, effect} });
}

// ע��ָ�����Ƶ�����Ч��
// @param card: Ҫע��Ч���Ŀ���
void EffectManager::unregisterEffect(Card* card) {
    // ��������Ч�����Ƴ�����ָ�����Ƶ�Ч��
    for (auto it = _effects.begin(); it != _effects.end();) {
        if (it->second.first == card) {
            // ���Ч������ָ�����ƣ���ɾ��
            it = _effects.erase(it);
        }
        else {
            // ������������һ��Ч��
            ++it;
        }
    }
}

// ����ָ�����͵�����Ч��
// @param trigger: Ҫ������Ч������
void EffectManager::triggerEffects(TriggerType trigger) {
    // ��ȡ���о���ָ���������͵�Ч��
    auto range = _effects.equal_range(trigger);

    // ��������ƥ���Ч��
    for (auto it = range.first; it != range.second; ++it) {
        // ��鿨���Ƿ񼤻���Ч���Ƿ���Դ���
        if (it->second.first->isActive() && it->second.second->canActivate()) {
            // �����Դ�����Ч���������
            _effectQueue.push(it->second.second);
        }
    }

    // �����ǰû���ڴ���Ч�����У���ʼ����
    if (!_isProcessingEffects) {
        processEffectQueue();
    }
}
void EffectManager::processEffectQueue() {
    if (_isProcessingEffects) {
        return;  // ��ֹ�ݹ����
    }

    _isProcessingEffects = true;

    // ��������е�����Ч��
    while (!_effectQueue.empty()) {
        auto effect = _effectQueue.front();
        _effectQueue.pop();

        // ���Ч���Ƿ���Ч�����Լ���
        if (effect && effect->canActivate()) {
            try {
                effect->onActivate();
            }
            catch (const std::exception& e) {
                CCLOG("Effect activation failed: %s", e.what());
            }
        }
    }

    _isProcessingEffects = false;
}

void EffectManager::clearEffects() {
    // ���Ч������
    std::queue<std::shared_ptr<IEffect>> empty;
    std::swap(_effectQueue, empty);

    // ���Ч��ע���
    _effects.clear();

    // ���ô���״̬
    _isProcessingEffects = false;
}
/*
ʹ��ʾ����

// ע��Ч��
auto effect = std::make_shared<SomeEffect>();
EffectManager::getInstance()->registerEffect(card, effect, TriggerType::ON_PLAY);

// ����Ч��
EffectManager::getInstance()->triggerEffects(TriggerType::ON_PLAY);

// ע��Ч��
EffectManager::getInstance()->unregisterEffect(card);
*/

/*
���ܵ���չ��
1. ���Ч�����ȼ�ϵͳ
2. ���Ч�����������
3. ���Ч���ĳ���ʱ�����
4. ���Ч���ĵ��ӹ���
5. ���Ч���Ļ�����
*/