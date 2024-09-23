#include "currencies_exchange_rate_databank_initializer.h"
#include "spdlog/spdlog.h"
#include "config/config.h"
#include "utilities/files_helper.h"
#include "json_processing/json_parser.h"
#include "types/currency_code.h"
#include "currencies_exchange_rate_databank.h"
#include "utilities.h"

void CurrenciesExchangeRateDatabankInitializer::loadCurrenciesExchangeRatesCacheFromFiles(CurrenciesExchangeRateDatabank& currenciesExchangeRateDatabank, const std::string& directoryPath)
{
    const std::set<CurrencyCode>& allCurrenciesCodes = currenciesExchangeRateDatabank.getCurrenciesCodes();

    spdlog::info("Loading exchange rates data for {} currencies", allCurrenciesCodes.size());

    auto initializeCurrenciesExchangeRateDatabank = [](CurrenciesExchangeRateDatabank& currenciesExchangeRateDatabank, const std::map<CurrencyCode, ParseResult>& currencyCodeToParseResultMapping)
    {
        for(const auto&[currencyCode, parseResult] : currencyCodeToParseResultMapping)
        {
            if(parseResult.isSuccess_)
            {
                currenciesExchangeRateDatabank.insertAllExchangeRatesDataForCurrency(currencyCode, *parseResult.currencyCodeToCurrencyExchangeRateDataMapping_);
            }
        }
    };

    std::map<CurrencyCode, std::string> currencyCodeToFilePathMapping = Utilities::getCurrencyCodeToFilePathMapping(directoryPath, allCurrenciesCodes);
    std::map<CurrencyCode, ParseResult> currencyCodeToParseResultMapping = Utilities::parseFiles(allCurrenciesCodes, currencyCodeToFilePathMapping);

    initializeCurrenciesExchangeRateDatabank(currenciesExchangeRateDatabank, currencyCodeToParseResultMapping);

    spdlog::info("Loaded exchange rates data for {} currencies", currenciesExchangeRateDatabank.size());
}

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
