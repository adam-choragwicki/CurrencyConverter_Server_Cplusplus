#include "currencies_exchange_rate_databank.h"
#include "json_processing/json_parser.h"
#include "utilities/files_helper.h"
#include "types/currency_code.h"
#include "json_processing/exceptions.h"
#include "config/config.h"
#include "currencies_exchange_rate_databank_initializer.h"
#include "spdlog/spdlog.h"

CurrenciesExchangeRateDatabank::CurrenciesExchangeRateDatabank(const std::string& currenciesListFilepath) : currenciesListFileContent_(loadCurrenciesListFileContent(currenciesListFilepath))
{
    if(!alreadyCreated_)
    {
        try
        {
            currenciesCodes_ = JsonParser::parseCurrenciesListFileContentToCurrenciesCodes(currenciesListFileContent_);

            //initializeCache
            CurrenciesExchangeRateDatabankInitializer::loadCurrenciesExchangeRatesCacheFromFiles(*this, Paths::CurrenciesDatabankConfig::CURRENCIES_EXCHANGE_RATE_CACHE_DIRECTORY_PATH);
        }
        catch(JsonParseError& jsonParseError)
        {
            spdlog::critical("Error while processing content of '{}', {}", currenciesListFilepath, jsonParseError.what());
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
        spdlog::info("Loading '{}'", currenciesListFilepath);
        return fileContent;
    }
    else
    {
        spdlog::critical("File '" + currenciesListFilepath + "' does not exist");
        exit(1);
    }
}

bool CurrenciesExchangeRateDatabank::containsExchangeRateData(const CurrencyCode& sourceCurrencyCode, const CurrencyCode& targetCurrencyCode) const
{
    return currenciesExchangeRatesCache_.contains(sourceCurrencyCode) && currenciesExchangeRatesCache_.at(sourceCurrencyCode).contains(targetCurrencyCode);
}

ExchangeRateData CurrenciesExchangeRateDatabank::getExchangeRateDataForCurrenciesPair(const CurrencyCode& sourceCurrencyCode, const CurrencyCode& targetCurrencyCode) const
{
    return currenciesExchangeRatesCache_.at(sourceCurrencyCode).at(targetCurrencyCode);
}

void CurrenciesExchangeRateDatabank::insertAllExchangeRatesDataForCurrency(const CurrencyCode& sourceCurrency, const CurrencyCodeToCurrencyExchangeRateDataMapping& currencyCodeToCurrencyExchangeRateDataMapping)
{
    const auto&[_, inserted] = currenciesExchangeRatesCache_.insert_or_assign(sourceCurrency, currencyCodeToCurrencyExchangeRateDataMapping);

    if(!inserted)
    {
        spdlog::error("Error, this should insert, not assign");
    }
}

void CurrenciesExchangeRateDatabank::reAssignAllExchangeRatesDataForCurrency(const CurrencyCode& sourceCurrency, const CurrencyCodeToCurrencyExchangeRateDataMapping& currencyCodeToCurrencyExchangeRateDataMapping)
{
    const auto&[_, inserted] = currenciesExchangeRatesCache_.insert_or_assign(sourceCurrency, currencyCodeToCurrencyExchangeRateDataMapping);

    if(inserted)
    {
        spdlog::error("Error, this should not insert");
    }
}
