#pragma once
#include <string>

struct AccountSummary {
    std::string currency;
    double balance;
    double available_funds;
    double equity;
};