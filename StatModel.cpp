#include "StatModel.h"
#include "rapidjson/document.h"
#include "MatchResult.h"
#include "axmol.h"

using namespace rapidjson;

std::vector<MatchResult> StatModel::parseFromJson(const std::string& json) {
    std::vector<MatchResult> results;
    Document doc;
    doc.Parse(json.c_str());

    if (doc.IsObject() && doc.HasMember("results") && doc["results"].IsArray()) {
        for (auto& entry : doc["results"].GetArray()) {
            if (entry.HasMember("Result") && entry.HasMember("Opponent")) {
                MatchResult r;
                r.result = entry["Result"].GetString();
                r.opponent = entry["Opponent"].GetString();
                results.push_back(r);
            }
        }
    } else {
        AXLOG("Stats JSON format unexpected or empty.");
    }

    return results;
}
