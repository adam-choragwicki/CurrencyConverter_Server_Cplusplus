#include "currencies_exchange_rate_databank_initializer.h"
#include "spdlog/spdlog.h"
#include "json_processing/json_parser.h"
#include "types/currency_code.h"
#include "currencies_exchange_rate_databank.h"
#include "utilities/utilities.h"
#include <stdexcept>
#include <vector>

void CurrenciesExchangeRatesDatabankInitializer::loadCurrenciesExchangeRatesCacheFromFiles(CurrenciesExchangeRatesDatabank& currenciesExchangeRatesDatabank, const std::string& directoryPath)
{
    const std::set<CurrencyCode>& allCurrenciesCodes = currenciesExchangeRatesDatabank.getCurrenciesCodes();

    const std::map<CurrencyCode, std::string> currencyCodeToFilePathMapping = Utilities::getCurrencyCodeToFilePathMapping(directoryPath, allCurrenciesCodes);
    std::vector<std::string> missingCurrencyCodes;

    for (const CurrencyCode& currencyCode: allCurrenciesCodes)
    {
        if (!currencyCodeToFilePathMapping.contains(currencyCode))
        {
            missingCurrencyCodes.push_back(currencyCode.toString());
        }
    }

    if (!missingCurrencyCodes.empty())
    {
        std::string message = "Missing cache files for currencies: ";

        for (size_t i = 0; i < missingCurrencyCodes.size(); ++i)
        {
            if (i > 0)
            {
                message += ", ";
            }

            message += missingCurrencyCodes[i];
        }

        spdlog::critical(message);
        throw std::runtime_error(message);
    }

    const std::map<CurrencyCode, ParseResult> currencyCodeToParseResultMapping = Utilities::parseFiles(allCurrenciesCodes, currencyCodeToFilePathMapping, true);

    initializeCurrenciesExchangeRatesDatabank(currenciesExchangeRatesDatabank, currencyCodeToParseResultMapping);

    spdlog::info("Loaded exchange rates data for {} currencies", currenciesExchangeRatesDatabank.size());
}

void CurrenciesExchangeRatesDatabankInitializer::initializeCurrenciesExchangeRatesDatabank(CurrenciesExchangeRatesDatabank& currenciesExchangeRatesDatabank, const std::map<CurrencyCode, ParseResult>& currencyCodeToParseResultMapping)
{
    for (const auto& [currencyCode, parseResult]: currencyCodeToParseResultMapping)
    {
        if (!parseResult.isSuccess_ || !parseResult.currencyCodeToCurrencyExchangeRateDataMapping_)
        {
            const std::string message = "Failed to parse exchange rates cache file for currency '" + currencyCode.toString() + "'";
            spdlog::critical(message);
            throw std::runtime_error(message);
        }

        currenciesExchangeRatesDatabank.insertAllExchangeRatesDataForCurrency(currencyCode, *parseResult.currencyCodeToCurrencyExchangeRateDataMapping_);
    }
}
