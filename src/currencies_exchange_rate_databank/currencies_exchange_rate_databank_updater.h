#pragma once

#include "types/definitions.h"

class CurrenciesExchangeRateDatabank;
class DownloadManager;
class DownloadReport;

class CurrenciesExchangeRateDatabankUpdater
{
public:
    static bool startCacheUpdate(CurrenciesExchangeRateDatabank& currenciesDatabank, DownloadManager& downloadManager);

private:
    static void displayDownloadReportData(const DownloadReport& downloadReport);
};
