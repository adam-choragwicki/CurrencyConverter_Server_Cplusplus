#include "common_test_fixture.h"
#include "message_processing/calculate_exchange_request_validator.h"
#include "stubs/correlation_id_stub.h"
#include "stubs/message_body_stub.h"
#include "stubs/currency_code_stub.h"

class RequestValidatorTest : public CommonTestFixture
{
};

TEST_F(RequestValidatorTest, SmokeTest)
{
    CalculateExchangeRequest calculationRequestData{MessageBodyStub::getValue(), CorrelationIdStub::getValue(), CurrencyCodeStub::getValueForEuro(), CurrencyCodeStub::getValueForUsd(), MoneyAmount("1.50")};

    EXPECT_TRUE(CalculateExchangeRequestValidator::validateRequest(calculationRequestData).isValid());
}

TEST_F(RequestValidatorTest, MoneyAmountIsDecimalNumberStartingWithZero)
{
    CalculateExchangeRequest calculationRequestData1{MessageBodyStub::getValue(), CorrelationIdStub::getValue(), CurrencyCodeStub::getValueForEuro(), CurrencyCodeStub::getValueForUsd(), MoneyAmount("0.50")};

    EXPECT_TRUE(CalculateExchangeRequestValidator::validateRequest(calculationRequestData1).isValid());

    CalculateExchangeRequest calculationRequestData2{MessageBodyStub::getValue(), CorrelationIdStub::getValue(), CurrencyCodeStub::getValueForEuro(), CurrencyCodeStub::getValueForUsd(), MoneyAmount("0.005")};

    EXPECT_TRUE(CalculateExchangeRequestValidator::validateRequest(calculationRequestData2).isValid());

    CalculateExchangeRequest calculationRequestData3{MessageBodyStub::getValue(), CorrelationIdStub::getValue(), CurrencyCodeStub::getValueForEuro(), CurrencyCodeStub::getValueForUsd(), MoneyAmount("0.00000005")};

    EXPECT_TRUE(CalculateExchangeRequestValidator::validateRequest(calculationRequestData3).isValid());

    CalculateExchangeRequest calculationRequestData4{MessageBodyStub::getValue(), CorrelationIdStub::getValue(), CurrencyCodeStub::getValueForEuro(), CurrencyCodeStub::getValueForUsd(), MoneyAmount("0.")};

    EXPECT_TRUE(CalculateExchangeRequestValidator::validateRequest(calculationRequestData4).isValid());
}

TEST_F(RequestValidatorTest, MoneyAmountIsNegative)
{
    CalculateExchangeRequest calculationRequestData{MessageBodyStub::getValue(), CorrelationIdStub::getValue(), CurrencyCodeStub::getValueForEuro(), CurrencyCodeStub::getValueForUsd(), MoneyAmount("-1.50")};

    EXPECT_FALSE(CalculateExchangeRequestValidator::validateRequest(calculationRequestData).isValid());
}

TEST_F(RequestValidatorTest, PrefixAndSuffix)
{
    CalculateExchangeRequest calculationRequestData1{MessageBodyStub::getValue(), CorrelationIdStub::getValue(), CurrencyCodeStub::getValueForEuro(), CurrencyCodeStub::getValueForUsd(), MoneyAmount("--1.50")};

    EXPECT_FALSE(CalculateExchangeRequestValidator::validateRequest(calculationRequestData1).isValid());

    CalculateExchangeRequest calculationRequestData2{MessageBodyStub::getValue(), CorrelationIdStub::getValue(), CurrencyCodeStub::getValueForEuro(), CurrencyCodeStub::getValueForUsd(), MoneyAmount("(1.50")};

    EXPECT_FALSE(CalculateExchangeRequestValidator::validateRequest(calculationRequestData2).isValid());

    CalculateExchangeRequest calculationRequestData3{MessageBodyStub::getValue(), CorrelationIdStub::getValue(), CurrencyCodeStub::getValueForEuro(), CurrencyCodeStub::getValueForUsd(), MoneyAmount("1.50)")};

    EXPECT_FALSE(CalculateExchangeRequestValidator::validateRequest(calculationRequestData2).isValid());
}

TEST_F(RequestValidatorTest, EmptyMoneyAmount)
{
    CalculateExchangeRequest calculationRequestData{MessageBodyStub::getValue(), CorrelationIdStub::getValue(), CurrencyCodeStub::getValueForEuro(), CurrencyCodeStub::getValueForUsd(), MoneyAmount("     ")};

    EXPECT_FALSE(CalculateExchangeRequestValidator::validateRequest(calculationRequestData).isValid());
}

TEST_F(RequestValidatorTest, MoneyAmountIsOctalNumber)
{
    CalculateExchangeRequest calculationRequestData1{MessageBodyStub::getValue(), CorrelationIdStub::getValue(), CurrencyCodeStub::getValueForEuro(), CurrencyCodeStub::getValueForUsd(), MoneyAmount("0123")};

    EXPECT_FALSE(CalculateExchangeRequestValidator::validateRequest(calculationRequestData1).isValid());

    CalculateExchangeRequest calculationRequestData2{MessageBodyStub::getValue(), CorrelationIdStub::getValue(), CurrencyCodeStub::getValueForEuro(), CurrencyCodeStub::getValueForUsd(), MoneyAmount("00123")};

    EXPECT_FALSE(CalculateExchangeRequestValidator::validateRequest(calculationRequestData2).isValid());
}

TEST_F(RequestValidatorTest, MoneyAmountIsHexadecimalNumber)
{
    CalculateExchangeRequest calculationRequestData{MessageBodyStub::getValue(), CorrelationIdStub::getValue(), CurrencyCodeStub::getValueForEuro(), CurrencyCodeStub::getValueForUsd(), MoneyAmount("0x123")};

    EXPECT_FALSE(CalculateExchangeRequestValidator::validateRequest(calculationRequestData).isValid());
}

TEST_F(RequestValidatorTest, MoneyAmountContainsWhitespaces)
{
    CalculateExchangeRequest calculationRequestData1{MessageBodyStub::getValue(), CorrelationIdStub::getValue(), CurrencyCodeStub::getValueForEuro(), CurrencyCodeStub::getValueForUsd(), MoneyAmount("123    456")};

    EXPECT_FALSE(CalculateExchangeRequestValidator::validateRequest(calculationRequestData1).isValid());

    CalculateExchangeRequest calculationRequestData2{MessageBodyStub::getValue(), CorrelationIdStub::getValue(), CurrencyCodeStub::getValueForEuro(), CurrencyCodeStub::getValueForUsd(), MoneyAmount("0. 123  456")};

    EXPECT_FALSE(CalculateExchangeRequestValidator::validateRequest(calculationRequestData2).isValid());
}

TEST_F(RequestValidatorTest, WrongMoneyAmount)
{
    CalculateExchangeRequest calculationRequestData1{MessageBodyStub::getValue(), CorrelationIdStub::getValue(), CurrencyCodeStub::getValueForEuro(), CurrencyCodeStub::getValueForUsd(), MoneyAmount("abc")};

    EXPECT_FALSE(CalculateExchangeRequestValidator::validateRequest(calculationRequestData1).isValid());

    CalculateExchangeRequest calculationRequestData2{MessageBodyStub::getValue(), CorrelationIdStub::getValue(), CurrencyCodeStub::getValueForEuro(), CurrencyCodeStub::getValueForUsd(), MoneyAmount(",,")};

    EXPECT_FALSE(CalculateExchangeRequestValidator::validateRequest(calculationRequestData2).isValid());
}
