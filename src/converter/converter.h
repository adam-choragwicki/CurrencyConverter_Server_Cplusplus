#pragma once

#include "types/currency_code.h"
#include "types/exchange_rate.h"
#include "types/money_amount.h"

class Converter
{
public:
    Converter() = delete;
    static std::string convert(const MoneyAmount& moneyAmount, const ExchangeRate& exchangeRate);

private:
    static std::string executeCalculation(const MoneyAmount& moneyAmount, const ExchangeRate& currencyExchangeRate);
};
