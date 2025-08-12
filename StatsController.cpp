#include "StatsController.h"
#include "StatPopup.h"
#include "MatchResult.h"
#include "network/HttpClient.h"
#include "rapidjson/document.h"
#include "StatModel.h"

using namespace ax::network;
using namespace rapidjson;

void StatsController::showStats(ax::Node* parent, const std::string& playerName) {
    HttpRequest* request = new HttpRequest();
    request->setUrl(("http://localhost:8080/stats?player=" + playerName).c_str());
    request->setRequestType(HttpRequest::Type::GET);
    
    request->setResponseCallback([parent](HttpClient* client, HttpResponse* response) {
        if (!response || !response->isSucceed())
            return;
        
        auto buffer = response->getResponseData();
        std::string json(buffer->data(), buffer->size());
        
        std::vector<MatchResult> results = StatModel::parseFromJson(json.c_str());
        StatPopup::createAndShow(parent, results);
    });

    HttpClient::getInstance()->send(request);
    request->release();
}
