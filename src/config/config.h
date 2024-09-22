#pragma once

#include <string>

namespace Paths::CurrenciesDatabankConfig
{
    const std::string CURRENCIES_LIST_FILE_PATH = "data/currencies_list.json";
    const std::string CURRENCIES_EXCHANGE_RATE_CACHE_DIRECTORY_PATH = "data/currencies_exchange_rate_cache/";

    const std::string DOWNLOAD_DIRECTORY_PATH = "downloaded_data";
}

class Config
{
public:
    explicit Config(int port, bool debug);

    [[nodiscard]] int getPort() const
    { return port_; }

    [[nodiscard]] bool isDebug() const
    { return debug_; }

private:
    const int port_;
    const bool debug_;
};
