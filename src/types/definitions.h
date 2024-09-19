#pragma once

#include "types/exchange_rate.h"
#include <map>

class CurrencyCode;
class CurrencyExchangeRatesJson;

typedef std::map<CurrencyCode, CurrencyExchangeRatesJson> CurrencyCodeToCurrencyExchangeRatesJsonMapping;
typedef std::map<CurrencyCode, std::map<CurrencyCode, ExchangeRate>> CurrenciesRatesCache;
