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
        auto[_, inserted] = errorDescriptionsPerCurrencyCode_.insert_or_assign(currencyCode, failureReason);

        if(!inserted)
        {
            throw std::runtime_error("Adding duplicate currency code");
        }
    }

private:
    int requestedFilesToBeDownloadedCount_;
    int filesDownloadedSuccessfullyCount_;
    int errorsCount_;

    //        std::set<CurrencyCode> invalidData_;

    const std::string DOWNLOAD_DIRECTORY_PATH;

    std::set<CurrencyCode> currencyCodesOfSuccessfullyDownloadedFiles_;
    std::map<CurrencyCode, std::string> errorDescriptionsPerCurrencyCode_;
};
