#include <gtest/gtest.h>
#include "exchange_calculator/exchange_calculator.h"
#include "types/currency_amount.h"
#include "types/exchange_rate.h"
#include "types/exchange_result.h"

TEST(ConverterTest, ConvertsAndRoundsToTwoDecimals)
{
    const ExchangeRate exchangeRate("1.0736503481886");

    const ExchangeResult result1 = ExchangeCalculator::convert(CurrencyAmount("1"), exchangeRate);
    EXPECT_EQ(result1.toString(), "1.07");

    const ExchangeResult result2 = ExchangeCalculator::convert(CurrencyAmount("5"), exchangeRate);
    EXPECT_EQ(result2.toString(), "5.37");

    const ExchangeResult result3 = ExchangeCalculator::convert(CurrencyAmount("15.53535"), exchangeRate);
    EXPECT_EQ(result3.toString(), "16.68");

    const ExchangeResult result4 = ExchangeCalculator::convert(CurrencyAmount("5464.657324345"), exchangeRate);
    EXPECT_EQ(result4.toString(), "5867.13");
}

TEST(ConverterTest, InvalidAmountThrows)
{
    const ExchangeRate exchangeRate("1.1");
    EXPECT_THROW(ExchangeCalculator::convert(CurrencyAmount("abc"), exchangeRate), ConversionError);
}

TEST(ConverterTest, InvalidRateThrows)
{
    EXPECT_THROW(ExchangeCalculator::convert(CurrencyAmount("1"), ExchangeRate("0")), ConversionError);
    EXPECT_THROW(ExchangeCalculator::convert(CurrencyAmount("1"), ExchangeRate("-1")), ConversionError);
    EXPECT_THROW(ExchangeCalculator::convert(CurrencyAmount("1"), ExchangeRate("abc")), ConversionError);
}
