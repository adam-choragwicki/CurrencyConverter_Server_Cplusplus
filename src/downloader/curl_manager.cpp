#include "curl_manager.h"
#include "types/currency_code.h"
#include "spdlog/spdlog.h"
#include "types/definitions.h"
#include <sstream>

CurlManager::CurlManager(const std::string& downloadDirectoryPath) : DOWNLOAD_DIRECTORY_PATH(downloadDirectoryPath), downloadReport_(DOWNLOAD_DIRECTORY_PATH)
{
    verbose_ = false;
    logFileSize_ = true;
}

DownloadReport CurlManager::downloadMultiplexing(const std::set<CurrencyCode>& currenciesCodes)
{
    const CurlMultiHandle curlMultiHandle = Utilities::createMultiHandle();
    std::map<CurrencyCode, std::string> responsesContents;
    setupDownload(curlMultiHandle, currenciesCodes, responsesContents);

    startBatchDownload(curlMultiHandle.get());

    handleResponseCodes(currencyCodesToHandlesMapping_);

    for(const auto&[_, handle] : currencyCodesToHandlesMapping_)
    {
        curl_multi_remove_handle(curlMultiHandle.get(), handle.get());
    }

//    for(const auto&[currencyCode, currencyExchangeRatesJson] : responsesContents)
//    {
//        if(!currencyExchangeRatesJson.empty())
//        {
//            currencyCodeToCurrencyExchangeRatesJsonMapping.try_emplace(currencyCode, currencyExchangeRatesJson);
//        }
//        else
//        {
//            spdlog::warn("No data downloaded for " + currencyCode.toUpperCase());
//        }
//    }

    return downloadReport_;
}

void CurlManager::setupDownload(const CurlMultiHandle& curlMultiHandle, const std::set<CurrencyCode>& currenciesCodes, std::map<CurrencyCode, std::string>& responsesContents)
{
    for(const CurrencyCode& currencyCode : currenciesCodes)
    {
        try
        {
            std::string fileName = DOWNLOAD_DIRECTORY_PATH + "/" + currencyCode.toString() + ".json";
            FILE* file = fopen(fileName.c_str(), "wb");

            if(file)
            {
                spdlog::debug("Opened file '{}'", fileName);
            }
            else
            {
                throw std::runtime_error("Failed to open file: " + fileName);
            }

            currencyCodesToFilesMapping_[currencyCode] = file;

            const auto&[iter1, _1] = currencyCodesToHandlesMapping_.try_emplace(currencyCode, Utilities::createEasyHandle());
            const auto& handle = iter1->second.get();

            curl_easy_setopt(handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);
            curl_easy_setopt(handle, CURLOPT_PIPEWAIT, 1L);
            curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1L);
            curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 1L);

            curl_easy_setopt(handle, CURLOPT_TIMEOUT, 10L); // 10 seconds timeout
            curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, 5L); // 5 seconds connect timeout

            /* TODO Progress bar etc? */
            //curl_easy_setopt(handle, CURLOPT_PROGRESSFUNCTION, progressCallback);
            //curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 0L); // Enable progress meter

            if(verbose_)
            {
                curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
            }

            const std::string url = "https://www.floatrates.com/daily/" + currencyCode.toString() + ".json";
            curl_easy_setopt(handle, CURLOPT_URL, url.c_str());

            Utilities::saveToFile(handle, file);

            curl_multi_add_handle(curlMultiHandle.get(), handle);

            spdlog::info("Queued for download: " + url);
        }
        catch(const std::exception& exception)
        {
            throw CurlError(exception.what());
        }
    }

    curl_multi_setopt(curlMultiHandle.get(), CURLMOPT_PIPELINING, CURLPIPE_MULTIPLEX);
}

void CurlManager::startBatchDownload(CURLM* multiHandle)
{
    spdlog::info("Starting batch download");

    int handlesStillRunningCount{};
    CURLMsg* message{};

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

                char* url{};
                curl_easy_getinfo(message->easy_handle, CURLINFO_EFFECTIVE_URL, &url);

                if(message->data.result == CURLE_OK && url)
                {
                    spdlog::info(std::string("Downloaded ") + url);

                    if(logFileSize_)
                    {
                        double fileSize = 0;
                        curl_easy_getinfo(handle, CURLINFO_SIZE_DOWNLOAD, &fileSize);

                        double fileSizeInKB = fileSize / 1024.0;

                        std::ostringstream formattedSize;
                        formattedSize.precision(2);
                        formattedSize << std::fixed << fileSizeInKB;

                        spdlog::info("Downloaded file size: " + formattedSize.str() + " KB");
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

    if(handlesStillRunningCount > 0)
    {
        spdlog::error("Before closing files, Download handles still running: " + std::to_string(handlesStillRunningCount));
    }

    for(auto&[currencyCode, file] : currencyCodesToFilesMapping_)
    {
        fclose(file);
        spdlog::debug("Closed file for '{}'", currencyCode.toString());
    }

    spdlog::info("Finished batch download");
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
            downloadReport_.addCurrencyCodeOfSuccessfullyDownloadedFile(currencyCode);
            continue;
        }
        else
        {
            std::string message = "Error occurred when trying to download data for " + currencyCode.toString() + ", HTTP code: " + std::to_string(responseCode);

            if(responseCode == 403 || responseCode == 404)
            {
                message += "\nThe currency JSON file is probably no longer available";
                downloadReport_.addDataForFailedDownload(currencyCode, message);
                spdlog::error(message);
            }
            else
            {
                downloadReport_.addDataForFailedDownload(currencyCode, message);
                throw CurlError(message);
            }
        }
    }
}
