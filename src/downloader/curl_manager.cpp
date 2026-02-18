#include "curl_manager.h"
#include "types/currency_code.h"
#include "spdlog/spdlog.h"
#include <sstream>
#include <ranges>
#include <optional>

namespace
{
    std::optional<CurrencyCode> tryCurrencyCodeFromUrl(const char* url)
    {
        if (!url)
        {
            return std::nullopt;
        }

        const std::string s(url);
        const size_t slash = s.find_last_of('/');
        if (slash == std::string::npos)
        {
            return std::nullopt;
        }

        std::string last = s.substr(slash + 1);
        const size_t dot = last.find('.');

        if (dot != std::string::npos)
        {
            last = last.substr(0, dot);
        }

        if (last.size() != 3)
        {
            return std::nullopt;
        }

        try
        {
            return CurrencyCode(last);
        }
        catch (...)
        {
            return std::nullopt;
        }
    }
}

CurlManager::CurlManager(const std::string& downloadDirectoryPath) : DOWNLOAD_DIRECTORY_PATH(downloadDirectoryPath), downloadReport_(DOWNLOAD_DIRECTORY_PATH)
{
    logFileSize_ = true;
}

DownloadReport CurlManager::downloadMultiplexing(const std::set<CurrencyCode>& currenciesCodes,
                                                 const std::function<void(size_t completed, size_t total)>& onProgress)
{
    const CurlMultiHandle curlMultiHandle = Utilities::createMultiHandle();
    setupDownload(curlMultiHandle, currenciesCodes);

    startBatchDownload(curlMultiHandle.get(), onProgress);

    handleResponseCodes(currencyCodesToHandlesMapping_);

    for (const auto& handle: currencyCodesToHandlesMapping_ | std::views::values)
    {
        curl_multi_remove_handle(curlMultiHandle.get(), handle.get());
    }

    return downloadReport_;
}

void CurlManager::setupDownload(const CurlMultiHandle& curlMultiHandle, const std::set<CurrencyCode>& currenciesCodes)
{
    for (const CurrencyCode& currencyCode: currenciesCodes)
    {
        try
        {
            std::string fileName = DOWNLOAD_DIRECTORY_PATH + "/" + currencyCode.toString() + ".json";
            FILE* file = fopen(fileName.c_str(), "wb");

            if (file)
            {
                spdlog::debug("Opened file '{}'", fileName);
            }
            else
            {
                throw std::runtime_error("Failed to open file: " + fileName);
            }

            currencyCodesToFilesMapping_[currencyCode] = file;

            const auto& [iter, _] = currencyCodesToHandlesMapping_.try_emplace(currencyCode, Utilities::createEasyHandle());
            const auto& handle = iter->second.get();

            curl_easy_setopt(handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);
            curl_easy_setopt(handle, CURLOPT_PIPEWAIT, 1L);
            curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1L);
            curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 1L);

            curl_easy_setopt(handle, CURLOPT_TIMEOUT, 10L); // 10 seconds timeout
            curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, 5L); // 5 seconds timeout

            const std::string url = "https://www.floatrates.com/daily/" + currencyCode.toString() + ".json";
            curl_easy_setopt(handle, CURLOPT_URL, url.c_str());

            Utilities::saveToFile(handle, file);

            curl_multi_add_handle(curlMultiHandle.get(), handle);

            downloadReport_.addCurrencyCodeOfFileRequestedToBeDownloaded(currencyCode);

            spdlog::info("Queued for download: " + url);
        }
        catch (const std::exception& exception)
        {
            throw CurlError(exception.what());
        }
    }

    curl_multi_setopt(curlMultiHandle.get(), CURLMOPT_PIPELINING, CURLPIPE_MULTIPLEX);
}

