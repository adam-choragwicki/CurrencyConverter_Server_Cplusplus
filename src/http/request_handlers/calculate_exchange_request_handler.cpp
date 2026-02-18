#include "calculate_exchange_request_handler.h"
#include "messages/message_contract.h"
#include "spdlog/spdlog.h"
#include "http/request_parser.h"
#include "message_processing/response_factory.h"
#include "services/request_processing_service.h"

CalculateExchangeRequestHandler::CalculateExchangeRequestHandler(const IRequestProcessingService& requestProcessingService) : requestProcessingService_(requestProcessingService)
{}

void CalculateExchangeRequestHandler::handle(const httplib::Request& request, httplib::Response& response) const
{
    handleRequestIfItHasCorrelationId(request, response, [&](const CorrelationId& correlationId)
    {
        spdlog::info("[<-] Received GET {} from {} query={}, CID=[{}]",
                     MessageContract::CALCULATE_EXCHANGE_ENDPOINT,
                     request.remote_addr,
                     request.target,
                     correlationId.toString());

        if (!RequestParser::hasRequiredCalculateExchangeRequestParameters(request))
        {
            spdlog::warn("Bad CalculateExchange Request: Missing required parameter");
            setErrorResponse(response,
                             400,
                             MessageContract::MessagePayload::ErrorCodeContract::INVALID_PARAMETERS,
                             "Invalid parameters",
                             &correlationId);
            return;
        }

        const CalculateExchangeRequest calculateExchangeRequest = RequestParser::parseCalculateExchangeRequest(request);
        const CalculateExchangeRequestResult calculateServiceResult = requestProcessingService_.handleCalculate(calculateExchangeRequest);

        if (!calculateServiceResult.valid_)
        {
            const std::string& failureReason = calculateServiceResult.failureReason_.value();
            const bool isMissingData = calculateServiceResult.missingData_;

            spdlog::warn("Bad CalculateExchange Request: {}", failureReason);
            setErrorResponse(response,
                             isMissingData ? 400 : 422,
                             isMissingData
                                 ? MessageContract::MessagePayload::ErrorCodeContract::EXCHANGE_RATE_DATA_MISSING
                                 : MessageContract::MessagePayload::ErrorCodeContract::INVALID_CALCULATION_REQUEST,
                             failureReason,
                             &correlationId);
            return;
        }

        const CalculateExchangeResponseDto& dto = *calculateServiceResult.response_;
        const std::string json = ResponseFactory::makeCalculateExchangeJson(dto);
        setJsonResponse(response, correlationId, json);
    });
}
