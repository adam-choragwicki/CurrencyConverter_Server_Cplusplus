#include "json_parser.h"
#include "json_reader.h"
#include "spdlog/spdlog.h"

std::set<CurrencyCode> JsonParser::parseJsonToCurrenciesCodes(const std::string& string)
{
    JsonReader jsonReader(string);

    auto keyValuePairs = jsonReader.getKeyValuePairs();

    std::set<CurrencyCode> currenciesCodes;

    for(const auto&[key, value] : keyValuePairs)
    {
        currenciesCodes.insert(CurrencyCode(value));
    }

    return currenciesCodes;
}

std::map<CurrencyCode, ExchangeRate> JsonParser::extractAllExchangeRatesFromCurrencyExchangeRatesJsonString(const CurrencyCode& sourceCurrencyCode, const std::set<CurrencyCode>& currenciesCodes, const CurrencyExchangeRatesJson& currencyExchangeRatesJson)
{
    JsonReader jsonReader(currencyExchangeRatesJson.toString());

    std::map<CurrencyCode, ExchangeRate> currencyCodeToExchangeRateMap;

    std::set<CurrencyCode> alreadyWarned;

    for(const CurrencyCode& currencyCode : currenciesCodes)
    {
        if(currencyCode == sourceCurrencyCode)
        {
            /*Skip source currency as it is obviously not present in the JSON */
            continue;
        }
        else
        {
            if(jsonReader.hasKey(currencyCode.toString()))
            {
                ExchangeRate exchangeRate(jsonReader.getNumericValueAsString(currencyCode.toString(), "rate"));
                currencyCodeToExchangeRateMap.insert_or_assign(currencyCode, exchangeRate);
            }
            else
            {
                if(!alreadyWarned.contains(sourceCurrencyCode))
                {
                    spdlog::warn("Data source does not provide some exchange rates for " + sourceCurrencyCode.toUpperCase());
                    alreadyWarned.insert(sourceCurrencyCode);
                }
            }
        }
    }

    return currencyCodeToExchangeRateMap;
}

Timestamp JsonParser::parseTimestamp(const CurrencyCode& currencyCode, const CurrencyExchangeRatesJson& currencyExchangeRatesJson)
{
    JsonReader jsonReader(currencyExchangeRatesJson.toString());
    return Timestamp(jsonReader.getStringValue(currencyCode.toString(), "date"));
}
