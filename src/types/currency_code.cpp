#include "currency_code.h"
#include <stdexcept>
#include "spdlog/spdlog.h"

CurrencyCode::CurrencyCode(const std::string& code) : AbstractStringWrapper(toLowerCase(code))
{
    if (code.length() != 3)
    {
        spdlog::error("Error while processing currency code: " + code);
        throw std::runtime_error("Expected 3 characters long currency code, actual is " + std::to_string(code.length()));
    }
}

std::string CurrencyCode::toLowerCase(const std::string& str)
{
    std::string lowerCode;
    lowerCode.reserve(str.size());
    std::ranges::transform(str, std::back_inserter(lowerCode), [](const unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return lowerCode;
}
