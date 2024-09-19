#pragma once

#include "types/currency_code.h"
#include "currencies_exchange_rate_databank.h"
#include "types/currency_exchange_rates_json.h"
#include <map>

class DownloadManager;

class CurrenciesExchangeRateDatabankUpdater
{
public:
    static bool startCacheUpdate(CurrenciesExchangeRateDatabank& currenciesDatabank, DownloadManager& downloadManager);

private:
    static void updateCache(const CurrencyCodeToCurrencyExchangeRatesJsonMapping& currenciesCodesToExchangeRatesJsonsMapping, const Timestamp& exchangeRatesTimestamp, CurrenciesExchangeRateDatabank& currenciesDatabank);
};
