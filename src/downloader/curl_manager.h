#pragma once

#include "utilities.h"
#include "download_report.h"
#include <set>
#include <map>
#include <functional>

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

    DownloadReport downloadMultiplexing(const std::set<CurrencyCode>& currenciesCodes,
                                        const std::function<void(size_t completed, size_t total)>& onProgress);

private:
    void setupDownload(const CurlMultiHandle& curlMultiHandle, const std::set<CurrencyCode>& currenciesCodes);
    void startBatchDownload(CURLM* multiHandle, const std::function<void(size_t completed, size_t total)>& onProgress);

    void handleResponseCodes(const std::map<CurrencyCode, CurlEasyHandle>& currencyCodesToHandlesMapping);

    const std::string DOWNLOAD_DIRECTORY_PATH;

    std::map<CurrencyCode, FILE*> currencyCodesToFilesMapping_;
    std::map<CurrencyCode, CurlEasyHandle> currencyCodesToHandlesMapping_;

    DownloadReport downloadReport_;

    bool logFileSize_{};
};
