#include "json_validator.h"
#include "rapidjson/document.h"
#include "types/currency_code.h"

bool JsonValidator::isValidJsonString(const std::string& string)
{
    rapidjson::Document document;
    return !document.Parse(string.c_str()).HasParseError();
}
