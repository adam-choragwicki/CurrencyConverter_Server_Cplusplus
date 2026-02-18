#include "currencies_exchange_rate_databank.h"
#include "json_processing/json_parser.h"
#include "types/currency_code.h"
#include "json_processing/exceptions.h"
#include "paths.h"
#include "currencies_exchange_rate_databank_initializer.h"
#include "spdlog/spdlog.h"

CurrenciesExchangeRatesDatabank::CurrenciesExchangeRatesDatabank(const CurrenciesNamesAndCodesFileContent& currenciesNamesAndCodesFileContent)
{
    try
    {
        currenciesCodes_ = JsonParser::parseCurrenciesNamesAndCodesFileToCurrenciesCodes(currenciesNamesAndCodesFileContent);

        //initializeCache
        CurrenciesExchangeRatesDatabankInitializer::loadCurrenciesExchangeRatesCacheFromFiles(*this, Paths::CURRENCIES_EXCHANGE_RATE_CACHE_DIRECTORY_PATH);
    }
    catch (JsonParseError& jsonParseError)
    {
        throw std::runtime_error(std::format("Error while parsing currencies list file content: {}", jsonParseError.what()));
    }
}

std::optional<ExchangeRateData> CurrenciesExchangeRatesDatabank::tryGetExchangeRateDataForCurrenciesPair(const CurrencyCode& sourceCurrencyCode, const CurrencyCode& targetCurrencyCode) const
{
    // combine lookup with retrieval under a single lock to avoid TOCTOU
    std::shared_lock lock(cacheMutex_);

    if (sourceCurrencyCode == targetCurrencyCode)
    {
        return std::nullopt;
    }

    const auto sourceIt = currenciesExchangeRatesCache_.find(sourceCurrencyCode);

    if (sourceIt == currenciesExchangeRatesCache_.end())
    {
        return std::nullopt;
    }

    const auto targetIt = sourceIt->second.find(targetCurrencyCode);

    if (targetIt == sourceIt->second.end())
    {
        return std::nullopt;
    }

    return targetIt->second;
}

void CurrenciesExchangeRatesDatabank::insertAllExchangeRatesDataForCurrency(const CurrencyCode& sourceCurrency, const CurrencyCodeToCurrencyExchangeRateDataMapping& currencyCodeToCurrencyExchangeRateDataMapping)
{
    std::unique_lock lock(cacheMutex_);

    auto [_, inserted] = currenciesExchangeRatesCache_.try_emplace(sourceCurrency, currencyCodeToCurrencyExchangeRateDataMapping);

    if (!inserted)
    {
        throw std::runtime_error("Error, exchange rate data for currency '" + sourceCurrency.toString() + "' already exists");
    }
}

void CurrenciesExchangeRatesDatabank::reassignAllExchangeRatesDataForCurrency(const CurrencyCode& sourceCurrency, const CurrencyCodeToCurrencyExchangeRateDataMapping& currencyCodeToCurrencyExchangeRateDataMapping)
{
    std::unique_lock lock(cacheMutex_);

    auto it = currenciesExchangeRatesCache_.find(sourceCurrency);

    if (it == currenciesExchangeRatesCache_.end())
    {
        throw std::runtime_error("Error, exchange rate data for currency '" + sourceCurrency.toString() + "' does not exist");
    }

    it->second = currencyCodeToCurrencyExchangeRateDataMapping;
}

CurrenciesExchangeRatesDatabank::ExchangeRatesCache CurrenciesExchangeRatesDatabank::snapshotExchangeRatesCache() const
{
    std::shared_lock lock(cacheMutex_);
    return currenciesExchangeRatesCache_;
}

void CurrenciesExchangeRatesDatabank::replaceExchangeRatesCache(ExchangeRatesCache newCache)
{
    std::unique_lock lock(cacheMutex_);
    currenciesExchangeRatesCache_.swap(newCache);
}

size_t CurrenciesExchangeRatesDatabank::size() const
{
    std::shared_lock lock(cacheMutex_);
    return currenciesExchangeRatesCache_.size();
}
