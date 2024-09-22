#pragma once

#include "types/definitions.h"
#include <set>
#include <map>
#include <string>
#include <optional>

class CurrencyCode;
class ExchangeRateData;
class Timestamp;
class CurrencyExchangeRatesJson;

struct ParseResult
{
    bool isSuccess_{};
    std::optional<CurrencyCodeToCurrencyExchangeRateDataMapping> currencyCodeToCurrencyExchangeRateDataMapping_;
};

class JsonParser
{
public:
    JsonParser() = delete;

    static std::set<CurrencyCode> parseCurrenciesListFileContentToCurrenciesCodes(const std::string& string);
    static ParseResult parseExchangeRatesJsonStringToCurrencyCodesToExchangeRateDataMapping(const CurrencyCode& sourceCurrencyCode,
                                                                                            const std::set<CurrencyCode>& currenciesCodes,
                                                                                            const CurrencyExchangeRatesJson& currencyExchangeRatesJson,
                                                                                            bool allKeysExistenceRequired = false);
};
