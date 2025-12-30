#ifndef ORDER_HPP
#define ORDER_HPP

#include <string>

class Order {
public:
    enum class Type { BUY, SELL };
    enum class Status { PENDING, FILLED, CANCELLED };

    Order(Type type, const std::string& instrument, double price, double quantity)
        : type_(type), instrument_(instrument), price_(price), quantity_(quantity), status_(Status::PENDING) {}

    Type getType() const { return type_; }
    const std::string& getInstrument() const { return instrument_; }
    double getPrice() const { return price_; }
    double getQuantity() const { return quantity_; }
    Status getStatus() const { return status_; }
    void setStatus(Status status) { status_ = status; }

private:
    Type type_;
    std::string instrument_;
    double price_;
    double quantity_;
    Status status_;
};

#endif // ORDER_HPP