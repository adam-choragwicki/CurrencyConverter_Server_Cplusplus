#pragma once

#include <string>
#include <set>
#include <map>

class CurrencyCode;
class ParseResult;

class Utilities
{
public:
    static std::map<CurrencyCode, std::string> getCurrencyCodeToFilePathMapping(const std::string& directoryPath, const std::set<CurrencyCode>& currenciesCodes);;
    static std::map<CurrencyCode, ParseResult> parseFiles(const std::set<CurrencyCode>& currenciesCodes, const std::map<CurrencyCode, std::string>& currencyCodeToFilePathMapping);;
};
