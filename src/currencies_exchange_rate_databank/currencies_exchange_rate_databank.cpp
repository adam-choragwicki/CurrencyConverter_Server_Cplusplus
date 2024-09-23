#include "currencies_exchange_rate_databank.h"
#include "json_processing/json_parser.h"
#include "types/currency_code.h"
#include "json_processing/exceptions.h"
#include "config/config.h"
#include "currencies_exchange_rate_databank_initializer.h"
#include "spdlog/spdlog.h"

CurrenciesExchangeRatesDatabank::CurrenciesExchangeRatesDatabank(const CurrenciesListFileContent& currenciesListFileContent)
{
    if(!alreadyCreated_)
    {
        try
        {
            currenciesCodes_ = JsonParser::parseCurrenciesListFileContentToCurrenciesCodes(currenciesListFileContent);

            //initializeCache
            CurrenciesExchangeRatesDatabankInitializer::loadCurrenciesExchangeRatesCacheFromFiles(*this, Paths::CurrenciesExchangeRatesDatabankConfig::CURRENCIES_EXCHANGE_RATE_CACHE_DIRECTORY_PATH);
        }
        catch(JsonParseError& jsonParseError)
        {
            spdlog::critical("Error while processing currencies list file content, {}", jsonParseError.what());
            exit(1);
        }

        alreadyCreated_ = true;
    }
    else
    {
        throw std::runtime_error("Error, trying to construct another instance of CurrenciesExchangeRatesDatabank");
    }

    spdlog::debug("Currencies exchange rate databank initialized");
}

CurrenciesExchangeRatesDatabank::~CurrenciesExchangeRatesDatabank()
{
    alreadyCreated_ = false;
}

bool CurrenciesExchangeRatesDatabank::containsExchangeRateData(const CurrencyCode& sourceCurrencyCode, const CurrencyCode& targetCurrencyCode) const
{
    return currenciesExchangeRatesCache_.contains(sourceCurrencyCode) && currenciesExchangeRatesCache_.at(sourceCurrencyCode).contains(targetCurrencyCode);
}

ExchangeRateData CurrenciesExchangeRatesDatabank::getExchangeRateDataForCurrenciesPair(const CurrencyCode& sourceCurrencyCode, const CurrencyCode& targetCurrencyCode) const
{
    return currenciesExchangeRatesCache_.at(sourceCurrencyCode).at(targetCurrencyCode);
}

void CurrenciesExchangeRatesDatabank::insertAllExchangeRatesDataForCurrency(const CurrencyCode& sourceCurrency, const CurrencyCodeToCurrencyExchangeRateDataMapping& currencyCodeToCurrencyExchangeRateDataMapping)
{
    const auto&[_, inserted] = currenciesExchangeRatesCache_.insert_or_assign(sourceCurrency, currencyCodeToCurrencyExchangeRateDataMapping);

    if(!inserted)
    {
        throw std::runtime_error("Error, exchange rate data for currency '" + sourceCurrency.toString() + "' already exists");
    }
}

void CurrenciesExchangeRatesDatabank::reassignAllExchangeRatesDataForCurrency(const CurrencyCode& sourceCurrency, const CurrencyCodeToCurrencyExchangeRateDataMapping& currencyCodeToCurrencyExchangeRateDataMapping)
{
    const auto&[_, inserted] = currenciesExchangeRatesCache_.insert_or_assign(sourceCurrency, currencyCodeToCurrencyExchangeRateDataMapping);

    if(inserted)
    {
        throw std::runtime_error("Error, exchange rate data for currency '" + sourceCurrency.toString() + "' does not exist");
    }
}
