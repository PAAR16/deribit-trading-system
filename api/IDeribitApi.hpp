#pragma once
#include "core/Order.hpp"

class IDeribitApi {
public:
    virtual void connect(bool testnet) = 0;
    virtual void authenticate(const std::string&, const std::string&) = 0;
    virtual void placeOrder(const Order&) = 0;
    virtual ~IDeribitApi() = default;
};
