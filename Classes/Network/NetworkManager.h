#ifndef __NETWORK_MANAGER_H__
#define __NETWORK_MANAGER_H__
#pragma execution_character_set("utf-8")
#include "cocos2d.h"
#include "Network/WebSocket.h"
#include "Network/GameAction.h"
#include <queue>
#include <functional>

class NetworkManager : public cocos2d::network::WebSocket::Delegate {
public:
    static NetworkManager* getInstance();

    // 连接管理
    bool connect(const std::string& serverIP, int port);
    void disconnect();
    bool reconnect();
    bool isConnected() const;

    // 游戏动作
    void sendGameAction(const GameAction& action);
    void processActionQueue();

    // 状态同步
    void syncGameState();
    void requestGameState();

    // WebSocket回调
    virtual void onOpen(cocos2d::network::WebSocket* ws) override;
    virtual void onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data) override;
    virtual void onClose(cocos2d::network::WebSocket* ws) override;
    virtual void onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error) override;

    // 回调注册
    void setOnConnectedCallback(const std::function<void()>& callback);
    void setOnDisconnectedCallback(const std::function<void()>& callback);
    void setOnActionReceivedCallback(const std::function<void(const GameAction&)>& callback);

private:
    NetworkManager();
    ~NetworkManager();
    static NetworkManager* _instance;

    cocos2d::network::WebSocket* _webSocket;
    std::queue<GameAction> _actionQueue;
    std::string _serverIP;
    int _serverPort;
    bool _isReconnecting;

    // 回调函数
    std::function<void()> _onConnectedCallback;
    std::function<void()> _onDisconnectedCallback;
    std::function<void(const GameAction&)> _onActionReceivedCallback;

    // 重连机制
    void startReconnectTimer();
    void stopReconnectTimer();
    int _reconnectAttempts;
    static const int MAX_RECONNECT_ATTEMPTS = 5;
};

#endif // __NETWORK_MANAGER_H__