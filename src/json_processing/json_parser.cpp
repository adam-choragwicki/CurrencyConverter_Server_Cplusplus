#include "json_parser.h"
#include "json_reader.h"
#include "types/currency_code.h"
#include "types/containers/exchange_rate_data.h"
#include "types/timestamp.h"
#include "types/currency_exchange_rates_json.h"
#include "json_processing/exceptions.h"
#include "spdlog/spdlog.h"
#include "types/definitions.h"
#include "types/currencies_names_and_codes_file_content.h"

ConfigData JsonParser::parseConfigData(const std::string& configFileContent)
{
    const JsonReader jsonReader(configFileContent);

    const std::string initialSourceCurrencyCode = jsonReader.getStringValue("initial_source_currency");
    const std::string initialTargetCurrencyCode = jsonReader.getStringValue("initial_target_currency");

    // spdlog::debug(
    //     "Config data:\n"
    //     "\t\t\t\t\tinitial_source_currency: {}\n"
    //     "\t\t\t\t\tinitial_target_currency: {}\n",
    //     initialSourceCurrencyCode,
    //     initialTargetCurrencyCode);

    return {initialSourceCurrencyCode, initialTargetCurrencyCode};
}

ConnectionData JsonParser::parseConnectionData(const std::string& configFileContent)
{
    const JsonReader jsonReader(configFileContent);

    const std::string host = jsonReader.getStringValue("host");
    const std::string portStr = jsonReader.getNumericValueAsString("port");

    int port = 0;
    try
    {
        port = std::stoi(portStr);
    }
    catch (const std::exception&)
    {
        throw JsonParseError("Invalid port value: " + portStr);
    }

    if (port <= 0 || port > 65535)
    {
        throw JsonParseError("Port out of range: " + portStr);
    }

    return {host, port};
}

std::set<CurrencyCode> JsonParser::parseCurrenciesNamesAndCodesFileToCurrenciesCodes(const CurrenciesNamesAndCodesFileContent& currenciesNamesAndCodesFileContent)
{
    const JsonReader jsonReader(currenciesNamesAndCodesFileContent.toString());

    auto keyValuePairs = jsonReader.getKeyValuePairs();

    if (keyValuePairs.empty())
    {
        throw JsonParseError("Could not parse any currencies codes-names key-value pairs");
    }

    std::set<CurrencyCode> currenciesCodes;

    for (const auto& [key, value]: keyValuePairs)
    {
        if (key.empty())
        {
            throw JsonParseError("Key is empty");
        }

        if (value.empty())
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
    const JsonReader jsonReader(currencyExchangeRatesJson.toString());

    CurrencyCodeToCurrencyExchangeRateDataMapping currencyCodeToExchangeRateDataMap;

    for (const CurrencyCode& currencyCode: currenciesCodes)
    {
        if (currencyCode == sourceCurrencyCode)
        {
            /*Skip source currency as it is obviously not present in the JSON */
            continue;
        }

        if (jsonReader.hasKey(currencyCode.toString()))
        {
            try
            {
                const ExchangeRate exchangeRate(jsonReader.getNumericValueAsString(currencyCode.toString(), "rate"));
                const Timestamp timestamp(jsonReader.getStringValue(currencyCode.toString(), "date"));

                ExchangeRateData exchangeRateData(exchangeRate, timestamp);

                currencyCodeToExchangeRateDataMap.insert_or_assign(currencyCode, exchangeRateData);
            }
            catch (const JsonMissingKeyError& jsonMissingKeyError)
            {
                spdlog::warn(jsonMissingKeyError.what());

                if (allKeysExistenceRequired)
                {
                    return ParseResult{false, std::nullopt};
                }
            }
        }
        else
        {
            if (allKeysExistenceRequired)
            {
                spdlog::error("Error, currency '{}' is not present in JSON", currencyCode.toString());
                return ParseResult(false, std::nullopt);
            }

            spdlog::warn("Currency '{}' is not present in JSON; skipping", currencyCode.toString());
        }
    }

    return ParseResult{true, currencyCodeToExchangeRateDataMap};
}

std::map<std::string, std::string> JsonParser::parseCurrenciesNamesAndCodesFileToMap(const CurrenciesNamesAndCodesFileContent& currenciesNamesAndCodesFileContent)
{
    // Expected schema:
    // {
    //   "U.S. Dollar": "USD",
    //   "Euro": "EUR",
    //   ...
    // }

    const JsonReader jsonReader(currenciesNamesAndCodesFileContent.toString());
    const auto keyValuePairs = jsonReader.getKeyValuePairs();

    if (keyValuePairs.empty())
    {
        throw JsonParseError("Could not parse any currencies names-codes key-value pairs");
    }

    std::map<std::string, std::string> currenciesNamesAndCodes;

    for (const auto& [currencyName, currencyCode]: keyValuePairs)
    {
        if (currencyName.empty())
        {
            throw JsonParseError("Currency name key is empty");
        }

        if (currencyCode.empty())
        {
            throw JsonParseError("Currency code for '" + currencyName + "' is empty");
        }

        const auto [_, inserted] = currenciesNamesAndCodes.emplace(currencyName, currencyCode);
        if (!inserted)
        {
            throw JsonParseError("Duplicate currency name key: '" + currencyName + "'");
        }
    }

    return currenciesNamesAndCodes;
}
