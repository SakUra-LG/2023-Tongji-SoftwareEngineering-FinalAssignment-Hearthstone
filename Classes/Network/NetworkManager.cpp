#include "NetworkManager.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "Utils/GameLogger.h"
#pragma execution_character_set("utf-8")
// 单例实例指针初始化
NetworkManager* NetworkManager::_instance = nullptr;

/**
 * @brief 获取NetworkManager的单例实例
 * @return NetworkManager的唯一实例
 */
NetworkManager* NetworkManager::getInstance() {
    if (!_instance) {
        _instance = new NetworkManager();
    }
    return _instance;
}

/**
 * @brief 构造函数，初始化成员变量
 */
NetworkManager::NetworkManager()
    : _webSocket(nullptr)        // WebSocket连接指针初始化为空
    , _serverPort(0)            // 服务器端口初始化为0
    , _isReconnecting(false)    // 重连状态初始化为false
    , _reconnectAttempts(0) {   // 重连尝试次数初始化为0
}

/**
 * @brief 连接到WebSocket服务器
 * @param serverIP 服务器IP地址
 * @param port 服务器端口
 * @return 连接是否成功
 */
bool NetworkManager::connect(const std::string& serverIP, int port) {
    // 如果已经存在连接，先断开
    if (_webSocket) {
        disconnect();
    }

    // 保存服务器信息
    _serverIP = serverIP;
    _serverPort = port;

    // 构建WebSocket URL
    std::string url = "ws://" + serverIP + ":" + std::to_string(port);
    _webSocket = new cocos2d::network::WebSocket();

    // 初始化WebSocket连接
    if (!_webSocket->init(*this, url)) {
        delete _webSocket;
        _webSocket = nullptr;
        return false;
    }

    return true;
}

/**
 * @brief 发送游戏动作到服务器
 * @param action 要发送的游戏动作
 */
void NetworkManager::sendGameAction(const GameAction& action) {
    // ... (保持现有注释)
}

/**
 * @brief 处理从服务器接收到的消息
 * @param ws WebSocket实例
 * @param data 接收到的数据
 */
void NetworkManager::onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data) {
    // ... (保持现有注释)
}

/**
 * @brief 断开WebSocket连接
 */
void NetworkManager::disconnect() {
    if (_webSocket) {
        _webSocket->close();     // 关闭连接
        delete _webSocket;       // 释放资源
        _webSocket = nullptr;    // 置空指针
    }
    _isReconnecting = false;     // 重置重连状态
}

/**
 * @brief 检查是否已连接到服务器
 * @return 是否已连接
 */
bool NetworkManager::isConnected() const {
    return _webSocket && _webSocket->getReadyState() == cocos2d::network::WebSocket::State::OPEN;
}

/**
 * @brief WebSocket连接成功时的回调
 * @param ws WebSocket实例
 */
void NetworkManager::onOpen(cocos2d::network::WebSocket* ws) {
    _isReconnecting = false;     // 重置重连状态
    _reconnectAttempts = 0;      // 重置重连尝试次数

    // 如果设置了连接成功回调，则调用
    if (_onConnectedCallback) {
        _onConnectedCallback();
    }
}
void NetworkManager::onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error) {
    // 记录错误日志
    GameLogger::getInstance()->log(LogLevel::ERR,
        "WebSocket error occurred: " + std::to_string(static_cast<int>(error)));

    // 根据错误类型处理
    if (!_isReconnecting && _reconnectAttempts < MAX_RECONNECT_ATTEMPTS) {
        startReconnectTimer();
    }

    // 记录具体错误信息
    GameLogger::getInstance()->log(LogLevel::ERR,
        "WebSocket connection failed, attempting to reconnect...");
}void NetworkManager::startReconnectTimer() {
    if (_isReconnecting) {
        return;  // 已经在重连中
    }

    _isReconnecting = true;
    _reconnectAttempts++;

    // 计算重连延迟（使用指数退避）
    float delay = std::min(1.0f * (1 << _reconnectAttempts), 30.0f);  // 最大30秒

    // 记录重连尝试
    GameLogger::getInstance()->log(LogLevel::INFO,
        "Starting reconnection attempt " + std::to_string(_reconnectAttempts) +
        " in " + std::to_string(delay) + " seconds");

    // 使用 cocos2d 的调度器来延迟执行重连
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

    stopReconnectTimer();  // 停止重连计时器

    // 尝试重新连接
    if (connect(_serverIP, _serverPort)) {
        GameLogger::getInstance()->log(LogLevel::INFO, "Reconnection successful");
        return true;
    }
    else {
        GameLogger::getInstance()->log(LogLevel::ERR, "Reconnection failed");

        // 如果还可以继续重试，启动下一次重连计时器
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
 * @brief WebSocket连接关闭时的回调
 * @param ws WebSocket实例
 */
void NetworkManager::onClose(cocos2d::network::WebSocket* ws) {
    // 调用断开连接回调
    if (_onDisconnectedCallback) {
        _onDisconnectedCallback();
    }

    // 如果未在重连且未超过最大重连次数，开始重连
    if (!_isReconnecting && _reconnectAttempts < MAX_RECONNECT_ATTEMPTS) {
        startReconnectTimer();
    }
}

/**
 * @brief WebSocket发生错误时的回调
 * @param ws WebSocket实例
 * @param error 错误代码
 */

/**
 * @brief 析构函数，确保清理资源
 */
NetworkManager::~NetworkManager() {
    disconnect();  // 断开连接并清理资源
}
