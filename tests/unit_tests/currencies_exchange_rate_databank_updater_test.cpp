#include "common_test_fixture.h"
#include "types/currency_code.h"
#include "currencies_exchange_rate_databank/currencies_exchange_rate_databank.h"
#include "currencies_exchange_rate_databank/currencies_exchange_rate_databank_updater.h"


class CurrenciesExchangeRateDatabankUpdaterTest : public CommonTestFixture
{
public:
    CurrenciesExchangeRateDatabankUpdaterTest() = default;

protected:
    CurrenciesExchangeRateDatabank currenciesDatabank_{"data/currencies_list.json"};
};

TEST_F(CurrenciesExchangeRateDatabankUpdaterTest, CurrenciesCount)
{
    CurrenciesExchangeRateDatabankUpdater currenciesExchangeRateDatabankUpdater;
    currenciesExchangeRateDatabankUpdater.startCurrenciesExchangeRateDatabankUpdate()
    EXPECT_EQ(currenciesDatabank_.getCurrenciesCodes().size(), 3);
}

TEST_F(CurrenciesExchangeRateDatabankUpdaterTest, AllCombinationsOfExchangeRatesArePresent)
{
    EXPECT_TRUE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("eur"), CurrencyCode("gbp")));
    EXPECT_TRUE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("eur"), CurrencyCode("usd")));

    EXPECT_TRUE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("gbp"), CurrencyCode("eur")));
    EXPECT_TRUE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("gbp"), CurrencyCode("usd")));

    EXPECT_TRUE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("usd"), CurrencyCode("eur")));
    EXPECT_TRUE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("usd"), CurrencyCode("gbp")));
}

TEST_F(CurrenciesExchangeRateDatabankUpdaterTest, XToXExchangeRatesAreNotPresent)
{
    EXPECT_FALSE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("eur"), CurrencyCode("eur")));
    EXPECT_FALSE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("gbp"), CurrencyCode("gbp")));
    EXPECT_FALSE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("usd"), CurrencyCode("usd")));
}
