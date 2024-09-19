#include "response_factory.h"
#include "messages/message_contract.h"

GetConfigResponse ResponseFactory::makeGetConfigResponse(const std::string& currenciesNamesAndCodes, const CorrelationId& correlationId)
{
    JsonWriter jsonWriter;

    assignResponseType(jsonWriter, MessageContract::MessageType::ResponseType::GET_CONFIG_RESPONSE);
    assignCorrelationId(jsonWriter, correlationId);

    jsonWriter.addKeyValuePair(MessageContract::MessageContent::GetConfigResponseContract::CURRENCIES_NAMES_AND_CODES, currenciesNamesAndCodes);

    return {MessageContract::MessageType::ResponseType::GET_CONFIG_RESPONSE, MessageBody(jsonWriter.toJsonString()), correlationId};
}

CalculateExchangeResponse ResponseFactory::makeCalculateExchangeResponse(const std::string& status, const std::string& calculationResult, const Timestamp& exchangeRateTimestamp, const std::string& failureReason, const CorrelationId& correlationId)
{
    JsonWriter jsonWriter;

    assignResponseType(jsonWriter, MessageContract::MessageType::ResponseType::CALCULATE_EXCHANGE_RESPONSE);
    assignCorrelationId(jsonWriter, correlationId);

    jsonWriter.addKeyValuePair(MessageContract::MessageContent::CalculateExchangeResponseContract::STATUS, status);
    jsonWriter.addKeyValuePair(MessageContract::MessageContent::CalculateExchangeResponseContract::CALCULATION_RESULT, calculationResult);
    jsonWriter.addKeyValuePair(MessageContract::MessageContent::CalculateExchangeResponseContract::EXCHANGE_RATE_TIMESTAMP, exchangeRateTimestamp.toString());
    jsonWriter.addKeyValuePair(MessageContract::MessageContent::CalculateExchangeResponseContract::FAILURE_REASON, failureReason);

    return {MessageContract::MessageType::ResponseType::CALCULATE_EXCHANGE_RESPONSE, MessageBody(jsonWriter.toJsonString()), correlationId};
}

UpdateCacheResponse ResponseFactory::makeUpdateCacheResponse(const std::string& status, const CorrelationId& correlationId)
{
    JsonWriter jsonWriter;

    assignResponseType(jsonWriter, MessageContract::MessageType::ResponseType::UPDATE_CACHE_RESPONSE);
    assignCorrelationId(jsonWriter, correlationId);

    jsonWriter.addKeyValuePair(MessageContract::MessageContent::UpdateCacheResponseContract::STATUS, status);

    return {MessageContract::MessageType::ResponseType::UPDATE_CACHE_RESPONSE, MessageBody(jsonWriter.toJsonString()), correlationId};
}

void ResponseFactory::assignResponseType(JsonWriter& jsonWriter, const std::string& type)
{
    jsonWriter.addKeyValuePair(MessageContract::TYPE, type);
}

void ResponseFactory::assignCorrelationId(JsonWriter& jsonWriter, const CorrelationId& correlationId)
{
    jsonWriter.addKeyValuePair(MessageContract::CORRELATION_ID, correlationId.toString());
}
