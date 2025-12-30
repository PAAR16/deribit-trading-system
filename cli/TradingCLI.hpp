#pragma once
#include "engine/TradingEngine.hpp"
#include "Command.hpp"
#include <map>
#include <memory>
#include <string>

class TradingCLI {
public:
    TradingCLI(TradingEngine& engine);
    void start();

private:
    TradingEngine& engine_;
    std::map<std::string, std::unique_ptr<Command>> commands_;

    void registerCommands();
    void processCommand(const std::string& line);
    std::vector<std::string> splitCommand(const std::string& cmd);
    void displayHelp();
};
