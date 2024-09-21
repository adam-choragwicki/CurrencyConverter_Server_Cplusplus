#include "json_validator.h"
#include "rapidjson/document.h"
#include "spdlog/spdlog.h"

bool JsonValidator::isValidJsonString(const std::string& string)
{
    rapidjson::Document document;
    return !document.Parse(string.c_str()).HasParseError();
}

bool JsonValidator::isValidCurrencyExchangeRatesJson(std::string string)
{
    if(!isValidJsonString(string))
    {
        spdlog::error("Error, '{}' is not a valid JSON string");
        return false;
    }

    return false;
}
