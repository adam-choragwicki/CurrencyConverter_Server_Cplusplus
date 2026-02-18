#pragma once

#include <string>

namespace Paths
{
    inline std::string readEnvOrDefault(const char* name, const char* fallback)
    {
#if defined(_WIN32)
        char* value = nullptr;
        size_t valueLen = 0;
        if (_dupenv_s(&value, &valueLen, name) == 0 && value)
        {
            std::string result(value);
            free(value);
            return result;
        }
        return fallback;
#else
        if (const char* value = std::getenv(name))
        {
            return std::string(value);
        }
        return fallback;
#endif
    }

    const std::string CONNECTION_FILE_PATH = "connection.json";
    const std::string CONFIG_FILE_PATH = "data/config.json";

    const std::string CURRENCIES_NAMES_AND_CODES_FILE_PATH =
        readEnvOrDefault("CURRENCIES_NAMES_AND_CODES_FILE_PATH", "data/currencies_names_and_codes.json");
    const std::string CURRENCIES_EXCHANGE_RATE_CACHE_DIRECTORY_PATH =
        readEnvOrDefault("CURRENCIES_EXCHANGE_RATE_CACHE_DIRECTORY_PATH", "data/currencies_exchange_rate_cache/");

    // const std::string CURRENCIES_NAMES_AND_CODES_FILE_PATH = "data/mini_currencies_names_and_codes.json"; // for dev-testing purposes
    // const std::string CURRENCIES_EXCHANGE_RATE_CACHE_DIRECTORY_PATH = "data/mini_currencies_exchange_rate_cache/"; // for dev-testing purposes

    const std::string DOWNLOAD_DIRECTORY_PATH = "downloaded_data";
}
