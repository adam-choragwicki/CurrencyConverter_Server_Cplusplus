#pragma once

#include <set>
#include <string>
#include <map>

class CurrencyCode;
class CurrenciesExchangeRatesDatabank;
class ParseResult;

class CurrenciesExchangeRatesDatabankInitializer
{
public:
    static void initializeCurrenciesExchangeRatesDatabank(CurrenciesExchangeRatesDatabank& currenciesExchangeRatesDatabank, const std::map<CurrencyCode, ParseResult>& currencyCodeToParseResultMapping);
    static void loadCurrenciesExchangeRatesCacheFromFiles(CurrenciesExchangeRatesDatabank& currenciesExchangeRatesDatabank, const std::string& directoryPath);
};
