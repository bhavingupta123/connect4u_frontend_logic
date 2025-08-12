#include "Controller/NetworkController.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace ax;
using namespace ax::network;
using namespace rapidjson;

NetworkController* NetworkController::create(const std::string& url) {
    auto* instance = new NetworkController();
    if (instance && instance->init(url)) {
        instance->autorelease();
        return instance;
    }
    delete instance;
    return nullptr;
}

bool NetworkController::init(const std::string& url) {
    _ws = new WebSocket();
    if (!_ws)
        return false;

    _ws->open(this, url);
    return true;
}

void NetworkController::sendMove(int column) {
    Document d;
    d.SetObject();
    auto& alloc = d.GetAllocator();
    d.AddMember("type", "move", alloc);
    d.AddMember("column", column, alloc);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    _ws->send(buffer.GetString());
}

void NetworkController::sendRematchRequest() {
    _ws->send(R"({"type":"rematch_request"})");
}

void NetworkController::sendRematchCancel() {
    _ws->send(R"({"type":"rematch_cancelled"})");
}

void NetworkController::sendRaw(const std::string& message) {
    if (_ws) _ws->send(message);
}

void NetworkController::setOnMessageCallback(std::function<void(const std::string&)> callback) {
    _onMessage = std::move(callback);
}

void NetworkController::setOnOpenCallback(std::function<void()> callback) {
    _onOpen = std::move(callback);
}

// WebSocket Delegate Callbacks
void NetworkController::onOpen(WebSocket* ws) {
    AXLOG("WebSocket connected");
    if (_onOpen) _onOpen();
}

void NetworkController::onMessage(WebSocket* ws, const WebSocket::Data& data) {
    if (_onMessage) {
        _onMessage(data.bytes);
    }
}

void NetworkController::onClose(WebSocket* ws, uint16_t code, std::string_view reason) {
    AXLOG("WebSocket closed: %d - %s", code, std::string(reason).c_str());
}

void NetworkController::onError(WebSocket* ws, const WebSocket::ErrorCode& error) {
    AXLOG("WebSocket error occurred: %d", static_cast<int>(error));
}

void NetworkController::close() {
    if (_ws) {
        _ws->close();
        delete _ws;
        _ws = nullptr;
    }
}
