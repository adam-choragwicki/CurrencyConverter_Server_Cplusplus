#pragma once


#include "types/containers/exchange_rate_data.h"
#include <map>

class CurrencyCode;
class CurrencyExchangeRatesJson;

typedef std::map<CurrencyCode, CurrencyExchangeRatesJson> CurrencyCodeToCurrencyExchangeRatesJsonMapping;
typedef std::map<CurrencyCode, std::map<CurrencyCode, ExchangeRateData>> CurrenciesRatesCache;
