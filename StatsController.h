#pragma once
#include "axmol.h"
#include <string>

class StatsController {
public:
    static void showStats(ax::Node* parent, const std::string& playerName);
};
