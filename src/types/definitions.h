#pragma once

#include <map>

class CurrencyCode;
class CurrencyExchangeRatesJson;
class ExchangeRate;

typedef std::map<CurrencyCode, CurrencyExchangeRatesJson> CurrencyCodeToCurrencyExchangeRatesJsonMapping;
typedef std::map<CurrencyCode, std::map<CurrencyCode, ExchangeRate>> CurrenciesRatesCache;
