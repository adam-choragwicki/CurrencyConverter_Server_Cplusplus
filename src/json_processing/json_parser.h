#pragma once

#include <set>
#include <map>
#include <string>

class CurrencyCode;
class ExchangeRate;
class Timestamp;
class CurrencyExchangeRatesJson;

class JsonParser
{
public:
    JsonParser() = delete;

    static std::set<CurrencyCode> parseJsonToCurrenciesCodes(const std::string& string);
    static std::map<CurrencyCode, ExchangeRate> extractAllExchangeRatesFromCurrencyExchangeRatesJsonString(const CurrencyCode& sourceCurrencyCode,
                                                                                                           const std::set<CurrencyCode>& currenciesCodes,
                                                                                                           const CurrencyExchangeRatesJson& currencyExchangeRatesJson);

    static Timestamp parseTimestamp(const CurrencyCode& currencyCode, const CurrencyExchangeRatesJson& currencyExchangeRatesJson);
};
