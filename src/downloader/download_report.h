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

    [[nodiscard]] std::set<CurrencyCode> getCurrencyCodesOfSuccessfullyDownloadedFiles() const
    {
        return currencyCodesOfSuccessfullyDownloadedFiles_;
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
    int requestedFilesToBeDownloadedCount_;
    int filesDownloadedSuccessfullyCount_;
    int errorsCount_;

    const std::string DOWNLOAD_DIRECTORY_PATH;

    std::set<CurrencyCode> currencyCodesOfSuccessfullyDownloadedFiles_;
    std::multimap<CurrencyCode, std::string> errorDescriptionsPerCurrencyCode_;
};
