#pragma once

#include "types/definitions.h"

class CurrenciesExchangeRateDatabank;
class DownloadManager;

class CurrenciesExchangeRateDatabankUpdater
{
public:
    static bool startCacheUpdate(CurrenciesExchangeRateDatabank& currenciesDatabank, DownloadManager& downloadManager);
};
