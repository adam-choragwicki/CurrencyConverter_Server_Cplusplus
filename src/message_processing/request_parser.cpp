#include "request_parser.h"
#include "json_processing/json_reader.h"
#include "exceptions.h"
#include "json_processing/exceptions.h"
#include <optional>

ParsedInboundMessage InboundMessageParser::parseRawInboundMessage(const RawInboundMessage& rawInboundMessage)
{
    const MessageType messageType = parseRequestType(rawInboundMessage);
    const CorrelationId& correlationId = parseCorrelationId(rawInboundMessage);

    return ParsedInboundMessage(messageType, rawInboundMessage.getMessageBody(), rawInboundMessage.getSenderId(), correlationId);
}

MessageType InboundMessageParser::parseRequestType(const RawInboundMessage& rawInboundMessage)
{
    try
    {
        JsonReader jsonReader(rawInboundMessage.getMessageBody().toString());

        if(jsonReader.hasKey(MessageContract::TYPE))
        {
            const std::string messageType = jsonReader.getStringValue(MessageContract::TYPE);

            if(messageType == MessageContract::MessageType::RequestType::GET_CONFIG_REQUEST)
            {
                return MessageType(MessageContract::MessageType::RequestType::GET_CONFIG_REQUEST);
            }
            else if(messageType == MessageContract::MessageType::RequestType::CALCULATE_EXCHANGE_REQUEST)
            {
                return MessageType(MessageContract::MessageType::RequestType::CALCULATE_EXCHANGE_REQUEST);
            }
            else if(messageType == MessageContract::MessageType::RequestType::UPDATE_CACHE_REQUEST)
            {
                return MessageType(MessageContract::MessageType::RequestType::UPDATE_CACHE_REQUEST);
            }
            else
            {
                throw InboundMessageError("Error, unrecognized message type: " + messageType);
            }
        }
        else
        {
            throw InboundMessageError("Error, message does not have type");
        }
    }
    catch(const JsonParseError& jsonParseError)
    {
        throw InboundMessageError("Error, message is not a valid JSON");
    }
}

CorrelationId InboundMessageParser::parseCorrelationId(const RawInboundMessage& rawInboundMessage)
{
    JsonReader jsonReader(rawInboundMessage.getMessageBody().toString());

    if(jsonReader.hasKey(MessageContract::CORRELATION_ID))
    {
        return static_cast<CorrelationId>(jsonReader.getStringValue(MessageContract::CORRELATION_ID));
    }
    else
    {
        throw InboundMessageError("Error, message does not have correlation ID");
    }
}

GetConfigRequest InboundMessageParser::parseToGetConfigRequest(const ParsedInboundMessage& parsedInboundMessage)
{
    JsonReader jsonReader(parsedInboundMessage.getMessageBody().toString());

    return {parsedInboundMessage.getMessageBody(), parsedInboundMessage.getCorrelationId()};
}

CalculateExchangeRequest InboundMessageParser::parseToCalculateExchangeRequest(const ParsedInboundMessage& parsedInboundMessage)
{
    JsonReader jsonReader(parsedInboundMessage.getMessageBody().toString());

    std::optional<CurrencyCode> sourceCurrencyCode;
    std::optional<CurrencyCode> targetCurrencyCode;
    std::optional<MoneyAmount> moneyAmount;

    if(jsonReader.hasKey(MessageContract::MessageContent::CalculateExchangeRequestContract::SOURCE_CURRENCY))
    {
        sourceCurrencyCode = CurrencyCode(jsonReader.getStringValue(MessageContract::MessageContent::CalculateExchangeRequestContract::SOURCE_CURRENCY));
    }
    else
    {
        std::string errorMessage = "Error, " + MessageContract::MessageType::RequestType::CALCULATE_EXCHANGE_REQUEST + " does not have field: " + MessageContract::MessageContent::CalculateExchangeRequestContract::SOURCE_CURRENCY;
        throw InboundMessageError(errorMessage);
    }

    if(jsonReader.hasKey(MessageContract::MessageContent::CalculateExchangeRequestContract::TARGET_CURRENCY))
    {
        targetCurrencyCode = CurrencyCode(jsonReader.getStringValue(MessageContract::MessageContent::CalculateExchangeRequestContract::TARGET_CURRENCY));
    }
    else
    {
        std::string errorMessage = "Error, " + MessageContract::MessageType::RequestType::CALCULATE_EXCHANGE_REQUEST + " does not have field: " + MessageContract::MessageContent::CalculateExchangeRequestContract::TARGET_CURRENCY;
        throw InboundMessageError(errorMessage);
    }

    if(jsonReader.hasKey(MessageContract::MessageContent::CalculateExchangeRequestContract::MONEY_AMOUNT))
    {
        moneyAmount = MoneyAmount(jsonReader.getStringValue(MessageContract::MessageContent::CalculateExchangeRequestContract::MONEY_AMOUNT));
    }
    else
    {
        std::string errorMessage = "Error, " + MessageContract::MessageType::RequestType::CALCULATE_EXCHANGE_REQUEST + " does not have field: " + MessageContract::MessageContent::CalculateExchangeRequestContract::MONEY_AMOUNT;
        throw InboundMessageError(errorMessage);
    }

    return {parsedInboundMessage.getMessageBody(), parsedInboundMessage.getCorrelationId(), sourceCurrencyCode.value(), targetCurrencyCode.value(), moneyAmount.value()};
}

UpdateCacheRequest InboundMessageParser::parseToUpdateCacheRequest(const ParsedInboundMessage& parsedInboundMessage)
{
    JsonReader jsonReader(parsedInboundMessage.getMessageBody().toString());

    return {parsedInboundMessage.getMessageBody(), parsedInboundMessage.getCorrelationId()};
}
