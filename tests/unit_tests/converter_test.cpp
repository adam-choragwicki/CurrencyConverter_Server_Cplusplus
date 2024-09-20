#include "common_test_fixture.h"
#include "converter/converter.h"
#include "stubs/currencies_databank_stub.h"
#include "types/money_amount.h"

class ConverterTest : public CommonTestFixture
{
protected:
    std::string result_;
};

TEST_F(ConverterTest, SmokeTest)
{
    ExchangeRate exchangeRate = CurrenciesDatabankStub::getEurToUsdExchangeRate();
    result_ = Converter::convert(MoneyAmount("1"), exchangeRate);

    EXPECT_EQ(result_, "1.07");
}

TEST_F(ConverterTest, ConversionResultPrecisionIsAlwaysTwo)
{
    ExchangeRate exchangeRate = CurrenciesDatabankStub::getEurToUsdExchangeRate();

    result_ = Converter::convert(MoneyAmount("5"), exchangeRate);
    EXPECT_EQ(result_, "5.37");

    result_ = Converter::convert(MoneyAmount("15.53535"), exchangeRate);
    EXPECT_EQ(result_, "16.68");

    result_ = Converter::convert(MoneyAmount("5464.657324345"), exchangeRate);
    EXPECT_EQ(result_, "5867.13");
}
