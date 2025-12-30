#ifndef TRADING_CLI_HPP
#define TRADING_CLI_HPP

#include "../engine/TradingEngine.hpp"
#include <memory>

class TradingCLI {
public:
    TradingCLI(std::unique_ptr<TradingEngine> engine);

    void run();

private:
    void processCommand(const std::string& command);

    std::unique_ptr<TradingEngine> engine_;
};

#endif // TRADING_CLI_HPP