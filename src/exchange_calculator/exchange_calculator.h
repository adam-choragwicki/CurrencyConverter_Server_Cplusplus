#pragma once

#include <string>
#include "types/currency_amount.h"

class CurrencyAmount;
class ExchangeRate;
class ExchangeResult;

class ConversionError : public std::runtime_error
{
public:
    explicit ConversionError(const std::string& message) : std::runtime_error(message) {}
};

class ExchangeCalculator
{
public:
    ExchangeCalculator() = delete;
    static ExchangeResult convert(const CurrencyAmount& currencyAmount, const ExchangeRate& exchangeRate);

private:
    static std::string executeCalculation(const CurrencyAmount& currencyAmount, const ExchangeRate& exchangeRate);
};
