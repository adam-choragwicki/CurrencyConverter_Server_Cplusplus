#pragma once

#include "types/exchange_rate.h"

class CurrenciesExchangeRatesDatabankStub
{
public:
    static ExchangeRate getEurToUsdExchangeRate()
    {
        return ExchangeRate("1.0736503481885999");
    }
};
