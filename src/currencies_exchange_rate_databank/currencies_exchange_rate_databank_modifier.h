#pragma once

#include "types/definitions.h"
#include "json_processing/json_parser.h"
#include <set>

class CurrenciesExchangeRatesDatabank;
struct ParseResult;

class CurrenciesExchangeRatesDatabankModifier
{
public:
    static void modifyCurrenciesExchangeRatesDatabank(CurrenciesExchangeRatesDatabank& currenciesExchangeRatesDatabank, const std::map<CurrencyCode, ParseResult>& currencyCodeToParseResultMapping);
};
