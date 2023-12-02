#pragma once

#include "messages/requests/get_config_request.h"
#include "messages/requests/calculate_exchange_request.h"
#include "messages/requests/update_cache_request.h"
#include "types/message_type.h"
#include "messages/raw_inbound_message.h"
#include "types/correlation_id.h"
#include "messages/parsed_inbound_message.h"

class InboundMessageParser
{
public:
    static ParsedInboundMessage parseRawInboundMessage(const RawInboundMessage& rawInboundMessage);
    static MessageType parseRequestType(const RawInboundMessage& rawInboundMessage);
    static CorrelationId parseCorrelationId(const RawInboundMessage& rawInboundMessage);
    static GetConfigRequest parseToGetConfigRequest(const ParsedInboundMessage& parsedInboundMessage);
    static CalculateExchangeRequest parseToCalculateExchangeRequest(const ParsedInboundMessage& parsedInboundMessage);
    static UpdateCacheRequest parseToUpdateCacheRequest(const ParsedInboundMessage& parsedInboundMessage);
};
