#pragma once

#include "types/definitions.h"
#include "types/currency_code.h"
#include <set>

class CurrenciesExchangeRateDatabank
{
public:
    CurrenciesExchangeRateDatabank();

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

    void setCache(const CurrencyCodeToCurrencyExchangeRatesJsonMapping& currenciesCodesToExchangeRatesJsonsMapping);

private:
    static const std::string& loadCurrenciesListFileContent();
    CurrencyExchangeRatesJson loadFileForCurrency(const CurrencyCode& currencyCode);
    void loadCacheFromFiles();
    void loadCacheFromMap(const CurrencyCodeToCurrencyExchangeRatesJsonMapping& currenciesCodesToExchangeRatesJsons);

    const std::string currenciesListFileContent_;
    std::set<CurrencyCode> currenciesCodes_;
    CurrenciesRatesCache currenciesRatesCache_;

    inline static bool alreadyCreated_{};
};
