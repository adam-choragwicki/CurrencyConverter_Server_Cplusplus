#pragma once

#include <set>
#include <string>

class CurrencyCode;
class CurrenciesExchangeRateDatabank;

class CurrenciesExchangeRateDatabankInitializer
{
public:
    static void loadCurrenciesExchangeRatesCacheFromFiles(CurrenciesExchangeRateDatabank& currenciesExchangeRateDatabank, const std::string& directoryPath);
};
