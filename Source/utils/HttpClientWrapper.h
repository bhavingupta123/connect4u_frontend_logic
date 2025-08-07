class HttpClientWrapper {
public:
    static void fetchStats(const std::string& playerName, std::function<void(const std::string&)> callback);
};
