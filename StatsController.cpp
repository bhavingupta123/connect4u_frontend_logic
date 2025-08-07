#include "StatsController.h"
#include "StatPopup.h"
#include "MatchResult.h"
#include "network/HttpClient.h"
#include "rapidjson/document.h"

using namespace ax::network;
using namespace rapidjson;

void StatsController::showStats(ax::Node* parent, const std::string& playerName) {
    AXLOG("stats is called");
    HttpRequest* request = new HttpRequest();
    request->setUrl(("http://localhost:8080/stats?player=" + playerName).c_str());
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback([parent](HttpClient* client, HttpResponse* response) {
        if (!response || !response->isSucceed()) return;
        
        auto buffer = response->getResponseData();
        std::string json(buffer->data(), buffer->size());
        AXLOG("Received JSON: %s", json.c_str());

        Document doc;
        doc.Parse(json.c_str());
        AXLOG("stats is called2");

        std::vector<MatchResult> results;

        if (doc.IsObject() && doc.HasMember("results") && doc["results"].IsArray()) {
            for (auto& entry : doc["results"].GetArray()) {
                if (entry.HasMember("Result") && entry.HasMember("Opponent")) {
                    MatchResult r;
                    r.result = entry["Result"].GetString();
                    r.opponent = entry["Opponent"].GetString(); 
                    AXLOG("Parsed stat: result %s vs %s", r.result.c_str(), r.opponent.c_str());
                    results.push_back(r);
                }
            }
        } else {
            AXLOG("Stats JSON format unexpected or empty.");
        }

        StatPopup::createAndShow(parent, results);
    });

    HttpClient::getInstance()->send(request);
    request->release();
}
