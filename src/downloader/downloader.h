#pragma once

#include "types/currency_code.h"
#include "utilities.h"
#include "types/currency_exchange_rates_json.h"
#include <set>
#include <map>

class CurlError : public std::runtime_error
{
public:
    explicit CurlError(const std::string& reason) : std::runtime_error(reason)
    {}
};

class Downloader
{
public:
    static std::map<CurrencyCode, CurrencyExchangeRatesJson> downloadMultiplexing(const std::set<CurrencyCode>& currenciesCodes);

private:
    static std::map<CurrencyCode, CurlEasyHandle> setupDownload(const CurlMultiHandle& curlMultiHandle, const std::set<CurrencyCode>& currenciesCodes, std::map<CurrencyCode, std::string>& responsesContents);
    static void startDownload(CURLM* multiHandle);
    static void handleResponseCodes(const std::map<CurrencyCode, CurlEasyHandle>& currencyCodesToHandlesMapping);
};
