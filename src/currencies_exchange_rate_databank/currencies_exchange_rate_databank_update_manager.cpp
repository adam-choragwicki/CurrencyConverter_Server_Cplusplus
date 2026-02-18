#include "currencies_exchange_rate_databank_update_manager.h"
#include "currencies_exchange_rate_databank.h"
#include "currencies_exchange_rate_databank_modifier.h"
#include "utilities/timer.h"
#include "downloader/download_manager.h"
#include "spdlog/spdlog.h"
#include "types/currency_code.h"
#include "downloader/download_report.h"
#include "utilities/file_helper.h"
#include "paths.h"
#include "utilities.h"
#include <filesystem>
#include <algorithm>
#include <system_error>
#include <cstdlib>
#include <chrono>
#include <thread>

namespace
{
    bool shouldUseLocalCache()
    {
#if defined(_WIN32)
        char* value = nullptr;
        size_t valueLen = 0;
        if (_dupenv_s(&value, &valueLen, "UPDATE_CACHE_USE_LOCAL_DATA") == 0 && value)
        {
            free(value);
            return true;
        }
        return false;
#else
        return std::getenv("UPDATE_CACHE_USE_LOCAL_DATA") != nullptr;
#endif
    }

    std::string getLocalCacheDirectory()
    {
#if defined(_WIN32)
        char* value = nullptr;
        size_t valueLen = 0;
        if (_dupenv_s(&value, &valueLen, "UPDATE_CACHE_LOCAL_DIR") == 0 && value)
        {
            std::string result(value);
            free(value);
            return result;
        }
#else
        if (const char* value = std::getenv("UPDATE_CACHE_LOCAL_DIR"))
        {
            return std::string(value);
        }
#endif
        return Paths::CURRENCIES_EXCHANGE_RATE_CACHE_DIRECTORY_PATH;
    }

    int getTestDelayMs()
    {
#if defined(_WIN32)
        char* value = nullptr;
        size_t valueLen = 0;
        if (_dupenv_s(&value, &valueLen, "UPDATE_CACHE_TEST_DELAY_MS") == 0 && value)
        {
            const long delay = std::strtol(value, nullptr, 10);
            free(value);
            return delay > 0 ? static_cast<int>(delay) : 0;
        }
        return 0;
#else
        if (const char* value = std::getenv("UPDATE_CACHE_TEST_DELAY_MS"))
        {
            const long delay = std::strtol(value, nullptr, 10);
            return delay > 0 ? static_cast<int>(delay) : 0;
        }
        return 0;
#endif
    }
}

bool CurrenciesExchangeRatesDatabankUpdateManager::startCurrenciesExchangeRatesDatabankUpdate(CurrenciesExchangeRatesDatabank& currenciesExchangeRatesDatabank,
                                                                                              DownloadManager& downloadManager,
                                                                                              const std::function<void(size_t completed, size_t total)>& onProgress)
{
    spdlog::info("Starting currencies exchange rates update");

    Timer timer;

    const int delayMs = getTestDelayMs();

    if (delayMs > 0)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }

    if (!prepareDownloadDirectory())
    {
        spdlog::error("Failed to prepare download directory. Cache update aborted");
        return false;
    }

    //download
    std::unique_ptr<DownloadReport> downloadReport;

    const std::set<CurrencyCode>& currenciesCodes = currenciesExchangeRatesDatabank.getCurrenciesCodes();
    const size_t total = currenciesCodes.size();

    if (onProgress)
    {
        onProgress(0, total);
    }

    if (shouldUseLocalCache())
    {
        downloadReport = std::make_unique<DownloadReport>(Paths::DOWNLOAD_DIRECTORY_PATH);
        const std::string localCacheDirectory = getLocalCacheDirectory();
        size_t completed{};

        for (const CurrencyCode& currencyCode: currenciesCodes)
        {
            downloadReport->addCurrencyCodeOfFileRequestedToBeDownloaded(currencyCode);

            const std::string sourcePath = localCacheDirectory + "/" + currencyCode.toString() + ".json";
            const std::string destinationPath = Paths::DOWNLOAD_DIRECTORY_PATH + "/" + currencyCode.toString() + ".json";

            if (FileHelper::fileExists(sourcePath))
            {
                std::filesystem::copy_file(sourcePath, destinationPath, std::filesystem::copy_options::overwrite_existing);
                downloadReport->addCurrencyCodeOfSuccessfullyDownloadedFile(currencyCode);
            }
            else
            {
                downloadReport->addDataForFailedDownload(currencyCode, "Local cache file missing");
            }

            ++completed;

            if (onProgress)
            {
                onProgress(completed, total);
            }
        }
    }
    else
    {
        try
        {
            downloadReport = std::make_unique<DownloadReport>(downloadManager.downloadCurrenciesExchangeRatesFiles(Paths::DOWNLOAD_DIRECTORY_PATH,
                                                                                                                   currenciesCodes,
                                                                                                                   onProgress));
        }
        catch (const DownloadError& exception)
        {
            spdlog::error(exception.what() + std::string(".\nCache update aborted"));
            return false;
        }
    }
    //end download

    displayDownloadReportData(*downloadReport);

    const size_t successfullyDownloadedFilesCount = downloadReport->getCurrencyCodesOfSuccessfullyDownloadedFiles().size();

    if (successfullyDownloadedFilesCount == 0)
    {
        spdlog::error("Error, no successfully downloaded currencies exchange rates files\nCache update aborted");
        return false;
    }

    const std::set<CurrencyCode>& allCurrenciesCodes = currenciesExchangeRatesDatabank.getCurrenciesCodes();

    std::map<CurrencyCode, std::string> currencyCodeToFilePathMapping = Utilities::getCurrencyCodeToFilePathMapping(Paths::DOWNLOAD_DIRECTORY_PATH, currenciesExchangeRatesDatabank.getCurrenciesCodes());
    std::map<CurrencyCode, ParseResult> currencyCodeToParseResultMapping = Utilities::parseFiles(allCurrenciesCodes, currencyCodeToFilePathMapping, false);

    CurrenciesExchangeRatesDatabankModifier::modifyCurrenciesExchangeRatesDatabank(currenciesExchangeRatesDatabank, currencyCodeToParseResultMapping);

    const size_t updatedCurrenciesCount = std::ranges::count_if(currencyCodeToParseResultMapping,
                                                                [](const auto& it)
                                                                {
                                                                    return it.second.isSuccess_ &&
                                                                           it.second.currencyCodeToCurrencyExchangeRateDataMapping_ &&
                                                                           !it.second.currencyCodeToCurrencyExchangeRateDataMapping_->empty();
                                                                });

    if (updatedCurrenciesCount == 0)
    {
        spdlog::error("Error, downloaded/cached files were not usable (no successful parses)\nCache update aborted");
        return false;
    }

    spdlog::info("Cache updated successfully in " + timer.getResult());

    return true;
}

