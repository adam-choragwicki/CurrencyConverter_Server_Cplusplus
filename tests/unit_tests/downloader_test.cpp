#include "common_test_fixture.h"
#include "message_processing/exceptions.h"
#include "downloader/download_manager.h"
#include "stubs/correlation_id_stub.h"
#include "stubs/client_id_stub.h"
#include "message_processing/request_parser.h"

class DownloaderTest : public CommonTestFixture
{
};

TEST_F(DownloaderTest, ParseGetConfigRequest)
{
//    DownloadManager::getInstance().downloadCurrenciesExchangeRates();
    //    std::string requestJson = R"({"correlation_id":"2c25a1bd-8407-4aea-9ccd-221fb011784d","type":"get_config_request"})";
    //
    //    ParsedInboundMessage parsedInboundMessage(MessageType("get_config_request"), MessageBody(requestJson), ClientIdStub::getValue(), CorrelationIdStub::getValue());
    //
    //    GetConfigRequest getConfigRequest = InboundMessageParser::parseToGetConfigRequest(parsedInboundMessage);
    //
    //    EXPECT_EQ(getConfigRequest.getMessageBody(), MessageBody(requestJson));
    //    EXPECT_EQ(getConfigRequest.getCorrelationId(), CorrelationIdStub::getValue());
}

//TEST_F(RequestParserTest, ParseGetConfigRequest)
//{
//    std::string requestJson = R"({"correlation_id":"2c25a1bd-8407-4aea-9ccd-221fb011784d","type":"get_config_request"})";
//
//    ParsedInboundMessage parsedInboundMessage(MessageType("get_config_request"), MessageBody(requestJson), ClientIdStub::getValue(), CorrelationIdStub::getValue());
//
//    GetConfigRequest getConfigRequest = InboundMessageParser::parseToGetConfigRequest(parsedInboundMessage);
//
//    EXPECT_EQ(getConfigRequest.getMessageBody(), MessageBody(requestJson));
//    EXPECT_EQ(getConfigRequest.getCorrelationId(), CorrelationIdStub::getValue());
//}
//
//TEST_F(RequestParserTest, ParseCalculateExchangeRequest)
//{
//    std::string requestJson = R"({"source_currency":"eur","money_amount":"1","target_currency":"usd","correlation_id":"2c25a1bd-8407-4aea-9ccd-221fb011784d","type":"calculate_exchange_request"})";
//
//    ParsedInboundMessage parsedInboundMessage(MessageType("calculate_exchange_request"), MessageBody(requestJson), ClientIdStub::getValue(), CorrelationIdStub::getValue());
//
//    CalculateExchangeRequest calculateExchangeRequest = InboundMessageParser::parseToCalculateExchangeRequest(parsedInboundMessage);
//
//    EXPECT_EQ(calculateExchangeRequest.getMessageBody(), MessageBody(requestJson));
//    EXPECT_EQ(calculateExchangeRequest.getCorrelationId(), CorrelationIdStub::getValue());
//}
//
//TEST_F(RequestParserTest, ParseUpdateCacheRequest)
//{
//    std::string requestJson = R"({"correlation_id":"2c25a1bd-8407-4aea-9ccd-221fb011784d","type":"update_cache_request"})";
//
//    ParsedInboundMessage parsedInboundMessage(MessageType("update_cache_request"), MessageBody(requestJson), ClientIdStub::getValue(), CorrelationIdStub::getValue());
//
//    UpdateCacheRequest updateCacheRequest = InboundMessageParser::parseToUpdateCacheRequest(parsedInboundMessage);
//
//    EXPECT_EQ(updateCacheRequest.getMessageBody(), MessageBody(requestJson));
//    EXPECT_EQ(updateCacheRequest.getCorrelationId(), CorrelationIdStub::getValue());
//}
//
//TEST_F(RequestParserTest, UnrecognizedRequestThrowsError)
//{
//    std::string unrecognizedRequestJson = R"({"source_currency":"eur","money_amount":"1","target_currency":"usd","type":"some_request", "correlation_id":"1234"})";
//
//    EXPECT_THROW(InboundMessageParser::parseRequestType(RawInboundMessage(MessageBody(unrecognizedRequestJson), ClientId("1"))), InboundMessageError);
//}
//
//TEST_F(RequestParserTest, RequestMissingCorrelationIdThrowsError)
//{
//    std::string requestMissingCorrelationIdJson = R"({"source_currency":"eur","money_amount":"1","target_currency":"usd","type":"calculate_exchange_request"})";
//
//    EXPECT_THROW(InboundMessageParser::parseCorrelationId(RawInboundMessage(MessageBody(requestMissingCorrelationIdJson), ClientId("1"))), InboundMessageError);
//}
