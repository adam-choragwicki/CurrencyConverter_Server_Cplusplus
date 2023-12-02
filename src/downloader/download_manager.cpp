#include "download_manager.h"
#include "downloader.h"
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

std::map<CurrencyCode, CurrencyExchangeRatesJson> DownloadManager::downloadCurrenciesExchangeRates(const std::set<CurrencyCode>& currenciesCodes)
{
    try
    {
        return Downloader::downloadMultiplexing(currenciesCodes);
    }
    catch(const CurlError& exception)
    {
        throw DownloadError(exception.what());
    }
}
