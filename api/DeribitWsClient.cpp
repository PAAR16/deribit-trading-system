#include "DeribitWsClient.hpp"
#include <iostream>
#include <chrono>

DeribitWsClient::DeribitWsClient() : running_(false) {}

DeribitWsClient::~DeribitWsClient() {
    disconnect();
}

void DeribitWsClient::connect() {
    if (running_) return;
    running_ = true;
    worker_ = std::thread(&DeribitWsClient::run, this);
    std::cout << "Connected to Deribit WebSocket" << std::endl;
}

void DeribitWsClient::disconnect() {
    if (!running_) return;
    running_ = false;
    if (worker_.joinable()) {
        worker_.join();
    }
    std::cout << "Disconnected from Deribit WebSocket" << std::endl;
}

void DeribitWsClient::subscribe(const std::string& channel) {
    std::cout << "Subscribed to channel: " << channel << std::endl;
}

void DeribitWsClient::unsubscribe(const std::string& channel) {
    std::cout << "Unsubscribed from channel: " << channel << std::endl;
}

void DeribitWsClient::sendRequest(const std::string& request, std::function<void(const std::string&)> callback) {
    std::cout << "Sending request: " << request << std::endl;
    // Mock response
    callback("{\"result\": \"mock response\"}");
}

void DeribitWsClient::run() {
    while (running_) {
        // Simulate receiving messages
        std::this_thread::sleep_for(std::chrono::seconds(1));
        onMessage("Mock message");
    }
}

void DeribitWsClient::onMessage(const std::string& message) {
    std::cout << "Received: " << message << std::endl;
}