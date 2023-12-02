#pragma once

#include <set>
#include <map>
#include <stdexcept>

class CurrencyCode;
class DownloadReport;

class DownloadError : public std::runtime_error
{
public:
    explicit DownloadError(const std::string& reason) : std::runtime_error(reason)
    {}
};

class DownloadManager
{
public:
    DownloadManager();

    DownloadManager(const DownloadManager&) = delete;
    DownloadManager(DownloadManager&&) = delete;
    DownloadManager& operator=(const DownloadManager&) = delete;
    DownloadManager& operator=(DownloadManager&&) = delete;
    ~DownloadManager();

    DownloadReport downloadCurrenciesExchangeRatesFiles(const std::string& downloadDirectoryPath, const std::set<CurrencyCode>& currenciesCodes);

private:
    inline static bool alreadyCreated_{};
};
