#pragma once

#include "types/definitions.h"
#include <set>
#include <map>
#include <stdexcept>

class CurrencyCode;
class CurrencyExchangeRatesJson;

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

    CurrencyCodeToCurrencyExchangeRatesJsonMapping downloadCurrenciesExchangeRates(const std::set<CurrencyCode>& currenciesCodes);

private:
    inline static bool alreadyCreated_{};
};
