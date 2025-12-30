#pragma once
#include "IDeribitApi.hpp"

class DeribitWsClient : public IDeribitApi {
public:
    void connect(bool testnet) override;
    void authenticate(const std::string&, const std::string&) override;
    void placeOrder(const Order&) override;
};
