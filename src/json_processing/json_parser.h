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
    static std::map<CurrencyCode, ExchangeRateData> parseExchangeRatesJsonStringToCurrencyCodesToExchangeRateDataMapping(const CurrencyCode& sourceCurrencyCode,
                                                                                                                         const std::set<CurrencyCode>& currenciesCodes,
                                                                                                                         const CurrencyExchangeRatesJson& currencyExchangeRatesJson,
                                                                                                                         bool allKeysExistenceRequired = false);

    static bool isValidJsonString(const std::string& string);
};
