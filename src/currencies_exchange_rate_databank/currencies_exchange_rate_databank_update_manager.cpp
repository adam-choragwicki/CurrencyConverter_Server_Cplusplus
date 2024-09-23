#include "currencies_exchange_rate_databank_update_manager.h"

#include "currencies_exchange_rate_databank.h"
#include "currencies_exchange_rate_databank_modifier.h"
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

bool CurrenciesExchangeRatesDatabankUpdateManager::startCurrenciesExchangeRatesDatabankUpdate(CurrenciesExchangeRatesDatabank& currenciesExchangeRatesDatabank, DownloadManager& downloadManager)
{
    spdlog::info("Starting currencies exchange rates update");

    Timer timer;

    prepareDownloadDirectory();

    //download
    std::unique_ptr<DownloadReport> downloadReport;

    try
    {
        downloadReport = std::make_unique<DownloadReport>(downloadManager.downloadCurrenciesExchangeRatesFiles(Paths::CurrenciesExchangeRatesDatabankConfig::DOWNLOAD_DIRECTORY_PATH, currenciesExchangeRatesDatabank.getCurrenciesCodes()));
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

    const std::set<CurrencyCode>& allCurrenciesCodes = currenciesExchangeRatesDatabank.getCurrenciesCodes();

    std::map<CurrencyCode, std::string> currencyCodeToFilePathMapping = Utilities::getCurrencyCodeToFilePathMapping(Paths::CurrenciesExchangeRatesDatabankConfig::DOWNLOAD_DIRECTORY_PATH, currenciesExchangeRatesDatabank.getCurrenciesCodes());
    std::map<CurrencyCode, ParseResult> currencyCodeToParseResultMapping = Utilities::parseFiles(allCurrenciesCodes, currencyCodeToFilePathMapping);

    CurrenciesExchangeRatesDatabankModifier::modifyCurrenciesExchangeRatesDatabank(currenciesExchangeRatesDatabank, currencyCodeToParseResultMapping);

    spdlog::info("Cache updated successfully in " + timer.getResult());

    return true;
}

void CurrenciesExchangeRatesDatabankUpdateManager::prepareDownloadDirectory()
{
    //TODO add error handling

    if(std::filesystem::exists(Paths::CurrenciesExchangeRatesDatabankConfig::DOWNLOAD_DIRECTORY_PATH))
    {
        std::filesystem::remove_all(Paths::CurrenciesExchangeRatesDatabankConfig::DOWNLOAD_DIRECTORY_PATH);
    }

    std::filesystem::create_directory(Paths::CurrenciesExchangeRatesDatabankConfig::DOWNLOAD_DIRECTORY_PATH);
}

void CurrenciesExchangeRatesDatabankUpdateManager::displayDownloadReportData(const DownloadReport& downloadReport)
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
