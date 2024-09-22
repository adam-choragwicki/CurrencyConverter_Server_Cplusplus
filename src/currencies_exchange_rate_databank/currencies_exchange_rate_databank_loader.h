#pragma once

#include <set>
#include <string>

class CurrencyCode;
class CurrenciesExchangeRateDatabank;

class CurrenciesExchangeRateDatabankLoader
{
public:
    static void loadCurrenciesExchangeRatesCacheFromFiles(CurrenciesExchangeRateDatabank& currenciesExchangeRateDatabank, const std::set<CurrencyCode>& currenciesCodes, const std::string& directoryPath);
};
