#include "currencies_exchange_rate_databank_updater.h"
#include "currencies_exchange_rate_databank.h"
#include "utilities/timer.h"
#include "downloader/download_manager.h"
#include "json_processing/json_parser.h"
#include "spdlog/spdlog.h"
#include "types/currency_code.h"
#include "types/definitions.h"
#include "types/currency_exchange_rates_json.h"
#include "downloader/download_report.h"

bool CurrenciesExchangeRateDatabankUpdater::startCacheUpdate(CurrenciesExchangeRateDatabank& currenciesDatabank, DownloadManager& downloadManager)
{
    spdlog::info("Starting currencies exchange rates update");

    //download
    //validate
    //parse

    Timer timer;

    CurrencyCodeToCurrencyExchangeRatesJsonMapping currenciesCodesToExchangeRatesJsonsMapping;


    //validate

    //    for(const auto&[currencyCode, currencyRateExchangeJson] : currenciesCodesToExchangeRatesJsonsMapping)
    //    {
    //        if(JsonReader::isValidJson(currencyRateExchangeJson.toString()))
    //        {
    //            currenciesDatabank.setExchangeRate(currencyCode, currencyRateExchangeJson);
    //        }
    //    }

    //    CurrencyCodeToCurrencyExchangeRatesJsonMappingValidator currenciesCodesToExchangeRatesJsonsMappingValidator;

    std::unique_ptr<DownloadReport> downloadReport;

    try
    {
        downloadReport = std::make_unique<DownloadReport>(downloadManager.downloadCurrenciesExchangeRatesFiles(currenciesDatabank.getCurrenciesCodes()));
    }
    catch(const DownloadError& exception)
    {
        spdlog::error(exception.what() + std::string(".\nCache update aborted"));
        return false;
    }

    spdlog::info("Downloaded all exchange rates");

    const std::string downloadDirectoryPath = downloadReport->getDownloadDirectoryPath();

    std::set<CurrencyCode> currenciesCodesOfFilesRequestedToBeDownloaded = downloadReport->getCurrenciesCodesOfFilesRequestedToBeDownloaded();
    std::set<CurrencyCode> currenciesCodesOfSuccessfullyDownloadedFiles_ = downloadReport->getCurrencyCodesOfSuccessfullyDownloadedFiles();
    std::multimap<CurrencyCode, std::string> errorDescriptionsPerCurrencyCode_ = downloadReport->getErrorDescriptionsPerCurrencyCode();

    if(currenciesCodesOfSuccessfullyDownloadedFiles_.empty())
    {
        spdlog::error("Error, no successfully downloaded currencies exchange rates files\nCache update aborted");
        return false;
    }

    spdlog::info("Files requested to download: {}", currenciesCodesOfFilesRequestedToBeDownloaded.size());
    spdlog::info("Files download successfully: {}", currenciesCodesOfSuccessfullyDownloadedFiles_.size());

    size_t filesFailedToDownloadCount = errorDescriptionsPerCurrencyCode_.size();

    if(filesFailedToDownloadCount > 0)
    {
        spdlog::error("Files failed to download: {}", errorDescriptionsPerCurrencyCode_.size());
    }

    currenciesDatabank.updateCurrenciesExchangeRatesCacheFromFiles(currenciesCodesOfSuccessfullyDownloadedFiles_, downloadDirectoryPath);

    spdlog::info("Cache updated successfully in " + timer.getResult());

    return true;
}

void CurrenciesExchangeRateDatabankUpdater::updateCache(const CurrencyCodeToCurrencyExchangeRatesJsonMapping& currenciesCodesToExchangeRatesJsonsMapping, const Timestamp& exchangeRatesTimestamp, CurrenciesExchangeRateDatabank& currenciesDatabank)
{
}
