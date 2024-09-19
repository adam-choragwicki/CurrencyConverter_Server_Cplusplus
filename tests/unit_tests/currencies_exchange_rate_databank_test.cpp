#include "common_test_fixture.h"
#include "currencies_exchange_rate_databank/currencies_exchange_rate_databank.h"

class CurrenciesDatabankTest : public CommonTestFixture
{
public:
    CurrenciesDatabankTest() = default;

    [[nodiscard]] const std::set<CurrencyCode>& getCurrenciesCodes() const
    {
        return currenciesDatabank_.getCurrenciesCodes();
    }

protected:
    CurrenciesExchangeRateDatabank currenciesDatabank_;
};

TEST_F(CurrenciesDatabankTest, CurrenciesCount)
{
    EXPECT_EQ(getCurrenciesCodes().size(), 147);
}

TEST_F(CurrenciesDatabankTest, AllCombinationsOfExchangeRatesArePresent)
{
    for(const CurrencyCode& sourceCurrencyCode : getCurrenciesCodes())
    {
        for(const CurrencyCode& targetCurrencyCode : getCurrenciesCodes())
        {
            if(sourceCurrencyCode != targetCurrencyCode)
            {
                EXPECT_TRUE(currenciesDatabank_.containsExchangeRateData(sourceCurrencyCode, targetCurrencyCode));
            }
        }
    }
}
