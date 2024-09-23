#include "json_parser.h"
#include "json_reader.h"
#include "types/currency_code.h"
#include "types/containers/exchange_rate_data.h"
#include "types/timestamp.h"
#include "types/currency_exchange_rates_json.h"
#include "json_processing/exceptions.h"
#include "spdlog/spdlog.h"
#include "types/definitions.h"
#include "types/currencies_list_file_content.h"

std::set<CurrencyCode> JsonParser::parseCurrenciesListFileContentToCurrenciesCodes(const CurrenciesListFileContent& currenciesListFileContent)
{
    //TODO implement expected schema
    JsonReader jsonReader(currenciesListFileContent.toString());

    auto keyValuePairs = jsonReader.getKeyValuePairs();

    if(keyValuePairs.empty())
    {
        throw JsonParseError("Could not parse any currencies codes-names key-value pairs");
    }

    std::set<CurrencyCode> currenciesCodes;

    for(const auto&[key, value] : keyValuePairs)
    {
        if(key.empty())
        {
            throw JsonParseError("Key is empty");
        }
        else if(value.empty())
        {
            throw JsonParseError("Value for '" + key + "' is empty");
        }

        currenciesCodes.insert(CurrencyCode(value));
    }

    return currenciesCodes;
}

ParseResult JsonParser::parseExchangeRatesJsonStringToCurrencyCodesToExchangeRateDataMapping(const CurrencyCode& sourceCurrencyCode,
                                                                                             const std::set<CurrencyCode>& currenciesCodes,
                                                                                             const CurrencyExchangeRatesJson& currencyExchangeRatesJson,
                                                                                             bool allKeysExistenceRequired)
{
    JsonReader jsonReader(currencyExchangeRatesJson.toString());

    CurrencyCodeToCurrencyExchangeRateDataMapping currencyCodeToExchangeRateDataMap;

    for(const CurrencyCode& currencyCode : currenciesCodes)
    {
        if(currencyCode == sourceCurrencyCode)
        {
            /*Skip source currency as it is obviously not present in the JSON */
            continue;
        }
        else
        {
            //TODO if everything is first time loaded and mandatory present

            if(jsonReader.hasKey(currencyCode.toString()))
            {
                try
                {
                    const ExchangeRate exchangeRate(jsonReader.getNumericValueAsString(currencyCode.toString(), "rate"));
                    const Timestamp timestamp(jsonReader.getStringValue(currencyCode.toString(), "date"));

                    ExchangeRateData exchangeRateData(exchangeRate, timestamp);

                    currencyCodeToExchangeRateDataMap.insert_or_assign(currencyCode, exchangeRateData);
                }
                catch(const JsonMissingKeyError& jsonMissingKeyError)
                {
                    if(allKeysExistenceRequired)
                    {
                        spdlog::critical(jsonMissingKeyError.what());
                        exit(1);
                    }
                    else
                    {
                        spdlog::warn(jsonMissingKeyError.what());
                        currencyCodeToExchangeRateDataMap.insert_or_assign(currencyCode, ExchangeRateData(ExchangeRate("NULL"), Timestamp("NULL")));
                    }
                }
            }
            else
            {
                spdlog::error("Error, currency '{}' is not present in JSON", currencyCode.toString());
                return ParseResult(false, std::nullopt);
            }
        }
    }

    return ParseResult{true, currencyCodeToExchangeRateDataMap};
}
