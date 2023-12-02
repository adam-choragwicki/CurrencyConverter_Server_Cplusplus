#pragma once

#include "types/currency_code.h"
#include "types/currency_exchange_rates_json.h"
#include <set>
#include <map>
#include <stdexcept>

class DownloadError : public std::runtime_error
{
public:
    explicit DownloadError(const std::string& reason) : std::runtime_error(reason)
    {}
};

class DownloadManager
{
public:
    static DownloadManager& getInstance();
    std::map<CurrencyCode, CurrencyExchangeRatesJson> downloadCurrenciesExchangeRates(const std::set<CurrencyCode>& currenciesCodes);

private:
    DownloadManager();
    ~DownloadManager();
};
