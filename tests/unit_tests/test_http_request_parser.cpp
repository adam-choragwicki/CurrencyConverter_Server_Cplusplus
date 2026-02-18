#include <gtest/gtest.h>
#include "http/request_parser.h"
#include "messages/message_contract.h"
#include "types/correlation_id.h"
#include "httplib.h"

namespace
{
    httplib::Request MakeBaseRequest()
    {
        httplib::Request request;
        request.method = "GET";
        request.target = "/";
        request.headers.emplace(MessageContract::CORRELATION_ID_HEADER, "cid-123");
        return request;
    }
}

TEST(RequestParserTest, ParseStatusRequest)
{
    const  httplib::Request request = MakeBaseRequest();

    const StatusRequest statusRequest = RequestParser::parseStatusRequest(request);
    EXPECT_EQ(statusRequest.getCorrelationId().toString(), "cid-123");
}

TEST(RequestParserTest, ParseGetConfigRequest)
{
    const  httplib::Request request = MakeBaseRequest();

    const GetConfigRequest getConfigRequest = RequestParser::parseGetConfigRequest(request);
    EXPECT_EQ(getConfigRequest.getCorrelationId().toString(), "cid-123");
}

TEST(RequestParserTest, ParseCalculateExchangeRequest)
{
    httplib::Request request = MakeBaseRequest();

    request.params = {
        {MessageContract::MessagePayload::CalculateExchangeRequestContract::SOURCE_CURRENCY, "eur"},
        {MessageContract::MessagePayload::CalculateExchangeRequestContract::TARGET_CURRENCY, "usd"},
        {MessageContract::MessagePayload::CalculateExchangeRequestContract::SOURCE_CURRENCY_AMOUNT, "12.5"}
    };

    const CalculateExchangeRequest calculateRequest = RequestParser::parseCalculateExchangeRequest(request);

    EXPECT_EQ(calculateRequest.getCorrelationId().toString(), "cid-123");
    EXPECT_EQ(calculateRequest.getSourceCurrencyCode().toString(), "eur");
    EXPECT_EQ(calculateRequest.getTargetCurrencyCode().toString(), "usd");
    EXPECT_EQ(calculateRequest.getSourceCurrencyAmount().toString(), "12.5");
}

TEST(RequestParserTest, ParseUpdateCacheRequest)
{
    const httplib::Request request = MakeBaseRequest();

    const UpdateCacheRequest updateCacheRequest = RequestParser::parseUpdateCacheRequest(request);
    EXPECT_EQ(updateCacheRequest.getCorrelationId().toString(), "cid-123");
}

TEST(RequestParserTest, ValidateCalculateExchangeRequestQuery)
{
    httplib::Request request = MakeBaseRequest();
    request.params = {
        {MessageContract::MessagePayload::CalculateExchangeRequestContract::SOURCE_CURRENCY, "eur"},
        {MessageContract::MessagePayload::CalculateExchangeRequestContract::TARGET_CURRENCY, "usd"},
        {MessageContract::MessagePayload::CalculateExchangeRequestContract::SOURCE_CURRENCY_AMOUNT, "1"}
    };

    EXPECT_TRUE(RequestParser::hasRequiredCalculateExchangeRequestParameters(request));

    request.params.erase(MessageContract::MessagePayload::CalculateExchangeRequestContract::TARGET_CURRENCY);
    EXPECT_FALSE(RequestParser::hasRequiredCalculateExchangeRequestParameters(request));

    request.params = {
        {MessageContract::MessagePayload::CalculateExchangeRequestContract::SOURCE_CURRENCY, "eur"},
        {MessageContract::MessagePayload::CalculateExchangeRequestContract::TARGET_CURRENCY, "usd"},
        {MessageContract::MessagePayload::CalculateExchangeRequestContract::SOURCE_CURRENCY_AMOUNT, "1"}, {"extra", "value"}
    };

    EXPECT_FALSE(RequestParser::hasRequiredCalculateExchangeRequestParameters(request));
}
