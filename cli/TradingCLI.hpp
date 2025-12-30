#pragma once
#include "engine/TradingEngine.hpp"

class TradingCLI {
public:
    TradingCLI(TradingEngine&);
    void start();

private:
    TradingEngine& engine_;
};
