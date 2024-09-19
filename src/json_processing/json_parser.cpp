#include "json_parser.h"
#include "json_reader.h"
#include "types/currency_code.h"
#include "types/containers/exchange_rate_data.h"
#include "types/timestamp.h"
#include "types/currency_exchange_rates_json.h"
#include "json_processing/exceptions.h"
#include "spdlog/spdlog.h"

std::set<CurrencyCode> JsonParser::parseCurrenciesListFileContentToCurrenciesCodes(const std::string& string)
{
    //TODO implement expected schema
    JsonReader jsonReader(string);

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

std::map<CurrencyCode, ExchangeRateData> JsonParser::parseExchangeRatesJsonStringToCurrencyCodesToExchangeRatesMapping(const CurrencyCode& sourceCurrencyCode,
                                                                                                                       const std::set<CurrencyCode>& currenciesCodes,
                                                                                                                       const CurrencyExchangeRatesJson& currencyExchangeRatesJson,
                                                                                                                       bool allKeysExistenceRequired)
{
    JsonReader jsonReader(currencyExchangeRatesJson.toString());

    std::map<CurrencyCode, ExchangeRateData> currencyCodeToExchangeRateDataMap;

    //    std::set<CurrencyCode> alreadyWarned;

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
            //            else
            //            {
            //                if(!alreadyWarned.contains(sourceCurrencyCode))
            //                {
            //                    spdlog::warn("Data source does not provide some exchange rates for " + sourceCurrencyCode.toUpperCase());
            //                    alreadyWarned.insert(sourceCurrencyCode);
            //                }
            //            }
        }
    }

    return currencyCodeToExchangeRateDataMap;
}

Timestamp JsonParser::parseTimestamp(const CurrencyCode& currencyCode, const CurrencyExchangeRatesJson& currencyExchangeRatesJson)
{
    JsonReader jsonReader(currencyExchangeRatesJson.toString());
    return Timestamp(jsonReader.getStringValue(currencyCode.toString(), "date"));
}

bool JsonParser::isValidJsonString(const std::string& string)
{
    rapidjson::Document document;
    return !document.Parse(string.c_str()).HasParseError();
}
