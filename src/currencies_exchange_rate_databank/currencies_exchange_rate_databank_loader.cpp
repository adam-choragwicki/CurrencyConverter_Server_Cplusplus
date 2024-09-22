#include "currencies_exchange_rate_databank_loader.h"
#include "spdlog/spdlog.h"
#include "types/definitions.h"
#include "config/config.h"
#include "utilities/files_helper.h"
#include "json_processing/json_parser.h"
#include "types/currency_code.h"
#include "currencies_exchange_rate_databank.h"
#include "types/currency_exchange_rates_json.h"

void CurrenciesExchangeRateDatabankLoader::loadCurrenciesExchangeRatesCacheFromFiles(CurrenciesExchangeRateDatabank& currenciesExchangeRateDatabank, const std::set<CurrencyCode>& currenciesCodes, const std::string& directoryPath)
{
    spdlog::info("Loading exchange rates data for {} currencies", currenciesCodes.size());

    auto getCurrencyCodeToFilePathMappingOfDownloadedFiles = [](const std::string& directoryPath, const std::set<CurrencyCode>& currenciesCodes)
    {
        std::map<CurrencyCode, std::string> currencyCodeToFilePathMapping;

        for(const CurrencyCode& currencyCode : currenciesCodes)
        {
            const std::string path = Paths::CurrenciesDatabankConfig::DOWNLOAD_DIRECTORY_PATH + "/" + currencyCode.toString() + ".json";

            if(FilesHelper::fileExists(path))
            {
                currencyCodeToFilePathMapping.insert_or_assign(currencyCode, path);
            }
        }

        return currencyCodeToFilePathMapping;
    };

    auto parseFiles = [&currenciesCodes](const std::map<CurrencyCode, std::string>& currencyCodeToFilePathMapping)
    {
        std::map<CurrencyCode, ParseResult> currencyCodeToParseResultMapping;

        for(const auto&[currencyCode, filePath] : currencyCodeToFilePathMapping)
        {
            std::string fileContent = FilesHelper::loadFileContent(filePath);

            ParseResult parseResult = JsonParser::parseExchangeRatesJsonStringToCurrencyCodesToExchangeRateDataMapping(currencyCode, currenciesCodes, CurrencyExchangeRatesJson(fileContent));

            currencyCodeToParseResultMapping.insert_or_assign(currencyCode, parseResult);
        }

        return currencyCodeToParseResultMapping;
    };

    auto loadDatabank = [&currenciesExchangeRateDatabank](const std::map<CurrencyCode, ParseResult>& currencyCodeToParseResultMapping)
    {
        for(const auto&[currencyCode, parseResult] : currencyCodeToParseResultMapping)
        {
            if(parseResult.isSuccess_)
            {
                currenciesExchangeRateDatabank.insertAllExchangeRatesDataForCurrency(currencyCode, *parseResult.currencyCodeToCurrencyExchangeRateDataMapping_);
            }
        }
    };

    std::map<CurrencyCode, std::string> currencyCodeToFilePathMapping = getCurrencyCodeToFilePathMappingOfDownloadedFiles(directoryPath, currenciesCodes);
    std::map<CurrencyCode, ParseResult> currencyCodeToParseResultMapping = parseFiles(currencyCodeToFilePathMapping);
    loadDatabank(currencyCodeToParseResultMapping);

    //    for(const CurrencyCode& currencyCode : currenciesCodes)
    //    {
    //        const std::string filePath = directoryPath + currencyCode.toString() + ".json";

    //        if(FilesHelper::fileExists(filePath))
    //        {
    //            const CurrencyExchangeRatesJson currencyExchangeRatesJson = CurrencyExchangeRatesJson(FilesHelper::loadFileContent(filePath));
    //
    //            if(JsonValidator::isValidJsonString(currencyExchangeRatesJson.toString()))
    //            {
    //                ParseResult parseResult = JsonParser::parseExchangeRatesJsonStringToCurrencyCodesToExchangeRateDataMapping(currencyCode, currenciesCodes_, currencyExchangeRatesJson, true);
    //                const CurrencyCodeToCurrencyExchangeRateDataMapping currencyCodeToExchangeRateDataMap = *parseResult.currencyCodeToCurrencyExchangeRateDataMapping_;
    //
    //                for(const auto&[currencyCode2, exchangeRateData] : currencyCodeToExchangeRateDataMap)
    //                {
    //                    if(!exchangeRateData.isNull())
    //                    {
    //                        currenciesExchangeRatesCache_.insert_or_assign(currencyCode, currencyCodeToExchangeRateDataMap);
    //                    }
    //                    else
    //                    {
    //                        spdlog::error("Wrong new exchange rate data, previous exchange rate will be kept for consistency");
    //                    }
    //                }
    //            }
    //            else
    //            {
    //                spdlog::critical("Error while loading currencies exchange rates cache.\n File '" + filePath + "' is not a valid JSON string");
    //                exit(1);
    //            }
    //        }
    //        else
    //        {
    //            spdlog::critical("Error while loading currencies exchange rates cache.\n File '" + filePath + "' does not exist");
    //            exit(1);
    //        }
    //    }

    spdlog::info("Loaded exchange rates data for {} currencies", currenciesExchangeRateDatabank.size());
}
