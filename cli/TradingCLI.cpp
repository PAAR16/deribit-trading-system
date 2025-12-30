#include "TradingCLI.hpp"
#include <iostream>

TradingCLI::TradingCLI(TradingEngine& engine)
    : engine_(engine) {}

void TradingCLI::start() {
    engine_.connect(true);
    engine_.authenticate("demo_id", "demo_secret");

    Order o{
        "BTC-PERPETUAL",
        1.0,
        0.0,
        OrderSide::BUY,
        OrderType::MARKET
    };
    engine_.submitOrder(o);
}
