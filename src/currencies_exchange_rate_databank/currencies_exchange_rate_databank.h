#pragma once

#include "types/definitions.h"
#include <set>
#include <shared_mutex>
#include <optional>

class CurrencyCode;
class CurrenciesNamesAndCodesFileContent;

class CurrenciesExchangeRatesDatabank
{
public:
    using ExchangeRatesCache = std::map<CurrencyCode, CurrencyCodeToCurrencyExchangeRateDataMapping>;

    explicit CurrenciesExchangeRatesDatabank(const CurrenciesNamesAndCodesFileContent& currenciesNamesAndCodesFileContent);

    CurrenciesExchangeRatesDatabank(const CurrenciesExchangeRatesDatabank&) = delete;
    CurrenciesExchangeRatesDatabank(CurrenciesExchangeRatesDatabank&&) = delete;
    CurrenciesExchangeRatesDatabank& operator=(const CurrenciesExchangeRatesDatabank&) = delete;
    CurrenciesExchangeRatesDatabank& operator=(CurrenciesExchangeRatesDatabank&&) = delete;

    std::optional<ExchangeRateData> tryGetExchangeRateDataForCurrenciesPair(const CurrencyCode& sourceCurrencyCode, const CurrencyCode& targetCurrencyCode) const;

    [[nodiscard]] const std::set<CurrencyCode>& getCurrenciesCodes() const { return currenciesCodes_; }

    void insertAllExchangeRatesDataForCurrency(const CurrencyCode& sourceCurrency, const CurrencyCodeToCurrencyExchangeRateDataMapping& currencyCodeToCurrencyExchangeRateDataMapping);

    void reassignAllExchangeRatesDataForCurrency(const CurrencyCode& sourceCurrency, const CurrencyCodeToCurrencyExchangeRateDataMapping& currencyCodeToCurrencyExchangeRateDataMapping);

    [[nodiscard]] ExchangeRatesCache snapshotExchangeRatesCache() const;
    void replaceExchangeRatesCache(ExchangeRatesCache newCache);

    [[nodiscard]] size_t size() const;

private:
    std::set<CurrencyCode> currenciesCodes_;

    ExchangeRatesCache currenciesExchangeRatesCache_;
    mutable std::shared_mutex cacheMutex_;
};
