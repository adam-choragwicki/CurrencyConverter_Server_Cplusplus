#pragma once

#include <set>
#include <stdexcept>
#include <functional>

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

    DownloadReport downloadCurrenciesExchangeRatesFiles(const std::string& downloadDirectoryPath,
                                                        const std::set<CurrencyCode>& currenciesCodes,
                                                        const std::function<void(size_t completed, size_t total)>& onProgress);

private:
    inline static bool alreadyCreated_{};
};