void CurlManager::startBatchDownload(CURLM* multiHandle, const std::function<void(size_t completed, size_t total)>& onProgress)
{
    spdlog::info("Starting batch download");

    int handlesStillRunningCount{};
    CURLMsg* message{};

    const size_t total = currencyCodesToHandlesMapping_.size();
    size_t completed{};

    if (onProgress)
    {
        onProgress(0, total);
    }

    curl_multi_perform(multiHandle, &handlesStillRunningCount);

    while (handlesStillRunningCount)
    {
        struct timeval timeout = Utilities::getTimeout(multiHandle);
        const int rc = Utilities::waitIfNeeded(multiHandle, timeout);

        do
        {
            int messagesLeft = 0;
            message = curl_multi_info_read(multiHandle, &messagesLeft);

            if (message && (message->msg == CURLMSG_DONE))
            {
                CURL* handle = message->easy_handle;
                curl_multi_remove_handle(multiHandle, handle);

                ++completed;
                if (onProgress)
                {
                    onProgress(completed, total);
                }

                char* url{};
                curl_easy_getinfo(message->easy_handle, CURLINFO_EFFECTIVE_URL, &url);

                if (message->data.result == CURLE_OK && url)
                {
                    spdlog::info(std::string("Downloaded ") + url);

                    curl_off_t fileSize = 0;
                    curl_easy_getinfo(handle, CURLINFO_SIZE_DOWNLOAD_T, &fileSize);

                    if (fileSize == 0)
                    {
                        std::string errorMsg = "Downloaded file is empty: " + std::string(url);
                        spdlog::error(errorMsg);

                        if (const auto currencyCode = tryCurrencyCodeFromUrl(url))
                        {
                            downloadReport_.addDataForFailedDownload(*currencyCode, errorMsg);
                        }

                        continue;
                    }

                    if (logFileSize_)
                    {
                        const double fileSizeInKB = static_cast<double>(fileSize) / 1024.0;

                        std::ostringstream formattedSize;
                        formattedSize.precision(2);
                        formattedSize << std::fixed << fileSizeInKB;

                        spdlog::info("Downloaded file size: " + formattedSize.str() + " KB");
                    }
                }
                else
                {
                    spdlog::error("Failed to download: " + std::string(url ? url : "unknown URL") + " with error: " + curl_easy_strerror(message->data.result));

                    if (const auto currencyCode = tryCurrencyCodeFromUrl(url))
                    {
                        downloadReport_.addDataForFailedDownload(*currencyCode, curl_easy_strerror(message->data.result));
                    }
                }
            }
        }
        while (message);

        if (rc >= 0)
        {
            curl_multi_perform(multiHandle, &handlesStillRunningCount);
        }
    }

    if (handlesStillRunningCount > 0)
    {
        spdlog::error("Before closing files, Download handles still running: " + std::to_string(handlesStillRunningCount));
    }

    for (auto& [currencyCode, file]: currencyCodesToFilesMapping_)
    {
        fclose(file);
        spdlog::debug("Closed file for '{}'", currencyCode.toString());
    }

    spdlog::info("Finished batch download");
}

void CurlManager::handleResponseCodes(const std::map<CurrencyCode, CurlEasyHandle>& currencyCodesToHandlesMapping)
{
    for (const auto& [currencyCode, handle]: currencyCodesToHandlesMapping)
    {
        long responseCode{};

        curl_easy_getinfo(handle.get(), CURLINFO_RESPONSE_CODE, &responseCode);

        if (responseCode == 200)
        {
            /*OK*/
            downloadReport_.addCurrencyCodeOfSuccessfullyDownloadedFile(currencyCode);
            continue;
        }

        std::string message = "Error occurred when trying to download data for " + currencyCode.toString() + ", HTTP code: " + std::to_string(responseCode);

        if (responseCode == 403 || responseCode == 404)
        {
            message += "\nThe currency JSON file is probably no longer available";
            downloadReport_.addDataForFailedDownload(currencyCode, message);
            spdlog::error(message);
        }
        else
        {
            downloadReport_.addDataForFailedDownload(currencyCode, message);
            spdlog::error(message);
        }
    }
}