bool CurrenciesExchangeRatesDatabankUpdateManager::prepareDownloadDirectory()
{
    std::error_code errorCode;

    if (std::filesystem::exists(Paths::DOWNLOAD_DIRECTORY_PATH))
    {
        std::filesystem::remove_all(Paths::DOWNLOAD_DIRECTORY_PATH, errorCode);

        if (errorCode)
        {
            spdlog::error("Failed to remove download directory '{}': {}", Paths::DOWNLOAD_DIRECTORY_PATH, errorCode.message());
            return false;
        }
    }

    std::filesystem::create_directories(Paths::DOWNLOAD_DIRECTORY_PATH, errorCode);

    if (errorCode)
    {
        spdlog::error("Failed to create download directory '{}': {}", Paths::DOWNLOAD_DIRECTORY_PATH, errorCode.message());
        return false;
    }

    if (!std::filesystem::exists(Paths::DOWNLOAD_DIRECTORY_PATH))
    {
        spdlog::error("Download directory '{}' does not exist after creation", Paths::DOWNLOAD_DIRECTORY_PATH);
        return false;
    }

    return true;
}

void CurrenciesExchangeRatesDatabankUpdateManager::displayDownloadReportData(const DownloadReport& downloadReport)
{
    const std::set<CurrencyCode>& currenciesCodesOfFilesRequestedToBeDownloaded = downloadReport.getCurrenciesCodesOfFilesRequestedToBeDownloaded();
    const std::set<CurrencyCode> currenciesCodesOfSuccessfullyDownloadedFiles_ = downloadReport.getCurrencyCodesOfSuccessfullyDownloadedFiles();
    std::multimap<CurrencyCode, std::string> errorDescriptionsPerCurrencyCode_ = downloadReport.getErrorDescriptionsPerCurrencyCode();

    if (currenciesCodesOfSuccessfullyDownloadedFiles_.empty())
    {
        spdlog::error("Error, no successfully downloaded currencies exchange rates files");
    }

    const size_t filesRequestedToBeDownloadedCount = currenciesCodesOfFilesRequestedToBeDownloaded.size();
    const size_t filesDownloadedSuccessfullyCount = currenciesCodesOfSuccessfullyDownloadedFiles_.size();

    spdlog::info("Files requested to download: {}", currenciesCodesOfFilesRequestedToBeDownloaded.size());
    spdlog::info("Files download successfully: {}", currenciesCodesOfSuccessfullyDownloadedFiles_.size());

    if (filesRequestedToBeDownloadedCount == filesDownloadedSuccessfullyCount)
    {
        spdlog::info("Downloaded all exchange rates successfully");
    }
    else
    {
        for (const auto& [currencyCode, error]: errorDescriptionsPerCurrencyCode_)
        {
            spdlog::warn("Missing/failed download for '{}': {}", currencyCode.toString(), error);
        }
    }

    const size_t filesFailedToDownloadCount = errorDescriptionsPerCurrencyCode_.size();

    if (filesFailedToDownloadCount > 0)
    {
        spdlog::error("Files failed to download: {}", errorDescriptionsPerCurrencyCode_.size());
    }
}
