#include "TradingEngine.hpp"
#include <iostream>

TradingEngine::TradingEngine(std::unique_ptr<IDeribitApi> api) : api_(std::move(api)) {}

void TradingEngine::start() {
    api_->connect();
    std::cout << "Trading engine started" << std::endl;
}

void TradingEngine::stop() {
    api_->disconnect();
    std::cout << "Trading engine stopped" << std::endl;
}

void TradingEngine::placeOrder(const Order& order) {
    orders_.push_back(order);
    std::string request = "{\"method\": \"public/buy\", \"params\": {\"instrument_name\": \"" + order.getInstrument() + "\", \"amount\": " + std::to_string(order.getQuantity()) + ", \"price\": " + std::to_string(order.getPrice()) + "}}";
    api_->sendRequest(request, [](const std::string& response) {
        std::cout << "Order response: " << response << std::endl;
    });
}

void TradingEngine::cancelOrder(int orderId) {
    // Implement cancel logic
    std::cout << "Cancelling order " << orderId << std::endl;
}