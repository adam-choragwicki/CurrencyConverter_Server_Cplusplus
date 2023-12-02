#pragma once

#include "types/exchange_rate.h"

class CurrenciesDatabankStub
{
public:
    static ExchangeRate getEurToUsdExchangeRate()
    {
        return ExchangeRate("1.0736503481885999");
    }
};
