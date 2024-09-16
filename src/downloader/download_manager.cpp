#include "download_manager.h"
#include "types/currency_code.h"
#include "curl_manager.h"
#include "curl/curl.h"

DownloadManager::DownloadManager()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

DownloadManager::~DownloadManager()
{
    curl_global_cleanup();
}

DownloadManager& DownloadManager::getInstance()
{
    static DownloadManager downloadManager;
    return downloadManager;
}

CurrencyCodeToCurrencyExchangeRatesJsonMapping DownloadManager::downloadCurrenciesExchangeRates(const std::set<CurrencyCode>& currenciesCodes)
{
    try
    {
        return CurlManager::downloadMultiplexing(currenciesCodes);
    }
    catch(const CurlError& exception)
    {
        throw DownloadError(exception.what());
    }
}
