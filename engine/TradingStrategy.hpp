#pragma once
#include "Order.hpp"
#include <vector>

class TradingStrategy {
public:
    virtual std::vector<Order> generateOrders() = 0;
    virtual void onMarketData(const std::string& data) = 0;
    virtual ~TradingStrategy() = default;
};