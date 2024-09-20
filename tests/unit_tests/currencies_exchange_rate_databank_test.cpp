#include "common_test_fixture.h"
#include "currencies_exchange_rate_databank/currencies_exchange_rate_databank.h"

class CurrenciesDatabankTest : public CommonTestFixture
{
public:
    CurrenciesDatabankTest() = default;

protected:
    CurrenciesExchangeRateDatabank currenciesDatabank_;
};

TEST_F(CurrenciesDatabankTest, CurrenciesCount)
{
    EXPECT_EQ(currenciesDatabank_.getCurrenciesCodes().size(), 147);
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
