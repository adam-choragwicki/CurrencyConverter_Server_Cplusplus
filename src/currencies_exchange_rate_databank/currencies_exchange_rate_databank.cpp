#include "currencies_exchange_rate_databank.h"
#include "json_processing/json_validator.h"
#include "json_processing/json_parser.h"
#include "utilities/files_helper.h"
#include "spdlog/spdlog.h"
#include "types/currency_code.h"
#include "types/currency_exchange_rates_json.h"
#include "json_processing/exceptions.h"
#include "config/config.h"

CurrenciesExchangeRateDatabank::CurrenciesExchangeRateDatabank(const std::string& currenciesListFilepath) : currenciesListFileContent_(loadCurrenciesListFileContent(currenciesListFilepath))
{
    if(!alreadyCreated_)
    {
        //TODO parse only once
        if(JsonValidator::isValidJsonString(currenciesListFileContent_))
        {
            try
            {
                currenciesCodes_ = JsonParser::parseCurrenciesListFileContentToCurrenciesCodes(currenciesListFileContent_);
                loadCurrenciesExchangeRatesCacheFromFiles(currenciesCodes_, Paths::CurrenciesDatabankConfig::CURRENCIES_EXCHANGE_RATE_CACHE_DIRECTORY_PATH);
            }
            catch(JsonParseError& jsonParseError)
            {
                spdlog::critical("Error while processing content of '{}', {}", currenciesListFilepath, jsonParseError.what());
                exit(1);
            }
        }
        else
        {
            spdlog::critical(currenciesListFilepath + " is not a valid JSON string");
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

CurrenciesExchangeRateDatabank::~CurrenciesExchangeRateDatabank()
{
    alreadyCreated_ = false;
}

const std::string& CurrenciesExchangeRateDatabank::loadCurrenciesListFileContent(const std::string& currenciesListFilepath)
{
    if(FilesHelper::fileExists(currenciesListFilepath))
    {
        static const std::string fileContent = FilesHelper::loadFileContent(currenciesListFilepath);
        return fileContent;
    }
    else
    {
        spdlog::critical("File '" + currenciesListFilepath + "' does not exist");
        exit(1);
    }
}

void CurrenciesExchangeRateDatabank::loadCurrenciesExchangeRatesCacheFromFiles(const std::set<CurrencyCode>& currenciesCodes, const std::string& directoryPath)
{
    spdlog::debug("Loading currencies exchange rates from files");

    for(const CurrencyCode& currencyCode : currenciesCodes)
    {
        const std::string filePath = directoryPath + currencyCode.toString() + ".json";

        if(FilesHelper::fileExists(filePath))
        {
            const CurrencyExchangeRatesJson currencyExchangeRatesJson = CurrencyExchangeRatesJson(FilesHelper::loadFileContent(filePath));

            if(JsonValidator::isValidJsonString(currencyExchangeRatesJson.toString()))
            {
                ParseResult parseResult = JsonParser::parseExchangeRatesJsonStringToCurrencyCodesToExchangeRateDataMapping(currencyCode, currenciesCodes_, currencyExchangeRatesJson, true);
                const CurrencyCodeToCurrencyExchangeRateDataMapping currencyCodeToExchangeRateDataMap = *parseResult.currencyCodeToCurrencyExchangeRateDataMapping_;

                for(const auto&[currencyCode2, exchangeRateData] : currencyCodeToExchangeRateDataMap)
                {
                    if(!exchangeRateData.isNull())
                    {
                        currenciesExchangeRatesCache_.insert_or_assign(currencyCode, currencyCodeToExchangeRateDataMap);
                    }
                    else
                    {
                        spdlog::error("Wrong new exchange rate data, previous exchange rate will be kept for consistency");
                    }
                }
            }
            else
            {
                spdlog::critical("Error while loading currencies exchange rates cache.\n File '" + filePath + "' is not a valid JSON string");
                exit(1);
            }
        }
        else
        {
            spdlog::critical("Error while loading currencies exchange rates cache.\n File '" + filePath + "' does not exist");
            exit(1);
        }
    }

    spdlog::debug("Loading currencies exchange rates from files... DONE");
}

bool CurrenciesExchangeRateDatabank::containsExchangeRateData(const CurrencyCode& sourceCurrencyCode, const CurrencyCode& targetCurrencyCode) const
{
    return currenciesExchangeRatesCache_.contains(sourceCurrencyCode) && currenciesExchangeRatesCache_.at(sourceCurrencyCode).contains(targetCurrencyCode);
}

ExchangeRateData CurrenciesExchangeRateDatabank::getExchangeRateDataForCurrenciesPair(const CurrencyCode& sourceCurrencyCode, const CurrencyCode& targetCurrencyCode) const
{
    return currenciesExchangeRatesCache_.at(sourceCurrencyCode).at(targetCurrencyCode);
}

void CurrenciesExchangeRateDatabank::setAllExchangeRatesDataForCurrency(const CurrencyCode& sourceCurrency, const CurrencyCodeToCurrencyExchangeRateDataMapping& currencyCodeToCurrencyExchangeRateDataMapping)
{
    const auto&[_, inserted] = currenciesExchangeRatesCache_.insert_or_assign(sourceCurrency, currencyCodeToCurrencyExchangeRateDataMapping);

    if(inserted)
    {
        spdlog::error("Error, this should not insert");
    }
}
