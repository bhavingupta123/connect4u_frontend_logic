#ifndef __NETWORK_CONTROLLER_H__
#define __NETWORK_CONTROLLER_H__

#include "axmol.h"
#include "network/WebSocket.h"
#include <string>
#include <functional>

class NetworkController : public ax::Object, public ax::network::WebSocket::Delegate {
public:
    static NetworkController* create(const std::string& url);
    bool init(const std::string& url);

    void sendMove(int column);
    void sendRematchRequest();
    void sendRematchCancel();

    void setOnMessageCallback(std::function<void(const std::string&)> callback);

    void onOpen(ax::network::WebSocket* ws) override;
    void onMessage(ax::network::WebSocket* ws, const ax::network::WebSocket::Data& data) override;
    void onClose(ax::network::WebSocket* ws, uint16_t code, std::string_view reason) override;
    void onError(ax::network::WebSocket* ws, const ax::network::WebSocket::ErrorCode& error) override;
    void sendRaw(const std::string& message);
    void setOnOpenCallback(std::function<void()> callback);
    std::function<void()> _onOpen;
    
private:
    ax::network::WebSocket* _ws = nullptr;
    std::function<void(const std::string&)> _onMessage;
};

#endif // __NETWORK_CONTROLLER_H__
