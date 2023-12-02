#pragma once

#include "types/definitions.h"
#include <set>

class CurrenciesExchangeRatesDatabank;
class DownloadManager;
class DownloadReport;
class ParseResult;

class CurrenciesExchangeRatesDatabankUpdateManager
{
public:
    static bool startCurrenciesExchangeRatesDatabankUpdate(CurrenciesExchangeRatesDatabank& currenciesExchangeRatesDatabank, DownloadManager& downloadManager);

private:
    static void prepareDownloadDirectory();
    static void displayDownloadReportData(const DownloadReport& downloadReport);
};
