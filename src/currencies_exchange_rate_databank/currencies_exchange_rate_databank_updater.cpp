#include "currencies_exchange_rate_databank_updater.h"
#include "currencies_exchange_rate_databank.h"
#include "utilities/timer.h"
#include "downloader/download_manager.h"
#include "spdlog/spdlog.h"
#include "types/currency_code.h"
#include "downloader/download_report.h"
#include "utilities/files_helper.h"
#include "json_processing/json_parser.h"
#include "json_processing/json_validator.h"
#include "config/config.h"
#include "utilities.h"

//TODO add common update failure reason exception?

bool CurrenciesExchangeRateDatabankUpdater::startCacheUpdate(CurrenciesExchangeRateDatabank& currenciesExchangeRateDatabank, DownloadManager& downloadManager)
{
    spdlog::info("Starting currencies exchange rates update");

    Timer timer;

    prepareDownloadDirectory();

    //download
    std::unique_ptr<DownloadReport> downloadReport;

    try
    {
        downloadReport = std::make_unique<DownloadReport>(downloadManager.downloadCurrenciesExchangeRatesFiles(Paths::CurrenciesDatabankConfig::DOWNLOAD_DIRECTORY_PATH, currenciesExchangeRateDatabank.getCurrenciesCodes()));
    }
    catch(const DownloadError& exception)
    {
        spdlog::error(exception.what() + std::string(".\nCache update aborted"));
        return false;
    }
    //end download

    displayDownloadReportData(*downloadReport);

    const size_t successfullyDownloadedFilesCount = downloadReport->getCurrencyCodesOfSuccessfullyDownloadedFiles().size();

    if(successfullyDownloadedFilesCount == 0)
    {
        spdlog::error("Error, no successfully downloaded currencies exchange rates files\nCache update aborted");
        return false;
    }

    auto updateDatabank = [](CurrenciesExchangeRateDatabank& currenciesExchangeRateDatabank, const std::map<CurrencyCode, ParseResult>& currencyCodeToParseResultMapping)
    {
        for(const auto&[currencyCode, parseResult] : currencyCodeToParseResultMapping)
        {
            if(parseResult.isSuccess_)
            {
                currenciesExchangeRateDatabank.updateAllExchangeRatesDataForCurrency(currencyCode, *parseResult.currencyCodeToCurrencyExchangeRateDataMapping_);
            }
        }
    };

    const std::set<CurrencyCode>& allCurrenciesCodes = currenciesExchangeRateDatabank.getCurrenciesCodes();

    std::map<CurrencyCode, std::string> currencyCodeToFilePathMapping = Utilities::getCurrencyCodeToFilePathMapping(Paths::CurrenciesDatabankConfig::DOWNLOAD_DIRECTORY_PATH, currenciesExchangeRateDatabank.getCurrenciesCodes());
    std::map<CurrencyCode, ParseResult> currencyCodeToParseResultMapping = Utilities::parseFiles(allCurrenciesCodes, currencyCodeToFilePathMapping);
    updateDatabank(currenciesExchangeRateDatabank, currencyCodeToParseResultMapping);

    spdlog::info("Cache updated successfully in " + timer.getResult());

    return true;
}

void CurrenciesExchangeRateDatabankUpdater::prepareDownloadDirectory()
{
    //TODO add error handling

    if(std::filesystem::exists(Paths::CurrenciesDatabankConfig::DOWNLOAD_DIRECTORY_PATH))
    {
        std::filesystem::remove_all(Paths::CurrenciesDatabankConfig::DOWNLOAD_DIRECTORY_PATH);
    }

    std::filesystem::create_directory(Paths::CurrenciesDatabankConfig::DOWNLOAD_DIRECTORY_PATH);
}

void CurrenciesExchangeRateDatabankUpdater::displayDownloadReportData(const DownloadReport& downloadReport)
{
    std::set<CurrencyCode> currenciesCodesOfFilesRequestedToBeDownloaded = downloadReport.getCurrenciesCodesOfFilesRequestedToBeDownloaded();
    std::set<CurrencyCode> currenciesCodesOfSuccessfullyDownloadedFiles_ = downloadReport.getCurrencyCodesOfSuccessfullyDownloadedFiles();
    std::multimap<CurrencyCode, std::string> errorDescriptionsPerCurrencyCode_ = downloadReport.getErrorDescriptionsPerCurrencyCode();

    if(currenciesCodesOfSuccessfullyDownloadedFiles_.empty())
    {
        spdlog::error("Error, no successfully downloaded currencies exchange rates files");
    }

    size_t filesRequestedToBeDownloadedCount = currenciesCodesOfFilesRequestedToBeDownloaded.size();
    size_t filesDownloadedSuccessfullyCount = currenciesCodesOfSuccessfullyDownloadedFiles_.size();

    spdlog::info("Files requested to download: {}", currenciesCodesOfFilesRequestedToBeDownloaded.size());
    spdlog::info("Files download successfully: {}", currenciesCodesOfSuccessfullyDownloadedFiles_.size());

    if(filesRequestedToBeDownloadedCount == filesDownloadedSuccessfullyCount)
    {
        spdlog::info("Downloaded all exchange rates successfully");
    }
    else
    {
        //log errors
    }

    size_t filesFailedToDownloadCount = errorDescriptionsPerCurrencyCode_.size();

    if(filesFailedToDownloadCount > 0)
    {
        spdlog::error("Files failed to download: {}", errorDescriptionsPerCurrencyCode_.size());
    }
}
