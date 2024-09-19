#pragma once

#include <set>
#include <map>
#include <string>

class CurrencyCode;
class ExchangeRateData;
class Timestamp;
class CurrencyExchangeRatesJson;

class JsonParser
{
public:
    JsonParser() = delete;

    static std::set<CurrencyCode> parseCurrenciesListFileContentToCurrenciesCodes(const std::string& string);
    static std::map<CurrencyCode, ExchangeRateData> parseExchangeRatesJsonStringToCurrencyCodesToExchangeRatesMapping(const CurrencyCode& sourceCurrencyCode,
                                                                                                                      const std::set<CurrencyCode>& currenciesCodes,
                                                                                                                      const CurrencyExchangeRatesJson& currencyExchangeRatesJson);

    static Timestamp parseTimestamp(const CurrencyCode& currencyCode, const CurrencyExchangeRatesJson& currencyExchangeRatesJson);

    static bool isValidJsonString(const std::string& string);
};
