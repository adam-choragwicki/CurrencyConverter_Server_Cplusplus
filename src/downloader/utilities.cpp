#include "utilities.h"
#include "spdlog/spdlog.h"
#include <thread>

CurlEasyHandle Utilities::createEasyHandle()
{
    auto curl = CurlEasyHandle(curl_easy_init(), curl_easy_cleanup);

    if(!curl)
    {
        throw std::runtime_error("Failed creating CURL easy object");
    }

    return curl;
}

CurlMultiHandle Utilities::createMultiHandle()
{
    auto curl = CurlMultiHandle(curl_multi_init(), curl_multi_cleanup);

    if(!curl)
    {
        throw std::runtime_error("Failed creating CURL multi object");
    }

    return curl;
}

size_t Utilities::writeToString(void* ptr, size_t size, size_t nmemb, std::string* data)
{
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}

void Utilities::saveToString(CURL* curl, std::string& currencyExchangeRatesJson)
{
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &Utilities::writeToString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &currencyExchangeRatesJson);
}

void Utilities::saveToFile(CURL* curl, FILE* file)
{
    /* Set the write function to fwrite */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fwrite);

    /* Pass the file pointer as the memory to write into */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
}

timeval Utilities::getTimeout(CURLM* multiHandle)
{
    long curlTimeout = -1;

    struct timeval timeout{};
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    curl_multi_timeout(multiHandle, &curlTimeout);

    if(curlTimeout >= 0)
    {
        timeout.tv_sec = curlTimeout / 1000;
        if(timeout.tv_sec > 1)
        {
            timeout.tv_sec = 1;
        }
        else
        {
            timeout.tv_usec = (curlTimeout % 1000) * 1000;
        }
    }

    return timeout;
}

int Utilities::waitIfNeeded(CURLM* multi_handle, timeval& timeout)
{
    fd_set fdread;
    fd_set fdwrite;
    fd_set fdexcep;

    FD_ZERO(&fdread);
    FD_ZERO(&fdwrite);
    FD_ZERO(&fdexcep);

    int maxfd = -1;

    CURLMcode curlMCode = curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);

    if(curlMCode != CURLM_OK)
    {
        spdlog::error("curl_multi_fdset() failed, code " + std::to_string(static_cast<int>(curlMCode)));
    }

    if(maxfd == -1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    int rc = maxfd != -1 ? select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout) : 0;
    return rc;
}
