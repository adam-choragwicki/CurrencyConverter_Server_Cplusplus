#include "utilities.h"
#include "utilities/files_helper.h"
#include "types/currency_code.h"
#include "json_processing/json_parser.h"
#include "types/currency_exchange_rates_json.h"

std::map<CurrencyCode, std::string> Utilities::getCurrencyCodeToFilePathMapping(const std::string& directoryPath, const std::set<CurrencyCode>& currenciesCodes)
{
    std::map<CurrencyCode, std::string> currencyCodeToFilePathMapping;

    for(const CurrencyCode& currencyCode : currenciesCodes)
    {
        const std::string path = directoryPath + "/" + currencyCode.toString() + ".json";

        if(FilesHelper::fileExists(path))
        {
            currencyCodeToFilePathMapping.insert_or_assign(currencyCode, path);
        }
    }

    return currencyCodeToFilePathMapping;
}

std::map<CurrencyCode, ParseResult> Utilities::parseFiles(const std::set<CurrencyCode>& currenciesCodes, const std::map<CurrencyCode, std::string>& currencyCodeToFilePathMapping)
{
    std::map<CurrencyCode, ParseResult> currencyCodeToParseResultMapping;

    for(const auto&[currencyCode, filePath] : currencyCodeToFilePathMapping)
    {
        std::string fileContent = FilesHelper::loadFileContent(filePath);

        ParseResult parseResult = JsonParser::parseExchangeRatesJsonStringToCurrencyCodesToExchangeRateDataMapping(currencyCode, currenciesCodes, CurrencyExchangeRatesJson(fileContent));

        currencyCodeToParseResultMapping.insert_or_assign(currencyCode, parseResult);
    }

    return currencyCodeToParseResultMapping;
}
