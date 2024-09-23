#pragma once

#include "types/definitions.h"
#include <set>

class CurrenciesExchangeRatesDatabank;
class ParseResult;

class CurrenciesExchangeRatesDatabankModifier
{
public:
    static void modifyCurrenciesExchangeRatesDatabank(CurrenciesExchangeRatesDatabank& currenciesExchangeRatesDatabank, const std::map<CurrencyCode, ParseResult>& currencyCodeToParseResultMapping);
};
