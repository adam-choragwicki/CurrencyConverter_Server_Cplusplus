#include "common_test_fixture.h"
#include "types/currency_code.h"
#include "currencies_exchange_rate_databank/currencies_exchange_rate_databank.h"
#include "currencies_exchange_rate_databank/currencies_exchange_rate_databank_modifier.h"
#include "stubs/currencies_list_file_content_stub.h"

class CurrenciesExchangeRateDatabankModifierTest : public CommonTestFixture
{
public:
    CurrenciesExchangeRateDatabankModifierTest() = default;

protected:
    CurrenciesExchangeRateDatabank currenciesDatabank_{CurrenciesListFileContentStub::getValue()};
};

TEST_F(CurrenciesExchangeRateDatabankModifierTest, CurrenciesCount)
{
    CurrenciesExchangeRateDatabankModifier currenciesExchangeRateDatabankUpdater;
    EXPECT_EQ(currenciesDatabank_.getCurrenciesCodes().size(), 3);
}

TEST_F(CurrenciesExchangeRateDatabankModifierTest, AllCombinationsOfExchangeRatesArePresent)
{
    EXPECT_TRUE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("eur"), CurrencyCode("gbp")));
    EXPECT_TRUE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("eur"), CurrencyCode("usd")));

    EXPECT_TRUE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("gbp"), CurrencyCode("eur")));
    EXPECT_TRUE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("gbp"), CurrencyCode("usd")));

    EXPECT_TRUE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("usd"), CurrencyCode("eur")));
    EXPECT_TRUE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("usd"), CurrencyCode("gbp")));
}

TEST_F(CurrenciesExchangeRateDatabankModifierTest, XToXExchangeRatesAreNotPresent)
{
    EXPECT_FALSE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("eur"), CurrencyCode("eur")));
    EXPECT_FALSE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("gbp"), CurrencyCode("gbp")));
    EXPECT_FALSE(currenciesDatabank_.containsExchangeRateData(CurrencyCode("usd"), CurrencyCode("usd")));
}
