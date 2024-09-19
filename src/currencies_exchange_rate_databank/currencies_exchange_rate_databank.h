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
    [[nodiscard]] ExchangeRate getExchangeRateForCurrenciesPair(const CurrencyCode& sourceCurrencyCode, const CurrencyCode& targetCurrencyCode) const;

    //    [[nodiscard]] const Timestamp& getCurrenciesExchangeRateTimestamp() const
    //    { return currenciesExchangeRatesTimestamp_; }
    //
    //    void setExchangeRatesTimestamp(const Timestamp& currenciesExchangeRatesTimestamp)
    //    { currenciesExchangeRatesTimestamp_ = currenciesExchangeRatesTimestamp; }

    [[nodiscard]] const Timestamp& getCurrenciesExchangeRateTimestamp() const
    { return *(new Timestamp("0"));}

    void setExchangeRatesTimestamp(const Timestamp& currenciesExchangeRatesTimestamp)
    { currenciesExchangeRatesTimestamp_ = currenciesExchangeRatesTimestamp; }

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
    Timestamp currenciesExchangeRatesTimestamp_;
    std::set<CurrencyCode> currenciesCodes_;
    CurrenciesRatesCache currenciesRatesCache_;

    bool alreadyCreated_{};
};
