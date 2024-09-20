#pragma once

#include "types/currency_code.h"
#include <map>
#include <set>

class DownloadReport
{
public:
    explicit DownloadReport(const std::string& downloadDirectoryPath);

    [[nodiscard]] const std::string& getDownloadDirectoryPath() const
    {
        return DOWNLOAD_DIRECTORY_PATH;
    }

    [[nodiscard]] const std::set<CurrencyCode>& getCurrenciesCodesOfFilesRequestedToBeDownloaded() const
    {
        return currenciesCodesOfFilesRequestedToBeDownloaded_;
    }

    [[nodiscard]] std::set<CurrencyCode> getCurrencyCodesOfSuccessfullyDownloadedFiles() const
    {
        return currencyCodesOfSuccessfullyDownloadedFiles_;
    }

    [[nodiscard]] const std::multimap<CurrencyCode, std::string>& getErrorDescriptionsPerCurrencyCode() const
    {
        return errorDescriptionsPerCurrencyCode_;
    }

    void addCurrencyCodeOfFileRequestedToBeDownloaded(const CurrencyCode& currencyCode)
    {
        const auto[_, inserted] = currenciesCodesOfFilesRequestedToBeDownloaded_.insert(currencyCode);

        if(!inserted)
        {
            throw std::runtime_error("Error, trying to download duplicate file for currency '" + currencyCode.toString() + "'");
        }
    }

    void addCurrencyCodeOfSuccessfullyDownloadedFile(const CurrencyCode& currencyCode)
    {
        auto[_, inserted] = currencyCodesOfSuccessfullyDownloadedFiles_.insert(currencyCode);

        if(!inserted)
        {
            throw std::runtime_error("Adding duplicate currency code");
        }
    }

    void addDataForFailedDownload(const CurrencyCode& currencyCode, const std::string& failureReason)
    {
        /* Check if the currency code already has two errors recorded */
        auto range = errorDescriptionsPerCurrencyCode_.equal_range(currencyCode);

        if(std::distance(range.first, range.second) >= 2)
        {
            throw std::runtime_error("Adding third error for currency code: " + currencyCode.toString());
        }

        errorDescriptionsPerCurrencyCode_.emplace(currencyCode, failureReason);
    }

private:
    const std::string DOWNLOAD_DIRECTORY_PATH;

    std::set<CurrencyCode> currenciesCodesOfFilesRequestedToBeDownloaded_;
    std::set<CurrencyCode> currencyCodesOfSuccessfullyDownloadedFiles_;
    std::multimap<CurrencyCode, std::string> errorDescriptionsPerCurrencyCode_;
};
