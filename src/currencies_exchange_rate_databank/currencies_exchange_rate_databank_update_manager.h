#pragma once

#include "types/definitions.h"
#include <set>

class CurrenciesExchangeRateDatabank;
class DownloadManager;
class DownloadReport;
class ParseResult;

class CurrenciesExchangeRateDatabankUpdateManager
{
public:
    static bool startCurrenciesExchangeRateDatabankUpdate(CurrenciesExchangeRateDatabank& currenciesExchangeRateDatabank, DownloadManager& downloadManager);

private:
    static void prepareDownloadDirectory();
    static void displayDownloadReportData(const DownloadReport& downloadReport);
};
