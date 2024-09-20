#include "curl_manager.h"
#include "types/currency_code.h"
#include "types/currency_exchange_rates_json.h"
#include "spdlog/spdlog.h"
#include "types/definitions.h"

CurrencyCodeToCurrencyExchangeRatesJsonMapping CurlManager::downloadMultiplexing(const std::set<CurrencyCode>& currenciesCodes)
{
    const CurlMultiHandle curlMultiHandle = Utilities::createMultiHandle();
    std::map<CurrencyCode, std::string> responsesContents;
    std::map<CurrencyCode, CurlEasyHandle> currencyCodesToHandlesMapping = setupDownload(curlMultiHandle, currenciesCodes, responsesContents);

    startDownload(curlMultiHandle.get());

    handleResponseCodes(currencyCodesToHandlesMapping);

    for(const auto&[_, handle] : currencyCodesToHandlesMapping)
    {
        curl_multi_remove_handle(curlMultiHandle.get(), handle.get());
    }

    CurrencyCodeToCurrencyExchangeRatesJsonMapping currencyCodeToCurrencyExchangeRatesJsonMapping;

    for(const auto&[currencyCode, currencyExchangeRatesJson] : responsesContents)
    {
        if(!currencyExchangeRatesJson.empty())
        {
            currencyCodeToCurrencyExchangeRatesJsonMapping.try_emplace(currencyCode, currencyExchangeRatesJson);
        }
        else
        {
            spdlog::warn("No data downloaded for " + currencyCode.toUpperCase());
        }
    }

    verbose_ = false;
    logFileSize_ = true;

    return currencyCodeToCurrencyExchangeRatesJsonMapping;
}

std::map<CurrencyCode, CurlEasyHandle> CurlManager::setupDownload(const CurlMultiHandle& curlMultiHandle, const std::set<CurrencyCode>& currenciesCodes, std::map<CurrencyCode, std::string>& responsesContents)
{
    std::map<CurrencyCode, CurlEasyHandle> currencyCodesToHandlesMapping;

    for(const CurrencyCode& currencyCode : currenciesCodes)
    {
        try
        {
            const auto&[iter1, _1] = currencyCodesToHandlesMapping.try_emplace(currencyCode, Utilities::createEasyHandle());
            const auto& handle = iter1->second.get();

            curl_easy_setopt(handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);
            curl_easy_setopt(handle, CURLOPT_PIPEWAIT, 1L);
            curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1L);
            curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 1L);

            if(verbose_)
            {
                curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
            }

            const std::string url = "https://www.floatrates.com/daily/" + currencyCode.toString() + ".json";
            curl_easy_setopt(handle, CURLOPT_URL, url.c_str());

            const std::string fileName = "downloaded_data/" + currencyCode.toString() + ".json";
            Utilities::saveToFile(handle, fileName);

            curl_multi_add_handle(curlMultiHandle.get(), handle);

            spdlog::info("Queued for download: " + url);
        }
        catch(const std::exception& exception)
        {
            throw CurlError(exception.what());
        }
    }

    curl_multi_setopt(curlMultiHandle.get(), CURLMOPT_PIPELINING, CURLPIPE_MULTIPLEX);

    return currencyCodesToHandlesMapping;
}

void CurlManager::startDownload(CURLM* multiHandle)
{
    spdlog::info("Starting downloads");

    int handlesStillRunningCount{};
    CURLMsg* message;

    curl_multi_perform(multiHandle, &handlesStillRunningCount);

    while(handlesStillRunningCount)
    {
        struct timeval timeout = Utilities::getTimeout(multiHandle);
        int rc = Utilities::waitIfNeeded(multiHandle, timeout);

        do
        {
            int messagesLeft = 0;
            message = curl_multi_info_read(multiHandle, &messagesLeft);

            if(message && (message->msg == CURLMSG_DONE))
            {
                CURL* handle = message->easy_handle;
                curl_multi_remove_handle(multiHandle, handle);

                char* url = nullptr;
                curl_easy_getinfo(message->easy_handle, CURLINFO_EFFECTIVE_URL, &url);

                if(message->data.result == CURLE_OK && url)
                {
                    spdlog::info(std::string("Downloaded ") + url);

                    if(logFileSize_)
                    {
                        long fileSize = 0;
                        curl_easy_getinfo(handle, CURLINFO_SIZE_DOWNLOAD, &fileSize);
                        spdlog::info("Downloaded file size: " + std::to_string(fileSize) + " bytes");
                    }
                }
                else
                {
                    spdlog::error("Failed to download: " + std::string(url ? url : "unknown URL") + " with error: " + curl_easy_strerror(message->data.result));

                    if(message->data.result == CURLE_COULDNT_RESOLVE_HOST)
                    {
                        throw CurlError("Error, could not resolve host. Probably no internet access");
                    }
                }
            }
        }
        while(message);

        if(rc >= 0)
        {
            curl_multi_perform(multiHandle, &handlesStillRunningCount);
        }
    }


    spdlog::info("Finished downloads");
}

void CurlManager::handleResponseCodes(const std::map<CurrencyCode, CurlEasyHandle>& currencyCodesToHandlesMapping)
{
    for(const auto&[currencyCode, handle] : currencyCodesToHandlesMapping)
    {
        long responseCode{};

        curl_easy_getinfo(handle.get(), CURLINFO_RESPONSE_CODE, &responseCode);

        if(responseCode == 200)
        {
            /*OK*/
            continue;
        }

        std::string message = "Error occurred when trying to download data for " + currencyCode.toString() + ", HTTP code: " + std::to_string(responseCode);

        if(responseCode == 403 || responseCode == 404)
        {
            message += "\nThe currency JSON file is probably no longer available";
            spdlog::error(message);
        }
        else
        {
            throw CurlError(message);
        }
    }
}
