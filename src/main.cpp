#include "engine/TradingEngine.hpp"
#include "cli/TradingCLI.hpp"

int main() {
    TradingEngine engine;
    TradingCLI cli(engine);
    cli.start();
    return 0;
}
