#include "TradingCLI.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include "../core/Order.hpp"
#include "../infra/Logger.hpp"

// Command implementations
class HelpCommand : public Command {
public:
    HelpCommand(TradingCLI& cli) : cli_(cli) {}
    void execute(const std::vector<std::string>& args) override {
        cli_.displayHelp();
    }
    std::string help() const override { return "Show this help"; }
private:
    TradingCLI& cli_;
};

class QuitCommand : public Command {
public:
    void execute(const std::vector<std::string>& args) override {
        std::cout << "Exiting...\n";
        exit(0);
    }
    std::string help() const override { return "Exit the program"; }
};

class BuyCommand : public Command {
public:
    BuyCommand(TradingEngine& engine) : engine_(engine) {}
    void execute(const std::vector<std::string>& args) override {
        if (args.size() < 3) {
            std::cout << "Usage: buy <instrument> <amount> [price|market]\n";
            return;
        }
        std::string instrument = args[0];
        double amount = std::stod(args[1]);
        Order order{instrument, amount, 0.0, OrderSide::BUY, OrderType::LIMIT};
        if (args.size() >= 3 && args[2] == "market") {
            order.type = OrderType::MARKET;
        } else if (args.size() >= 3) {
            order.price = std::stod(args[2]);
        }
        engine_.submitOrder(order);
    }
    std::string help() const override { return "Place buy order: buy <instrument> <amount> [price|market]"; }
private:
    TradingEngine& engine_;
};

class SellCommand : public Command {
public:
    SellCommand(TradingEngine& engine) : engine_(engine) {}
    void execute(const std::vector<std::string>& args) override {
        if (args.size() < 3) {
            std::cout << "Usage: sell <instrument> <amount> [price|market]\n";
            return;
        }
        std::string instrument = args[0];
        double amount = std::stod(args[1]);
        Order order{instrument, amount, 0.0, OrderSide::SELL, OrderType::LIMIT};
        if (args.size() >= 3 && args[2] == "market") {
            order.type = OrderType::MARKET;
        } else if (args.size() >= 3) {
            order.price = std::stod(args[2]);
        }
        engine_.submitOrder(order);
    }
    std::string help() const override { return "Place sell order: sell <instrument> <amount> [price|market]"; }
private:
    TradingEngine& engine_;
};

class BookCommand : public Command {
public:
    BookCommand(TradingEngine& engine) : engine_(engine) {}
    void execute(const std::vector<std::string>& args) override {
        if (args.empty()) {
            std::cout << "Usage: book <instrument>\n";
            return;
        }
        engine_.getOrderbook(args[0]);
    }
    std::string help() const override { return "Get orderbook: book <instrument>"; }
private:
    TradingEngine& engine_;
};

class SubscribeCommand : public Command {
public:
    SubscribeCommand(TradingEngine& engine) : engine_(engine) {}
    void execute(const std::vector<std::string>& args) override {
        if (args.size() < 2) {
            std::cout << "Usage: sub <type> <instrument> (type: book, trades, ticker)\n";
            return;
        }
        std::string type = args[0];
        std::string instrument = args[1];
        if (type == "book") engine_.subscribeToOrderbook(instrument);
        else if (type == "trades") engine_.subscribeToTrades(instrument);
        else if (type == "ticker") engine_.subscribeToTicker(instrument);
        else std::cout << "Invalid subscription type\n";
    }
    std::string help() const override { return "Subscribe: sub <type> <instrument>"; }
private:
    TradingEngine& engine_;
};

TradingCLI::TradingCLI(TradingEngine& engine)
    : engine_(engine) {
    registerCommands();
    engine_.setResponseCallback([](const std::string& msg) {
        std::cout << "Response: " << msg << std::endl;
    });
    engine_.setSubscriptionCallback([](const std::string& ch, const std::string& data) {
        std::cout << "Subscription [" << ch << "]: " << data << std::endl;
    });
}

void TradingCLI::registerCommands() {
    commands_["help"] = std::make_unique<HelpCommand>(*this);
    commands_["quit"] = std::make_unique<QuitCommand>();
    commands_["buy"] = std::make_unique<BuyCommand>(engine_);
    commands_["sell"] = std::make_unique<SellCommand>(engine_);
    commands_["book"] = std::make_unique<BookCommand>(engine_);
    commands_["sub"] = std::make_unique<SubscribeCommand>(engine_);
}

void TradingCLI::start() {
    displayHelp();
    std::string line;
    while (true) {
        std::cout << "\nDeribit> ";
        std::getline(std::cin, line);
        if (!line.empty()) {
            processCommand(line);
        }
    }
}

void TradingCLI::processCommand(const std::string& line) {
    auto tokens = splitCommand(line);
    if (tokens.empty()) return;
    std::string cmd = tokens[0];
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
    std::vector<std::string> args(tokens.begin() + 1, tokens.end());

    if (commands_.count(cmd)) {
        try {
            commands_[cmd]->execute(args);
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    } else {
        std::cout << "Unknown command. Type 'help' for available commands.\n";
    }
}

std::vector<std::string> TradingCLI::splitCommand(const std::string& cmd) {
    std::vector<std::string> tokens;
    std::stringstream ss(cmd);
    std::string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

void TradingCLI::displayHelp() {
    std::cout << "\n=== Deribit Trading Commands ===\n";
    for (const auto& pair : commands_) {
        std::cout << "  " << pair.first << " - " << pair.second->help() << "\n";
    }
    std::cout << "=================================\n";
}
