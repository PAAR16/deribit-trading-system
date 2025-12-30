#pragma once

#include "IDeribitApi.hpp"

// ===== Deribit / WebSocket dependencies stay HERE =====
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <nlohmann/json.hpp>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <map>

class DeribitWsClient : public IDeribitApi {
public:
    DeribitWsClient();
    ~DeribitWsClient();

    void connect(bool use_testnet) override;
    void disconnect() override;
    void authenticate(const std::string& client_id,
                      const std::string& client_secret) override;

    // Public API
    void getTime() override;
    void getInstruments(const std::string& currency, const std::string& kind) override;
    void getCurrencies() override;
    void getOrderbook(const std::string& instrument_name, int depth = 5) override;
    void getTradingviewChartData(const std::string& instrument_name,
                                 const std::string& start_timestamp,
                                 const std::string& end_timestamp,
                                 const std::string& resolution) override;

    // Private API
    void getAccountSummary(const std::string& currency) override;
    void getPositions(const std::string& currency) override;
    void placeOrder(const Order& order) override;
    void cancelOrder(const std::string& order_id) override;
    void cancelAllOrders() override;
    void modifyOrder(const std::string& order_id, double amount, double price) override;
    void getOpenOrders(const std::string& instrument_name = "") override;
    void getOrderHistory(const std::string& instrument_name = "") override;

    // Subscriptions
    void subscribeToOrderbook(const std::string& instrument_name) override;
    void subscribeToTrades(const std::string& instrument_name) override;
    void subscribeToTicker(const std::string& instrument_name) override;
    void unsubscribe(const std::string& channel) override;

    // Handlers
    void setResponseHandler(std::function<void(const std::string&)> handler) override;
    void setSubscriptionHandler(std::function<void(const std::string& channel, const std::string& data)> handler) override;

private:
    // ===== Types =====
    using Client = websocketpp::client<websocketpp::config::asio_tls_client>;
    using json = nlohmann::json;

    // ===== WebSocket internals =====
    Client client_;
    Client::connection_ptr connection_;
    std::thread ws_thread_;

    // ===== State =====
    int request_id_;
    std::string access_token_;
    bool connected_;
    bool authenticated_;

    std::mutex mutex_;
    std::condition_variable cv_;

    // ===== Handlers =====
    std::function<void(const std::string&)> response_handler_;
    std::function<void(const std::string& channel, const std::string& data)> subscription_handler_;

    // ===== Subscriptions =====
    std::map<std::string, bool> active_subscriptions_;
    std::mutex subscription_mutex_;

    // ===== Internal helpers =====
    void setupClient();
    void sendRequest(const std::string& method, const json& params);
    void sendPrivateRequest(const std::string& method, const json& params);
    void handleMessage(const std::string& message);
    void waitForConnection();
    void waitForAuthentication();
};
