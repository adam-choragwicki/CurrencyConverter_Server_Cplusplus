#pragma once

#include "types/timestamp.h"
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

    [[nodiscard]] bool containsExchangeRate(const CurrencyCode& sourceCurrencyCode, const CurrencyCode& targetCurrencyCode) const;
    [[nodiscard]] ExchangeRate getExchangeRate(const CurrencyCode& sourceCurrencyCode, const CurrencyCode& targetCurrencyCode) const;

    [[nodiscard]] const Timestamp& getCurrenciesExchangeRateTimestamp() const
    { return currenciesExchangeRatesTimestamp_; }

    void setExchangeRatesTimestamp(const Timestamp& currenciesExchangeRatesTimestamp)
    { currenciesExchangeRatesTimestamp_ = currenciesExchangeRatesTimestamp; }

    [[nodiscard]] const std::string& getCurrenciesFileContent() const
    { return currenciesFileContent_; }

    [[nodiscard]] const std::set<CurrencyCode>& getCurrenciesCodes() const
    { return currenciesCodes_; }

    void setCache(const CurrencyCodeToCurrencyExchangeRatesJsonMapping& currenciesCodesToExchangeRatesJsonsMapping);

private:
    static const std::string& loadCurrenciesFileContent();
    void loadCacheFromFiles();
    void loadCacheFromMap(const CurrencyCodeToCurrencyExchangeRatesJsonMapping& currenciesCodesToExchangeRatesJsons);

    const std::string currenciesFileContent_;
    Timestamp currenciesExchangeRatesTimestamp_;
    std::set<CurrencyCode> currenciesCodes_;
    CurrenciesRatesCache currenciesRatesCache_;

    bool alreadyCreated_{};
};
