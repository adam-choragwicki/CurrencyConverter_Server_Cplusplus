#include "currencies_exchange_rate_databank.h"
#include "json_processing/json_parser.h"
#include "utilities/file_loader.h"
#include "config.h"
#include "spdlog/spdlog.h"
#include "types/currency_code.h"
#include "types/currency_exchange_rates_json.h"
#include "types/exchange_rate.h"

CurrenciesExchangeRateDatabank::CurrenciesExchangeRateDatabank() : currenciesFileContent_(FileLoader::loadFileContent(CurrenciesDatabankConfig::CURRENCIES_DATA_FILE_PATH))
{
    if(!alreadyCreated_)
    {
        currenciesCodes_ = JsonParser::parseJsonToCurrenciesCodes(currenciesFileContent_);
        loadCacheFromFiles();
    }
    else
    {
        throw std::runtime_error("Error, trying to construct another instance of CurrenciesExchangeRateDatabank");
    }

    spdlog::debug("Currencies exchange rate databank initialized");
}

void CurrenciesExchangeRateDatabank::loadCacheFromFiles()
{
    spdlog::debug("Loading currencies exchange rates from files");

    CurrenciesRatesCache currenciesRatesCache;

    currenciesExchangeRatesTimestamp_ = JsonParser::parseTimestamp(CurrencyCode("eur"),
                                                                   CurrencyExchangeRatesJson(FileLoader::loadFileContent(CurrenciesDatabankConfig::CURRENCIES_EXCHANGE_RATE_CACHE_DIRECTORY_PATH + CurrencyCode("usd").toString() + ".json")));

    spdlog::debug("Exchange rates timestamp: " + currenciesExchangeRatesTimestamp_.toString());

    for(const CurrencyCode& currencyCode : currenciesCodes_)
    {
        const CurrencyExchangeRatesJson currencyExchangeRatesJson = CurrencyExchangeRatesJson(FileLoader::loadFileContent(CurrenciesDatabankConfig::CURRENCIES_EXCHANGE_RATE_CACHE_DIRECTORY_PATH + currencyCode.toString() + ".json"));
        currenciesRatesCache.insert_or_assign(currencyCode, JsonParser::extractAllExchangeRatesFromCurrencyExchangeRatesJsonString(currencyCode, currenciesCodes_, currencyExchangeRatesJson));
    }

    currenciesRatesCache_ = currenciesRatesCache;

    spdlog::debug("Loading currencies exchange rates from files... DONE");
}

void CurrenciesExchangeRateDatabank::loadCacheFromMap(const CurrencyCodeToCurrencyExchangeRatesJsonMapping& currenciesCodesToExchangeRatesJsons)
{
    spdlog::info("Loading currencies exchange rates from map");

    CurrenciesRatesCache currenciesRatesCache;

    for(const CurrencyCode& currencyCode : currenciesCodes_)
    {
        const CurrencyExchangeRatesJson& currencyExchangeRatesJson = currenciesCodesToExchangeRatesJsons.at(currencyCode);

        if(!currencyExchangeRatesJson.toString().empty())
        {
            currenciesRatesCache.insert_or_assign(currencyCode, JsonParser::extractAllExchangeRatesFromCurrencyExchangeRatesJsonString(currencyCode, currenciesCodes_, currencyExchangeRatesJson));
        }
        else
        {
            spdlog::warn("Currency exchange rates JSON for " + currencyCode.toUpperCase() + " is empty");
        }
    }

    currenciesRatesCache_ = currenciesRatesCache;

    spdlog::info("Loading currencies exchange rates from map... DONE");
}

bool CurrenciesExchangeRateDatabank::containsExchangeRate(const CurrencyCode& sourceCurrencyCode, const CurrencyCode& targetCurrencyCode) const
{
    return currenciesRatesCache_.contains(sourceCurrencyCode) && currenciesRatesCache_.at(sourceCurrencyCode).contains(targetCurrencyCode);
}

ExchangeRate CurrenciesExchangeRateDatabank::getExchangeRate(const CurrencyCode& sourceCurrencyCode, const CurrencyCode& targetCurrencyCode) const
{
    return currenciesRatesCache_.at(sourceCurrencyCode).at(targetCurrencyCode);
}

void CurrenciesExchangeRateDatabank::setCache(const CurrencyCodeToCurrencyExchangeRatesJsonMapping& currenciesCodesToExchangeRatesJsonsMapping)
{
    loadCacheFromMap(currenciesCodesToExchangeRatesJsonsMapping);
}
