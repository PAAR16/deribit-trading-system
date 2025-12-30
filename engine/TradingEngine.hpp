#pragma once
#include <memory>
#include "api/IDeribitApi.hpp"

class TradingEngine {
public:
    TradingEngine();
    void connect(bool testnet);
    void authenticate(const std::string&, const std::string&);
    void submitOrder(const Order&);

private:
    std::unique_ptr<IDeribitApi> api_;
};
