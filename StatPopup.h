
#pragma once
#include "axmol.h"
#include "MatchResult.h"
#include <vector>

class StatPopup : public ax::Layer {
public:
    static void createAndShow(ax::Node* parent, const std::vector<MatchResult>& stats);
};
