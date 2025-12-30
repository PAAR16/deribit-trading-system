#ifndef DERIBIT_WS_CLIENT_HPP
#define DERIBIT_WS_CLIENT_HPP

#include "IDeribitApi.hpp"
#include <string>
#include <functional>
#include <thread>
#include <atomic>

class DeribitWsClient : public IDeribitApi {
public:
    DeribitWsClient();
    ~DeribitWsClient() override;

    void connect() override;
    void disconnect() override;
    void subscribe(const std::string& channel) override;
    void unsubscribe(const std::string& channel) override;
    void sendRequest(const std::string& request, std::function<void(const std::string&)> callback) override;

private:
    void run();
    void onMessage(const std::string& message);

    std::thread worker_;
    std::atomic<bool> running_;
    // Placeholder for websocket connection
};

#endif // DERIBIT_WS_CLIENT_HPP