#include "DeribitWsClient.hpp"
#include <iostream>

void DeribitWsClient::connect(bool testnet) {
    std::cout << "[WS] Connecting to "
              << (testnet ? "testnet" : "mainnet") << std::endl;
}

void DeribitWsClient::authenticate(const std::string&, const std::string&) {
    std::cout << "[WS] Authenticating\n";
}

void DeribitWsClient::placeOrder(const Order&) {
    std::cout << "[WS] Placing order\n";
}
