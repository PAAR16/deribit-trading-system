#ifndef INSTRUMENT_HPP
#define INSTRUMENT_HPP

#include <string>

class Instrument {
public:
    Instrument(const std::string& symbol, const std::string& baseCurrency, const std::string& quoteCurrency)
        : symbol_(symbol), baseCurrency_(baseCurrency), quoteCurrency_(quoteCurrency) {}

    const std::string& getSymbol() const { return symbol_; }
    const std::string& getBaseCurrency() const { return baseCurrency_; }
    const std::string& getQuoteCurrency() const { return quoteCurrency_; }

private:
    std::string symbol_;
    std::string baseCurrency_;
    std::string quoteCurrency_;
};

#endif // INSTRUMENT_HPP