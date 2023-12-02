#include "downloader.h"
#include "spdlog/spdlog.h"

std::map<CurrencyCode, CurrencyExchangeRatesJson> Downloader::downloadMultiplexing(const std::set<CurrencyCode>& currenciesCodes)
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

    std::map<CurrencyCode, CurrencyExchangeRatesJson> currencyCodeToCurrencyExchangeRatesJsonMapping;

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

    return currencyCodeToCurrencyExchangeRatesJsonMapping;
}

std::map<CurrencyCode, CurlEasyHandle> Downloader::setupDownload(const CurlMultiHandle& curlMultiHandle, const std::set<CurrencyCode>& currenciesCodes, std::map<CurrencyCode, std::string>& responsesContents)
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

            const std::string url = "https://www.floatrates.com/daily/" + currencyCode.toString() + ".json";
            curl_easy_setopt(handle, CURLOPT_URL, url.c_str());

            const auto&[iter2, _2] = responsesContents.try_emplace(currencyCode, "");
            Utilities::saveToString(handle, iter2->second);

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

void Downloader::startDownload(CURLM* multiHandle)
{
    spdlog::info("Starting downloads");

    int stillRunningHandlesCount{};
    CURLMsg* message;

    curl_multi_perform(multiHandle, &stillRunningHandlesCount);

    while(stillRunningHandlesCount)
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
                }
                else if(message->data.result == CURLE_COULDNT_RESOLVE_HOST)
                {
                    throw CurlError("Error, could not resolve host. Probably no internet access");
                }
            }
        }
        while(message);

        if(rc >= 0)
        {
            curl_multi_perform(multiHandle, &stillRunningHandlesCount);
        }
    }

    spdlog::info("Finished downloads");
}

void Downloader::handleResponseCodes(const std::map<CurrencyCode, CurlEasyHandle>& currencyCodesToHandlesMapping)
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
