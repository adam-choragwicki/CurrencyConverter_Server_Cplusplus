#include "currencies_exchange_rate_databank_updater.h"
#include "currencies_exchange_rate_databank.h"
#include "utilities/timer.h"
#include "downloader/download_manager.h"
#include "spdlog/spdlog.h"
#include "types/currency_code.h"
#include "types/definitions.h"
#include "types/currency_exchange_rates_json.h"
#include "downloader/download_report.h"
#include "utilities/files_helper.h"
#include "json_processing/json_parser.h"
#include "json_processing/json_validator.h"

bool CurrenciesExchangeRateDatabankUpdater::startCacheUpdate(CurrenciesExchangeRateDatabank& currenciesDatabank, DownloadManager& downloadManager)
{
    spdlog::info("Starting currencies exchange rates update");

    //download

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

    const std::string downloadDirectoryPath = downloadReport->getDownloadDirectoryPath();

    std::map<CurrencyCode, std::string> currencyCodeToFilePathMapping;

    for(const CurrencyCode& currencyCode : currenciesDatabank.getCurrenciesCodes())
    {
        const std::string path = downloadDirectoryPath + "/" + currencyCode.toString() + ".json";

        if(FilesHelper::fileExists(path))
        {
            currencyCodeToFilePathMapping.insert_or_assign(currencyCode, path);
        }
    }

    for(const auto&[currencyCode, filePath] : currencyCodeToFilePathMapping)
    {
        std::string fileContent = FilesHelper::loadFileContent(filePath);

        if(JsonValidator::isValidJsonString(fileContent))
        {
            JsonParser::parseExchangeRatesJsonStringToCurrencyCodesToExchangeRateDataMapping(currencyCode, currenciesDatabank.getCurrenciesCodes(), CurrencyExchangeRatesJson(fileContent), true);
        }


    }

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



    std::set<CurrencyCode> currenciesCodesOfFilesRequestedToBeDownloaded = downloadReport->getCurrenciesCodesOfFilesRequestedToBeDownloaded();
    std::set<CurrencyCode> currenciesCodesOfSuccessfullyDownloadedFiles_ = downloadReport->getCurrencyCodesOfSuccessfullyDownloadedFiles();
    std::multimap<CurrencyCode, std::string> errorDescriptionsPerCurrencyCode_ = downloadReport->getErrorDescriptionsPerCurrencyCode();

    if(currenciesCodesOfSuccessfullyDownloadedFiles_.empty())
    {
        spdlog::error("Error, no successfully downloaded currencies exchange rates files\nCache update aborted");
        return false;
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

    currenciesDatabank.updateCurrenciesExchangeRatesCacheFromFiles(currenciesCodesOfSuccessfullyDownloadedFiles_, downloadDirectoryPath);

    spdlog::info("Cache updated successfully in " + timer.getResult());

    return true;
}
