#include "TradingEngine.hpp"
#include "api/DeribitWsClient.hpp"

TradingEngine::TradingEngine() {
    api_ = std::make_unique<DeribitWsClient>();
}

void TradingEngine::connect(bool testnet) {
    api_->connect(testnet);
}

void TradingEngine::authenticate(const std::string& id,
                                 const std::string& secret) {
    api_->authenticate(id, secret);
}

void TradingEngine::submitOrder(const Order& order) {
    api_->placeOrder(order);
}
