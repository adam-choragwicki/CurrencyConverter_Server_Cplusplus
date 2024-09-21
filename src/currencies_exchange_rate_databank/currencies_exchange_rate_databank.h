#pragma once

#include "types/definitions.h"
#include "types/currency_code.h"
#include <set>

class CurrenciesExchangeRateDatabank
{
public:
    CurrenciesExchangeRateDatabank();
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

    void loadCurrenciesExchangeRatesCacheFromFiles(const std::set<CurrencyCode>& currenciesCodes, const std::string& directoryPath);

    void setAllExchangeRatesDataForCurrency(const CurrencyCode& sourceCurrency, const CurrencyCodeToCurrencyExchangeRateDataMapping& currencyCodeToCurrencyExchangeRateDataMapping);

private:
    static const std::string& loadCurrenciesListFileContent();

    const std::string currenciesListFileContent_;
    std::set<CurrencyCode> currenciesCodes_;
    CurrenciesExchangeRatesCache currenciesExchangeRatesCache_;

    inline static bool alreadyCreated_{};
};
