#include "download_manager.h"
#include "types/currency_code.h"
#include "curl_manager.h"
#include "curl/curl.h"
#include "spdlog/spdlog.h"

DownloadManager::DownloadManager()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);

    if(!alreadyCreated_)
    {
        alreadyCreated_ = true;
    }
    else
    {
        throw std::runtime_error("Error, trying to construct another instance of DownloadManager");
    }

    spdlog::debug("DownloadManager initialized, CURL initialized");
}

DownloadManager::~DownloadManager()
{
    curl_global_cleanup();

    spdlog::debug("Destroying DownloadManager, CURL cleaned up");
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
