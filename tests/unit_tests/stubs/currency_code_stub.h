#pragma once

#include "types/currency_code.h"

class CurrencyCodeStub
{
public:
    static CurrencyCode getValueForEuro()
    {
        return CurrencyCode("EUR");
    }

    static CurrencyCode getValueForUsd()
    {
        return CurrencyCode("USD");
    }
};
