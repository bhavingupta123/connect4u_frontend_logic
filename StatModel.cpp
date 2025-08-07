#include "StatModel.h"
#include "rapidjson/document.h"
#include "MatchResult.h"

using namespace rapidjson;

std::vector<MatchResult> StatModel::parseFromJson(const std::string& json) {
    std::vector<MatchResult> results;
    Document doc;
    doc.Parse(json.c_str());

    if (!doc.IsArray()) return results;

    for (const auto& val : doc.GetArray()) {
        MatchResult r;
        if (val.HasMember("player_name") && val["player_name"].IsString())
            r.player = val["player_name"].GetString();
        if (val.HasMember("opponent_name") && val["opponent_name"].IsString())
            r.opponent = val["opponent_name"].GetString();
        if (val.HasMember("result") && val["result"].IsString())
            r.result = val["result"].GetString();

        results.push_back(r);
    }

    return results;
}
