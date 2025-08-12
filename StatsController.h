#pragma once
#include "axmol.h"
#include <string>
#include "StatModel.h"

class StatsController {
public:
    static void showStats(ax::Node* parent, const std::string& playerName);

};
