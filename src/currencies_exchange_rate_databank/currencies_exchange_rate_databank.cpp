#include "currencies_exchange_rate_databank.h"
#include "json_processing/json_parser.h"
#include "utilities/file_loader.h"
#include "config.h"
#include "spdlog/spdlog.h"
#include "types/currency_code.h"
#include "types/currency_exchange_rates_json.h"
#include "json_processing/exceptions.h"

CurrenciesExchangeRateDatabank::CurrenciesExchangeRateDatabank() : currenciesListFileContent_(loadCurrenciesListFileContent())
{
    if(!alreadyCreated_)
    {
        //TODO parse only once
        if(JsonParser::isValidJsonString(currenciesListFileContent_))
        {
            try
            {
                currenciesCodes_ = JsonParser::parseJsonToCurrenciesCodes(currenciesListFileContent_);
                loadCacheFromFiles();
            }
            catch(JsonParseError& jsonParseError)
            {
                spdlog::critical("Error while processing content of '{}', {}", CurrenciesDatabankConfig::CURRENCIES_LIST_FILE_PATH, jsonParseError.what());
                exit(1);
            }
        }
        else
        {
            spdlog::critical(CurrenciesDatabankConfig::CURRENCIES_LIST_FILE_PATH + " is not a valid JSON string");
            exit(1);
        }

        alreadyCreated_ = true;
    }
    else
    {
        throw std::runtime_error("Error, trying to construct another instance of CurrenciesExchangeRateDatabank");
    }

    spdlog::debug("Currencies exchange rate databank initialized");
}

const std::string& CurrenciesExchangeRateDatabank::loadCurrenciesListFileContent()
{
    if(FileLoader::fileExists(CurrenciesDatabankConfig::CURRENCIES_LIST_FILE_PATH))
    {
        static const std::string fileContent = FileLoader::loadFileContent(CurrenciesDatabankConfig::CURRENCIES_LIST_FILE_PATH);
        return fileContent;
    }
    else
    {
        spdlog::critical(CurrenciesDatabankConfig::CURRENCIES_LIST_FILE_PATH + " does not exist");
        exit(1);
    }
}

void CurrenciesExchangeRateDatabank::loadCacheFromFiles()
{
    spdlog::debug("Loading currencies exchange rates from files");

    CurrenciesRatesCache currenciesRatesCache;

    for(const CurrencyCode& currencyCode : currenciesCodes_)
    {
        const CurrencyExchangeRatesJson currencyExchangeRatesJson = loadFileForCurrency(currencyCode);
        currenciesRatesCache.insert_or_assign(currencyCode, JsonParser::extractAllExchangeRatesDataFromCurrencyExchangeRatesJsonString(currencyCode, currenciesCodes_, currencyExchangeRatesJson));
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
            currenciesRatesCache.insert_or_assign(currencyCode, JsonParser::extractAllExchangeRatesDataFromCurrencyExchangeRatesJsonString(currencyCode, currenciesCodes_, currencyExchangeRatesJson));
        }
        else
        {
            spdlog::warn("Currency exchange rates JSON for " + currencyCode.toUpperCase() + " is empty");
        }
    }

    currenciesRatesCache_ = currenciesRatesCache;

    spdlog::info("Loading currencies exchange rates from map... DONE");
}

bool CurrenciesExchangeRateDatabank::containsExchangeRateData(const CurrencyCode& sourceCurrencyCode, const CurrencyCode& targetCurrencyCode) const
{
    return currenciesRatesCache_.contains(sourceCurrencyCode) && currenciesRatesCache_.at(sourceCurrencyCode).contains(targetCurrencyCode);
}

ExchangeRateData CurrenciesExchangeRateDatabank::getExchangeRateDataForCurrenciesPair(const CurrencyCode& sourceCurrencyCode, const CurrencyCode& targetCurrencyCode) const
{
    return currenciesRatesCache_.at(sourceCurrencyCode).at(targetCurrencyCode);
}

void CurrenciesExchangeRateDatabank::setCache(const CurrencyCodeToCurrencyExchangeRatesJsonMapping& currenciesCodesToExchangeRatesJsonsMapping)
{
    loadCacheFromMap(currenciesCodesToExchangeRatesJsonsMapping);
}

CurrencyExchangeRatesJson CurrenciesExchangeRateDatabank::loadFileForCurrency(const CurrencyCode& currencyCode)
{
    CurrencyExchangeRatesJson currencyExchangeRatesJson = CurrencyExchangeRatesJson(FileLoader::loadFileContent(CurrenciesDatabankConfig::CURRENCIES_EXCHANGE_RATE_CACHE_DIRECTORY_PATH + currencyCode.toString() + ".json"));
    return currencyExchangeRatesJson;
}
