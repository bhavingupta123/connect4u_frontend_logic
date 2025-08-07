#pragma once
#include <vector>
#include <string>
#include "MatchResult.h"

class StatModel {
public:
    static std::vector<MatchResult> parseFromJson(const std::string& json);
};
