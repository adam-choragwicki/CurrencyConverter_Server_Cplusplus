#pragma once

#include <string>

class MoneyAmount;
class ExchangeRate;

class Converter
{
public:
    Converter() = delete;
    static std::string convert(const MoneyAmount& moneyAmount, const ExchangeRate& exchangeRate);

private:
    static std::string executeCalculation(const MoneyAmount& moneyAmount, const ExchangeRate& currencyExchangeRate);
};
