#ifndef TRADING_ENGINE_HPP
#define TRADING_ENGINE_HPP

#include "../api/IDeribitApi.hpp"
#include "../core/Order.hpp"
#include <memory>
#include <vector>

class TradingEngine {
public:
    TradingEngine(std::unique_ptr<IDeribitApi> api);

    void start();
    void stop();
    void placeOrder(const Order& order);
    void cancelOrder(int orderId);

private:
    std::unique_ptr<IDeribitApi> api_;
    std::vector<Order> orders_;
};

#endif // TRADING_ENGINE_HPP