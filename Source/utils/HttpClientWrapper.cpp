void HttpClientWrapper::fetchStats(const std::string& playerName, std::function<void(const std::string&)> callback) {
    std::string url = "http://localhost:8080/stats?player=" + playerName;

    ax::network::HttpRequest* request = new ax::network::HttpRequest();
    request->setUrl(url);
    request->setRequestType(ax::network::HttpRequest::Type::GET);
    request->setResponseCallback([callback](ax::network::HttpClient* client, ax::network::HttpResponse* response) {
        if (response && response->isSucceed()) {
            std::string resp(response->getResponseData()->begin(), response->getResponseData()->end());
            callback(resp);
        }
    });
    ax::network::HttpClient::getInstance()->send(request);
    request->release();
}
