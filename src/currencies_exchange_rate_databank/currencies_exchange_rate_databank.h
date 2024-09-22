#pragma once

#include "types/definitions.h"
#include <set>

class CurrencyCode;

class CurrenciesExchangeRateDatabank
{
public:
    explicit CurrenciesExchangeRateDatabank(const std::string& currenciesListFilepath);
    ~CurrenciesExchangeRateDatabank();

    CurrenciesExchangeRateDatabank(const CurrenciesExchangeRateDatabank&) = delete;
    CurrenciesExchangeRateDatabank(CurrenciesExchangeRateDatabank&&) = delete;
    CurrenciesExchangeRateDatabank& operator=(const CurrenciesExchangeRateDatabank&) = delete;
    CurrenciesExchangeRateDatabank& operator=(CurrenciesExchangeRateDatabank&&) = delete;

    [[nodiscard]] bool containsExchangeRateData(const CurrencyCode& sourceCurrencyCode, const CurrencyCode& targetCurrencyCode) const;
    [[nodiscard]] ExchangeRateData getExchangeRateDataForCurrenciesPair(const CurrencyCode& sourceCurrencyCode, const CurrencyCode& targetCurrencyCode) const;

    [[nodiscard]] const std::string& getCurrenciesListFileContent() const
    { return currenciesListFileContent_; }

    [[nodiscard]] const std::set<CurrencyCode>& getCurrenciesCodes() const
    { return currenciesCodes_; }

    void insertAllExchangeRatesDataForCurrency(const CurrencyCode& sourceCurrency, const CurrencyCodeToCurrencyExchangeRateDataMapping& currencyCodeToCurrencyExchangeRateDataMapping);

    void updateAllExchangeRatesDataForCurrency(const CurrencyCode& sourceCurrency, const CurrencyCodeToCurrencyExchangeRateDataMapping& currencyCodeToCurrencyExchangeRateDataMapping);

    [[nodiscard]] size_t size() const
    { return currenciesExchangeRatesCache_.size(); }

private:
    static const std::string& loadCurrenciesListFileContent(const std::string& currenciesListFilepath);

    const std::string currenciesListFileContent_;
    std::set<CurrencyCode> currenciesCodes_;
    CurrenciesExchangeRatesCache currenciesExchangeRatesCache_;

    inline static bool alreadyCreated_{};
};
