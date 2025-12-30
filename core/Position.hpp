#pragma once
#include <string>

struct Position {
    std::string instrument_name;
    double size;
    double average_price;
    double mark_price;
    double unrealized_pnl;
};