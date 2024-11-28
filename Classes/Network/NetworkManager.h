#ifndef __NETWORK_MANAGER_H__
#define __NETWORK_MANAGER_H__

#include "cocos2d.h"
#include "Network/WebSocket.h"
#include "Network/GameAction.h"
#include <queue>
#include <functional>

class NetworkManager : public cocos2d::network::WebSocket::Delegate {
public:
    static NetworkManager* getInstance();

    // ���ӹ���
    bool connect(const std::string& serverIP, int port);
    void disconnect();
    bool reconnect();
    bool isConnected() const;

    // ��Ϸ����
    void sendGameAction(const GameAction& action);
    void processActionQueue();

    // ״̬ͬ��
    void syncGameState();
    void requestGameState();

    // WebSocket�ص�
    virtual void onOpen(cocos2d::network::WebSocket* ws) override;
    virtual void onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data) override;
    virtual void onClose(cocos2d::network::WebSocket* ws) override;
    virtual void onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error) override;

    // �ص�ע��
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

    // �ص�����
    std::function<void()> _onConnectedCallback;
    std::function<void()> _onDisconnectedCallback;
    std::function<void(const GameAction&)> _onActionReceivedCallback;

    // ��������
    void startReconnectTimer();
    void stopReconnectTimer();
    int _reconnectAttempts;
    static const int MAX_RECONNECT_ATTEMPTS = 5;
};

#endif // __NETWORK_MANAGER_H__