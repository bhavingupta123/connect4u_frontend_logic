#pragma once

#include <string>
#include <functional>
#include "network/HttpClient.h"

class HttpClientWrapper {
public:
    static void fetchStats(const std::string& playerName, const std::function<void(const std::string&)>& callback);
};
