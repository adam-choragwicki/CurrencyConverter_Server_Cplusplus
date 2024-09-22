#include "common_test_fixture.h"
#include "currencies_exchange_rate_databank/currencies_exchange_rate_databank.h"
#include "types/currency_code.h"

class CurrenciesDatabankTest : public CommonTestFixture
{
public:
    CurrenciesDatabankTest() = default;

protected:
    CurrenciesExchangeRateDatabank currenciesDatabank_{"data/currencies_list.json"};
};

TEST_F(CurrenciesDatabankTest, CurrenciesCount)
{
    EXPECT_EQ(currenciesDatabank_.getCurrenciesCodes().size(), 4);
}

TEST_F(CurrenciesDatabankTest, AllCombinationsOfExchangeRatesArePresent)
{
    const auto sourceCurrencyCodes = currenciesDatabank_.getCurrenciesCodes();
    const auto targetCurrencyCodes = currenciesDatabank_.getCurrenciesCodes();

    for(const CurrencyCode& sourceCurrencyCode : sourceCurrencyCodes)
    {
        for(const CurrencyCode& targetCurrencyCode : targetCurrencyCodes)
        {
            if(sourceCurrencyCode != targetCurrencyCode)
            {
                EXPECT_TRUE(currenciesDatabank_.containsExchangeRateData(sourceCurrencyCode, targetCurrencyCode));
            }
        }
    }
}
