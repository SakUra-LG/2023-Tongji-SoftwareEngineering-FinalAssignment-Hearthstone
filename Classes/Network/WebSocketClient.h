 
#ifndef __WEBSOCKET_CLIENT_H__
#define __WEBSOCKET_CLIENT_H__

#include "cocos2d.h"
#include "network/WebSocket.h"
#include <functional>
#include <queue>
#include <string>

class WebSocketClient : public cocos2d::network::WebSocket::Delegate {
public:
    // ����״̬
    enum class State {
        DISCONNECTED,
        CONNECTING,
        CONNECTED
    };

    static WebSocketClient* getInstance();

    // ���ӹ���
    bool connect(const std::string& url);
    void disconnect();
    bool isConnected() const { return _state == State::CONNECTED; }

    // ��Ϣ����
    void sendMessage(const std::string& message);
    void sendBinary(const char* data, unsigned int len);

    // �ص�����
    void setOnConnectedCallback(const std::function<void()>& callback);
    void setOnMessageCallback(const std::function<void(const std::string&)>& callback);
    void setOnErrorCallback(const std::function<void(const std::string&)>& callback);
    void setOnClosedCallback(const std::function<void()>& callback);

    // WebSocket������
    virtual void onOpen(cocos2d::network::WebSocket* ws) override;
    virtual void onMessage(cocos2d::network::WebSocket* ws,
        const cocos2d::network::WebSocket::Data& data) override;
    virtual void onClose(cocos2d::network::WebSocket* ws) override;
    virtual void onError(cocos2d::network::WebSocket* ws,
        const cocos2d::network::WebSocket::ErrorCode& error) override;

private:
    WebSocketClient();
    ~WebSocketClient();

    static WebSocketClient* _instance;
    cocos2d::network::WebSocket* _ws;
    State _state;

    // �ص�����
    std::function<void()> _onConnectedCallback;
    std::function<void(const std::string&)> _onMessageCallback;
    std::function<void(const std::string&)> _onErrorCallback;
    std::function<void()> _onClosedCallback;

};
#endif 