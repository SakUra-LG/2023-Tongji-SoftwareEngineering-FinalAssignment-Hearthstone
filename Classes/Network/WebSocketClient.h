 
#ifndef __WEBSOCKET_CLIENT_H__
#define __WEBSOCKET_CLIENT_H__

#include "cocos2d.h"
#include "network/WebSocket.h"
#include <functional>
#include <queue>
#include <string>

class WebSocketClient : public cocos2d::network::WebSocket::Delegate {
public:
    // 连接状态
    enum class State {
        DISCONNECTED,
        CONNECTING,
        CONNECTED
    };

    static WebSocketClient* getInstance();

    // 连接管理
    bool connect(const std::string& url);
    void disconnect();
    bool isConnected() const { return _state == State::CONNECTED; }

    // 消息发送
    void sendMessage(const std::string& message);
    void sendBinary(const char* data, unsigned int len);

    // 回调设置
    void setOnConnectedCallback(const std::function<void()>& callback);
    void setOnMessageCallback(const std::function<void(const std::string&)>& callback);
    void setOnErrorCallback(const std::function<void(const std::string&)>& callback);
    void setOnClosedCallback(const std::function<void()>& callback);

    // WebSocket代理方法
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

    // 回调函数
    std::function<void()> _onConnectedCallback;
    std::function<void(const std::string&)> _onMessageCallback;
    std::function<void(const std::string&)> _onErrorCallback;
    std::function<void()> _onClosedCallback;

};
#endif 