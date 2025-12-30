#include "TradingCLI.hpp"
#include "../core/Order.hpp"
#include <iostream>
#include <sstream>

TradingCLI::TradingCLI(std::unique_ptr<TradingEngine> engine) : engine_(std::move(engine)) {}

void TradingCLI::run() {
    std::string command;
    std::cout << "Trading CLI started. Type 'help' for commands." << std::endl;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);
        if (command == "exit") break;
        processCommand(command);
    }
}

void TradingCLI::processCommand(const std::string& command) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (cmd == "buy") {
        std::string instrument;
        double price, quantity;
        iss >> instrument >> price >> quantity;
        Order order(Order::Type::BUY, instrument, price, quantity);
        engine_->placeOrder(order);
    } else if (cmd == "sell") {
        std::string instrument;
        double price, quantity;
        iss >> instrument >> price >> quantity;
        Order order(Order::Type::SELL, instrument, price, quantity);
        engine_->placeOrder(order);
    } else if (cmd == "help") {
        std::cout << "Commands: buy <instrument> <price> <quantity>, sell <instrument> <price> <quantity>, exit" << std::endl;
    } else {
        std::cout << "Unknown command" << std::endl;
    }
}