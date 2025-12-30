#include "TradingEngine.hpp"
#include "api/DeribitWsClient.hpp"
#include "../infra/Logger.hpp"

TradingEngine::TradingEngine() {
    api_ = std::make_unique<DeribitWsClient>();
    api_->setResponseHandler([this](const std::string& msg) {
        if (response_callback_) response_callback_(msg);
    });
    api_->setSubscriptionHandler([this](const std::string& ch, const std::string& data) {
        if (subscription_callback_) subscription_callback_(ch, data);
    });
}

TradingEngine::~TradingEngine() {
    disconnect();
}

void TradingEngine::connect(bool testnet) {
    api_->connect(testnet);
}

void TradingEngine::disconnect() {
    api_->disconnect();
}

void TradingEngine::authenticate(const std::string& id, const std::string& secret) {
    api_->authenticate(id, secret);
}

void TradingEngine::getTime() { api_->getTime(); }
void TradingEngine::getInstruments(const std::string& currency, const std::string& kind) {
    api_->getInstruments(currency, kind);
}
void TradingEngine::getCurrencies() { api_->getCurrencies(); }
void TradingEngine::getOrderbook(const std::string& instrument, int depth) {
    api_->getOrderbook(instrument, depth);
}
void TradingEngine::getAccountSummary(const std::string& currency) {
    api_->getAccountSummary(currency);
}
void TradingEngine::getPositions(const std::string& currency) {
    api_->getPositions(currency);
}
void TradingEngine::submitOrder(const Order& order) { api_->placeOrder(order); }
void TradingEngine::cancelOrder(const std::string& order_id) { api_->cancelOrder(order_id); }
void TradingEngine::cancelAllOrders() { api_->cancelAllOrders(); }
void TradingEngine::modifyOrder(const std::string& order_id, double amount, double price) {
    api_->modifyOrder(order_id, amount, price);
}
void TradingEngine::getOpenOrders(const std::string& instrument) {
    api_->getOpenOrders(instrument);
}
void TradingEngine::getOrderHistory(const std::string& instrument) {
    api_->getOrderHistory(instrument);
}

void TradingEngine::subscribeToOrderbook(const std::string& instrument) {
    api_->subscribeToOrderbook(instrument);
}
void TradingEngine::subscribeToTrades(const std::string& instrument) {
    api_->subscribeToTrades(instrument);
}
void TradingEngine::subscribeToTicker(const std::string& instrument) {
    api_->subscribeToTicker(instrument);
}

void TradingEngine::setResponseCallback(std::function<void(const std::string&)> cb) {
    response_callback_ = cb;
}

void TradingEngine::setSubscriptionCallback(std::function<void(const std::string& channel, const std::string& data)> cb) {
    subscription_callback_ = cb;
}
