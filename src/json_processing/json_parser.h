#pragma once

#include "types/currency_code.h"
#include "types/exchange_rate.h"
#include "types/timestamp.h"
#include "types/currency_exchange_rates_json.h"
#include <set>
#include <map>

class JsonParser
{
public:
    JsonParser() = delete;

    static std::set<CurrencyCode> parseJsonToCurrenciesCodes(const std::string& string);
    static std::map<CurrencyCode, ExchangeRate> extractAllExchangeRatesFromCurrencyExchangeRatesJsonString(const CurrencyCode& sourceCurrencyCode, const std::set<CurrencyCode>& currenciesCodes, const CurrencyExchangeRatesJson& currencyExchangeRatesJson);
    static Timestamp parseTimestamp(const CurrencyCode& currencyCode, const CurrencyExchangeRatesJson& currencyExchangeRatesJson);
};
