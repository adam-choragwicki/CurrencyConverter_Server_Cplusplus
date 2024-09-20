#pragma once

#include "curl/curl.h"
#include "types/currency_exchange_rates_json.h"
#include <memory>
#include <functional>

using CurlEasyHandle = std::unique_ptr<CURL, std::function<void(CURL*)>>;
using CurlMultiHandle = std::unique_ptr<CURLM, std::function<void(CURLM*)>>;

class Utilities
{
public:
    Utilities() = delete;

    static CurlEasyHandle createEasyHandle();
    static CurlMultiHandle createMultiHandle();

    static size_t writeToString(void* ptr, size_t size, size_t nmemb, std::string* data);
    static timeval getTimeout(CURLM* multiHandle);
    static int waitIfNeeded(CURLM* multi_handle, timeval& timeout);

    static void saveToString(CURL* curl, std::string& currencyExchangeRatesJson);
    static void saveToFile(CURL* curl, const std::string& filePath);
};
