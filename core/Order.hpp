#pragma once
#include <string>

enum class OrderSide { BUY, SELL };
enum class OrderType { MARKET, LIMIT };

struct Order {
    std::string instrument;
    double amount;
    double price;
    OrderSide side;
    OrderType type;
};
