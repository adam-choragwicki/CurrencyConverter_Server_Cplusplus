#include "currency_code.h"
#include <stdexcept>
#include "spdlog/spdlog.h"

CurrencyCode::CurrencyCode(const std::string& code) : AbstractStringWrapper(toLowerCase(code))
{
    if(code.length() != 3)
    {
        spdlog::error("Error while processing currency code: " + code);
        throw std::runtime_error("Expected 3 characters long currency code, actual is " + std::to_string(code.length()));
    }
}

std::string CurrencyCode::toLowerCase(const std::string& string)
{
    std::string lowerCode;
    std::transform(string.begin(), string.end(), std::back_inserter(lowerCode), ::tolower);
    return lowerCode;
}
