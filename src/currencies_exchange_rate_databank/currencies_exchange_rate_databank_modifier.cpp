#include "currencies_exchange_rate_databank_modifier.h"
#include "currencies_exchange_rate_databank.h"
#include "types/currency_code.h"
#include <utility>

void CurrenciesExchangeRatesDatabankModifier::modifyCurrenciesExchangeRatesDatabank(CurrenciesExchangeRatesDatabank& currenciesExchangeRatesDatabank, const std::map<CurrencyCode, ParseResult>& currencyCodeToParseResultMapping)
{
    CurrenciesExchangeRatesDatabank::ExchangeRatesCache updatedCache = currenciesExchangeRatesDatabank.snapshotExchangeRatesCache(); // rare expensive copy

    for (const auto& [currencyCode, parseResult]: currencyCodeToParseResultMapping)
    {
        if (!parseResult.isSuccess_ || !parseResult.currencyCodeToCurrencyExchangeRateDataMapping_)
        {
            continue;
        }

        auto existingIt = updatedCache.find(currencyCode);

        if (existingIt == updatedCache.end())
        {
            continue;
        }

        auto& existingMapping = existingIt->second;
        const auto& newMapping = *parseResult.currencyCodeToCurrencyExchangeRateDataMapping_;

        if (newMapping.empty())
        {
            continue;
        }

        for (const auto& [targetCurrencyCode, exchangeRateData]: newMapping)
        {
            existingMapping.insert_or_assign(targetCurrencyCode, exchangeRateData);
        }
    }

    currenciesExchangeRatesDatabank.replaceExchangeRatesCache(std::move(updatedCache));
}
