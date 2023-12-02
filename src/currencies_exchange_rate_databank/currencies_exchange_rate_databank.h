#pragma once

#include "types/definitions.h"
#include <set>

class CurrencyCode;
class CurrenciesListFileContent;

class CurrenciesExchangeRatesDatabank
{
public:
    explicit CurrenciesExchangeRatesDatabank(const CurrenciesListFileContent& currenciesListFileContent);
    ~CurrenciesExchangeRatesDatabank();

    CurrenciesExchangeRatesDatabank(const CurrenciesExchangeRatesDatabank&) = delete;
    CurrenciesExchangeRatesDatabank(CurrenciesExchangeRatesDatabank&&) = delete;
    CurrenciesExchangeRatesDatabank& operator=(const CurrenciesExchangeRatesDatabank&) = delete;
    CurrenciesExchangeRatesDatabank& operator=(CurrenciesExchangeRatesDatabank&&) = delete;

    [[nodiscard]] bool containsExchangeRateData(const CurrencyCode& sourceCurrencyCode, const CurrencyCode& targetCurrencyCode) const;
    [[nodiscard]] ExchangeRateData getExchangeRateDataForCurrenciesPair(const CurrencyCode& sourceCurrencyCode, const CurrencyCode& targetCurrencyCode) const;

    [[nodiscard]] const std::set<CurrencyCode>& getCurrenciesCodes() const
    { return currenciesCodes_; }

    void insertAllExchangeRatesDataForCurrency(const CurrencyCode& sourceCurrency, const CurrencyCodeToCurrencyExchangeRateDataMapping& currencyCodeToCurrencyExchangeRateDataMapping);

    void reassignAllExchangeRatesDataForCurrency(const CurrencyCode& sourceCurrency, const CurrencyCodeToCurrencyExchangeRateDataMapping& currencyCodeToCurrencyExchangeRateDataMapping);

    [[nodiscard]] size_t size() const
    { return currenciesExchangeRatesCache_.size(); }

private:
    std::set<CurrencyCode> currenciesCodes_;

    std::map<CurrencyCode, CurrencyCodeToCurrencyExchangeRateDataMapping> currenciesExchangeRatesCache_;

    inline static bool alreadyCreated_{};
};
