#include "download_manager.h"
#include "types/currency_code.h"
#include "curl_manager.h"
#include "curl/curl.h"
#include "spdlog/spdlog.h"
#include <filesystem>

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
        const std::string DOWNLOAD_DIRECTORY_PATH = "downloaded_data";

        if(std::filesystem::exists(DOWNLOAD_DIRECTORY_PATH))
        {
            std::filesystem::remove_all(DOWNLOAD_DIRECTORY_PATH);
        }

        std::filesystem::create_directory(DOWNLOAD_DIRECTORY_PATH);

        CurlManager curlManager(DOWNLOAD_DIRECTORY_PATH);

        return curlManager.downloadMultiplexing(currenciesCodes);
    }
    catch(const CurlError& exception)
    {
        throw DownloadError(exception.what());
    }
}
