#include "HttpClientWrapper.h"
#include <iostream>

using namespace ax::network;

void HttpClientWrapper::fetchStats(const std::string& playerName, const std::function<void(const std::string&)>& callback) {
    std::string url = "http://localhost:8080/stats?player=" + playerName;

    auto* request = new HttpRequest();
    request->setUrl(url.c_str());
    request->setRequestType(HttpRequest::Type::GET);

    request->setResponseCallback([callback](HttpClient* client, HttpResponse* response) {
        if (!response || !response->isSucceed()) {
            std::cerr << "[HttpClientWrapper] HTTP request failed\n";
            callback("");
            return;
        }

        auto* buffer = response->getResponseData();
        if (buffer && !buffer->empty()) {
            std::string result(buffer->data(), buffer->size());
            callback(result);
        } else {
            std::cerr << "[HttpClientWrapper] Empty response buffer\n";
            callback("");
        }
    });

    HttpClient::getInstance()->send(request);
    request->release();
}
