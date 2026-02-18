#include "utilities.h"
#include "utilities/file_helper.h"
#include "types/currency_code.h"
#include "json_processing/json_parser.h"
#include "types/currency_exchange_rates_json.h"
#include "spdlog/spdlog.h"

std::map<CurrencyCode, std::string> Utilities::getCurrencyCodeToFilePathMapping(const std::string& directoryPath, const std::set<CurrencyCode>& currenciesCodes)
{
    std::map<CurrencyCode, std::string> currencyCodeToFilePathMapping;

    for(const CurrencyCode& currencyCode : currenciesCodes)
    {
        const std::string path = directoryPath + "/" + currencyCode.toString() + ".json";

        if(FileHelper::fileExists(path))
        {
            currencyCodeToFilePathMapping.insert_or_assign(currencyCode, path);
        }
    }

    return currencyCodeToFilePathMapping;
}

std::map<CurrencyCode, ParseResult> Utilities::parseFiles(const std::set<CurrencyCode>& currenciesCodes,
                                                          const std::map<CurrencyCode, std::string>& currencyCodeToFilePathMapping,
                                                          const bool allKeysExistenceRequired)
{
    std::map<CurrencyCode, ParseResult> currencyCodeToParseResultMapping;

    for(const auto&[currencyCode, filePath] : currencyCodeToFilePathMapping)
    {
        try
        {
            std::string fileContent = FileHelper::readFileToString(filePath);

            ParseResult parseResult = JsonParser::parseExchangeRatesJsonStringToCurrencyCodesToExchangeRateDataMapping(currencyCode,
                                                                                                                      currenciesCodes,
                                                                                                                      CurrencyExchangeRatesJson(fileContent),
                                                                                                                      allKeysExistenceRequired);

            if (!parseResult.isSuccess_)
            {
                spdlog::warn("Skipping incomplete/invalid cache file '{}'", filePath);
            }

            currencyCodeToParseResultMapping.insert_or_assign(currencyCode, parseResult);
        }
        catch (const std::exception& ex)
        {
            spdlog::warn("Failed to parse cache file '{}': {}", filePath, ex.what());
            currencyCodeToParseResultMapping.insert_or_assign(currencyCode, ParseResult{false, std::nullopt});
        }
    }

    return currencyCodeToParseResultMapping;
}

std::map<CurrencyCode, ParseResult> Utilities::parseFiles(const std::set<CurrencyCode>& currenciesCodes,
                                                          const std::map<CurrencyCode, std::string>& currencyCodeToFilePathMapping)
{
    return parseFiles(currenciesCodes, currencyCodeToFilePathMapping, false);
}
