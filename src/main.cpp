#include "engine/TradingEngine.hpp"
#include "cli/TradingCLI.hpp"
#include <iostream>
#include <string>

int main() {
    try {
        TradingEngine engine;
        TradingCLI cli(engine);

        // Get connection type from user
        std::string network_type;
        std::cout << "Connect to testnet? (y/n): ";
        std::getline(std::cin, network_type);
        bool use_testnet = (network_type == "y" || network_type == "Y");

        // Connect to appropriate network
        std::cout << "Connecting to Deribit " << (use_testnet ? "testnet" : "mainnet") << "...\n";
        engine.connect(use_testnet);

        // Get API credentials
        std::string client_id, client_secret;
        std::cout << "Enter client_id: ";
        std::getline(std::cin, client_id);
        std::cout << "Enter client_secret: ";
        std::getline(std::cin, client_secret);

        // Authenticate
        engine.authenticate(client_id, client_secret);

        // Start CLI
        std::cout << "\nStarting trading interface...\n";
        cli.start();
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
