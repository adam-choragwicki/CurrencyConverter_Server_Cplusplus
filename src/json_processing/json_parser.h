#pragma once

#include "types/definitions.h"
#include "config_data.h"
#include "connection_data.h"
#include <set>
#include <map>
#include <string>
#include <optional>

class CurrencyCode;
class ExchangeRateData;
class Timestamp;
class CurrenciesNamesAndCodesFileContent;
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

    static ConfigData parseConfigData(const std::string& configFileContent);
    static ConnectionData parseConnectionData(const std::string& configFileContent);
    static std::set<CurrencyCode> parseCurrenciesNamesAndCodesFileToCurrenciesCodes(const CurrenciesNamesAndCodesFileContent& currenciesNamesAndCodesFileContent);
    static ParseResult parseExchangeRatesJsonStringToCurrencyCodesToExchangeRateDataMapping(const CurrencyCode& sourceCurrencyCode,
                                                                                            const std::set<CurrencyCode>& currenciesCodes,
                                                                                            const CurrencyExchangeRatesJson& currencyExchangeRatesJson,
                                                                                            bool allKeysExistenceRequired = false);

    static std::map<std::string, std::string> parseCurrenciesNamesAndCodesFileToMap(const CurrenciesNamesAndCodesFileContent& currenciesNamesAndCodesFileContent);
};
