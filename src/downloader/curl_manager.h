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
    explicit CurlManager(const std::string& downloadDirectoryPath);

    CurrencyCodeToCurrencyExchangeRatesJsonMapping downloadMultiplexing(const std::set<CurrencyCode>& currenciesCodes);

private:
    void setupDownload(const CurlMultiHandle& curlMultiHandle, const std::set<CurrencyCode>& currenciesCodes, std::map<CurrencyCode, std::string>& responsesContents);
    void startDownload(CURLM* multiHandle);

    void handleResponseCodes(const std::map<CurrencyCode, CurlEasyHandle>& currencyCodesToHandlesMapping);

    const std::string DOWNLOAD_DIRECTORY_PATH;

    std::map<CurrencyCode, FILE*> currencyCodesToFilesMapping_;
    std::map<CurrencyCode, CurlEasyHandle> currencyCodesToHandlesMapping_;

    bool verbose_{};
    bool logFileSize_{};
};
