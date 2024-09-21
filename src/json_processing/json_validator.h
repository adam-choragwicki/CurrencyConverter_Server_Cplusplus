#pragma once

#include <string>

class JsonValidator
{
public:
    static bool isValidJsonString(const std::string& string);

    static bool isValidCurrencyExchangeRatesJson(std::string string);
};
