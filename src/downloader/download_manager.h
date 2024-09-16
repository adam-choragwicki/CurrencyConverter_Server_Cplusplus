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
    static DownloadManager& getInstance();
    CurrencyCodeToCurrencyExchangeRatesJsonMapping downloadCurrenciesExchangeRates(const std::set<CurrencyCode>& currenciesCodes);

private:
    DownloadManager();
    ~DownloadManager();
};
