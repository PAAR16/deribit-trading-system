#pragma once
#include <memory>
#include <functional>
#include "api/IDeribitApi.hpp"
#include "core/Order.hpp"

class TradingEngine {
public:
    TradingEngine();
    ~TradingEngine();

    void connect(bool testnet);
    void disconnect();
    void authenticate(const std::string& id, const std::string& secret);

    // API wrappers
    void getTime();
    void getInstruments(const std::string& currency, const std::string& kind);
    void getCurrencies();
    void getOrderbook(const std::string& instrument, int depth = 5);
    void getAccountSummary(const std::string& currency);
    void getPositions(const std::string& currency);
    void submitOrder(const Order& order);
    void cancelOrder(const std::string& order_id);
    void cancelAllOrders();
    void modifyOrder(const std::string& order_id, double amount, double price);
    void getOpenOrders(const std::string& instrument = "");
    void getOrderHistory(const std::string& instrument = "");

    // Subscriptions
    void subscribeToOrderbook(const std::string& instrument);
    void subscribeToTrades(const std::string& instrument);
    void subscribeToTicker(const std::string& instrument);

    // Handlers
    void setResponseCallback(std::function<void(const std::string&)> cb);
    void setSubscriptionCallback(std::function<void(const std::string& channel, const std::string& data)> cb);

private:
    std::unique_ptr<IDeribitApi> api_;
    std::function<void(const std::string&)> response_callback_;
    std::function<void(const std::string& channel, const std::string& data)> subscription_callback_;
};
