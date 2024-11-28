#include "NetworkManager.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "Utils/GameLogger.h"
// ����ʵ��ָ���ʼ��
NetworkManager* NetworkManager::_instance = nullptr;

/**
 * @brief ��ȡNetworkManager�ĵ���ʵ��
 * @return NetworkManager��Ψһʵ��
 */
NetworkManager* NetworkManager::getInstance() {
    if (!_instance) {
        _instance = new NetworkManager();
    }
    return _instance;
}

/**
 * @brief ���캯������ʼ����Ա����
 */
NetworkManager::NetworkManager()
    : _webSocket(nullptr)        // WebSocket����ָ���ʼ��Ϊ��
    , _serverPort(0)            // �������˿ڳ�ʼ��Ϊ0
    , _isReconnecting(false)    // ����״̬��ʼ��Ϊfalse
    , _reconnectAttempts(0) {   // �������Դ�����ʼ��Ϊ0
}

/**
 * @brief ���ӵ�WebSocket������
 * @param serverIP ������IP��ַ
 * @param port �������˿�
 * @return �����Ƿ�ɹ�
 */
bool NetworkManager::connect(const std::string& serverIP, int port) {
    // ����Ѿ��������ӣ��ȶϿ�
    if (_webSocket) {
        disconnect();
    }

    // �����������Ϣ
    _serverIP = serverIP;
    _serverPort = port;

    // ����WebSocket URL
    std::string url = "ws://" + serverIP + ":" + std::to_string(port);
    _webSocket = new cocos2d::network::WebSocket();

    // ��ʼ��WebSocket����
    if (!_webSocket->init(*this, url)) {
        delete _webSocket;
        _webSocket = nullptr;
        return false;
    }

    return true;
}

/**
 * @brief ������Ϸ������������
 * @param action Ҫ���͵���Ϸ����
 */
void NetworkManager::sendGameAction(const GameAction& action) {
    // ... (��������ע��)
}

/**
 * @brief ����ӷ��������յ�����Ϣ
 * @param ws WebSocketʵ��
 * @param data ���յ�������
 */
void NetworkManager::onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data) {
    // ... (��������ע��)
}

/**
 * @brief �Ͽ�WebSocket����
 */
void NetworkManager::disconnect() {
    if (_webSocket) {
        _webSocket->close();     // �ر�����
        delete _webSocket;       // �ͷ���Դ
        _webSocket = nullptr;    // �ÿ�ָ��
    }
    _isReconnecting = false;     // ��������״̬
}

/**
 * @brief ����Ƿ������ӵ�������
 * @return �Ƿ�������
 */
bool NetworkManager::isConnected() const {
    return _webSocket && _webSocket->getReadyState() == cocos2d::network::WebSocket::State::OPEN;
}

/**
 * @brief WebSocket���ӳɹ�ʱ�Ļص�
 * @param ws WebSocketʵ��
 */
void NetworkManager::onOpen(cocos2d::network::WebSocket* ws) {
    _isReconnecting = false;     // ��������״̬
    _reconnectAttempts = 0;      // �����������Դ���

    // ������������ӳɹ��ص��������
    if (_onConnectedCallback) {
        _onConnectedCallback();
    }
}
void NetworkManager::onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error) {
    // ��¼������־
    GameLogger::getInstance()->log(LogLevel::ERR,
        "WebSocket error occurred: " + std::to_string(static_cast<int>(error)));

    // ���ݴ������ʹ���
    if (!_isReconnecting && _reconnectAttempts < MAX_RECONNECT_ATTEMPTS) {
        startReconnectTimer();
    }

    // ��¼���������Ϣ
    GameLogger::getInstance()->log(LogLevel::ERR,
        "WebSocket connection failed, attempting to reconnect...");
}void NetworkManager::startReconnectTimer() {
    if (_isReconnecting) {
        return;  // �Ѿ���������
    }

    _isReconnecting = true;
    _reconnectAttempts++;

    // ���������ӳ٣�ʹ��ָ���˱ܣ�
    float delay = std::min(1.0f * (1 << _reconnectAttempts), 30.0f);  // ���30��

    // ��¼��������
    GameLogger::getInstance()->log(LogLevel::INFO,
        "Starting reconnection attempt " + std::to_string(_reconnectAttempts) +
        " in " + std::to_string(delay) + " seconds");

    // ʹ�� cocos2d �ĵ��������ӳ�ִ������
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->schedule([this](float dt) {
        this->reconnect();
        }, this, delay, 0, 0.0f, false, "reconnect_timer");
}

void NetworkManager::stopReconnectTimer() {
    _isReconnecting = false;
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->unschedule("reconnect_timer", this);
}

bool NetworkManager::reconnect() {
    GameLogger::getInstance()->log(LogLevel::INFO, "Attempting to reconnect...");

    stopReconnectTimer();  // ֹͣ������ʱ��

    // ������������
    if (connect(_serverIP, _serverPort)) {
        GameLogger::getInstance()->log(LogLevel::INFO, "Reconnection successful");
        return true;
    }
    else {
        GameLogger::getInstance()->log(LogLevel::ERR, "Reconnection failed");

        // ��������Լ������ԣ�������һ��������ʱ��
        if (_reconnectAttempts < MAX_RECONNECT_ATTEMPTS) {
            startReconnectTimer();
        }
        else {
            GameLogger::getInstance()->log(LogLevel::ERR,
                "Max reconnection attempts reached. Giving up.");
        }
        return false;
    }
}

void NetworkManager::setOnConnectedCallback(const std::function<void()>& callback) {
    _onConnectedCallback = callback;
}

void NetworkManager::setOnDisconnectedCallback(const std::function<void()>& callback) {
    _onDisconnectedCallback = callback;
}

void NetworkManager::setOnActionReceivedCallback(const std::function<void(const GameAction&)>& callback) {
    _onActionReceivedCallback = callback;
}
/**
 * @brief WebSocket���ӹر�ʱ�Ļص�
 * @param ws WebSocketʵ��
 */
void NetworkManager::onClose(cocos2d::network::WebSocket* ws) {
    // ���öϿ����ӻص�
    if (_onDisconnectedCallback) {
        _onDisconnectedCallback();
    }

    // ���δ��������δ�������������������ʼ����
    if (!_isReconnecting && _reconnectAttempts < MAX_RECONNECT_ATTEMPTS) {
        startReconnectTimer();
    }
}

/**
 * @brief WebSocket��������ʱ�Ļص�
 * @param ws WebSocketʵ��
 * @param error �������
 */

/**
 * @brief ����������ȷ��������Դ
 */
NetworkManager::~NetworkManager() {
    disconnect();  // �Ͽ����Ӳ�������Դ
}
