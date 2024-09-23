#include "common_test_fixture.h"
#include "currencies_exchange_rate_databank/currencies_exchange_rate_databank.h"
#include "types/currency_code.h"

class CurrenciesExchangeRateDatabankTest : public CommonTestFixture
{
public:
    CurrenciesExchangeRateDatabankTest() = default;

protected:
    CurrenciesExchangeRateDatabank currenciesDatabank_{"data/currencies_list.json"};
};

TEST_F(CurrenciesExchangeRateDatabankTest, CurrenciesCount)
{
    EXPECT_EQ(currenciesDatabank_.getCurrenciesCodes().size(), 3);
}

TEST_F(CurrenciesExchangeRateDatabankTest, AllCombinationsOfExchangeRatesArePresent)
{
    EXPECT_TRUE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("eur"), CurrencyCode("gbp")));
    EXPECT_TRUE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("eur"), CurrencyCode("usd")));

    EXPECT_TRUE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("gbp"), CurrencyCode("eur")));
    EXPECT_TRUE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("gbp"), CurrencyCode("usd")));

    EXPECT_TRUE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("usd"), CurrencyCode("eur")));
    EXPECT_TRUE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("usd"), CurrencyCode("gbp")));
}

TEST_F(CurrenciesExchangeRateDatabankTest, XToXExchangeRatesAreNotPresent)
{
    EXPECT_FALSE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("eur"), CurrencyCode("eur")));
    EXPECT_FALSE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("gbp"), CurrencyCode("gbp")));
    EXPECT_FALSE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("usd"), CurrencyCode("usd")));
}
