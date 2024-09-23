#pragma once

#include <set>
#include <string>
#include <map>

class CurrencyCode;
class CurrenciesExchangeRateDatabank;
class ParseResult;

class CurrenciesExchangeRateDatabankInitializer
{
public:
    static bool initializeCurrenciesExchangeRateDatabank(CurrenciesExchangeRateDatabank& currenciesExchangeRateDatabank, const std::map<CurrencyCode, ParseResult>& currencyCodeToParseResultMapping);
    static void loadCurrenciesExchangeRatesCacheFromFiles(CurrenciesExchangeRateDatabank& currenciesExchangeRateDatabank, const std::string& directoryPath);
};
