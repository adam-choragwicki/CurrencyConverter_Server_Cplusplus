#pragma once

#include "utilities.h"
#include "types/definitions.h"
#include <set>
#include <map>

class CurrencyCode;
class CurrencyExchangeRatesJson;

class CurlError : public std::runtime_error
{
public:
    explicit CurlError(const std::string& reason) : std::runtime_error(reason)
    {}
};

class CurlManager
{
public:
    static CurrencyCodeToCurrencyExchangeRatesJsonMapping downloadMultiplexing(const std::set<CurrencyCode>& currenciesCodes);

private:
    static std::map<CurrencyCode, CurlEasyHandle> setupDownload(const CurlMultiHandle& curlMultiHandle, const std::set<CurrencyCode>& currenciesCodes, std::map<CurrencyCode, std::string>& responsesContents);
    static void startDownload(CURLM* multiHandle);
    static void handleResponseCodes(const std::map<CurrencyCode, CurlEasyHandle>& currencyCodesToHandlesMapping);

    inline static bool verbose_{};
    inline static bool logFileSize_{};
};
