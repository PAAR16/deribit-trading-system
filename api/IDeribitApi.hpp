#pragma once
#include <string>
#include <functional>
#include "core/Order.hpp"

class IDeribitApi {
public:
    virtual void connect(bool use_testnet) = 0;
    virtual void disconnect() = 0;
    virtual void authenticate(const std::string& client_id,
                              const std::string& client_secret) = 0;

    // Public API
    virtual void getTime() = 0;
    virtual void getInstruments(const std::string& currency, const std::string& kind) = 0;
    virtual void getCurrencies() = 0;
    virtual void getOrderbook(const std::string& instrument_name, int depth = 5) = 0;
    virtual void getTradingviewChartData(const std::string& instrument_name,
                                         const std::string& start_timestamp,
                                         const std::string& end_timestamp,
                                         const std::string& resolution) = 0;

    // Private API
    virtual void getAccountSummary(const std::string& currency) = 0;
    virtual void getPositions(const std::string& currency) = 0;
    virtual void placeOrder(const Order& order) = 0;
    virtual void cancelOrder(const std::string& order_id) = 0;
    virtual void cancelAllOrders() = 0;
    virtual void modifyOrder(const std::string& order_id, double amount, double price) = 0;
    virtual void getOpenOrders(const std::string& instrument_name = "") = 0;
    virtual void getOrderHistory(const std::string& instrument_name = "") = 0;

    // Subscriptions
    virtual void subscribeToOrderbook(const std::string& instrument_name) = 0;
    virtual void subscribeToTrades(const std::string& instrument_name) = 0;
    virtual void subscribeToTicker(const std::string& instrument_name) = 0;
    virtual void unsubscribe(const std::string& channel) = 0;

    // Response handlers
    virtual void setResponseHandler(std::function<void(const std::string&)> handler) = 0;
    virtual void setSubscriptionHandler(std::function<void(const std::string& channel, const std::string& data)> handler) = 0;

    virtual ~IDeribitApi() = default;
};
