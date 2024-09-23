#pragma once

#include "types/definitions.h"
#include <set>

class CurrenciesExchangeRateDatabank;
class ParseResult;

class CurrenciesExchangeRateDatabankModifier
{
public:
    static void modifyCurrenciesExchangeRateDatabank(CurrenciesExchangeRateDatabank& currenciesExchangeRateDatabank, const std::map<CurrencyCode, ParseResult>& currencyCodeToParseResultMapping);
};
