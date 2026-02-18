#pragma once

#include <functional>

class CurrenciesExchangeRatesDatabank;
class DownloadManager;
class DownloadReport;

class CurrenciesExchangeRatesDatabankUpdateManager
{
public:
    static bool startCurrenciesExchangeRatesDatabankUpdate(CurrenciesExchangeRatesDatabank& currenciesExchangeRatesDatabank,
                                                           DownloadManager& downloadManager,
                                                           const std::function<void(size_t completed, size_t total)>& onProgress);

private:
    static bool prepareDownloadDirectory();
    static void displayDownloadReportData(const DownloadReport& downloadReport);
};
