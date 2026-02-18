#include "abstract_request_handler.h"
#include "http/correlation_id_http_helper.h"
#include "message_processing/response_factory.h"
#include "messages/message_contract.h"
#include "spdlog/spdlog.h"

std::optional<CorrelationId> AbstractRequestHandler::ensureRequestHasCorrelationId(const httplib::Request& request, httplib::Response& response)
{
    if (!CorrelationIdHttpHelper::hasCorrelationId(request))
    {
        spdlog::error("Request has no correlation id");
        setErrorResponse(response,
                         400,
                         MessageContract::MessagePayload::ErrorCodeContract::MISSING_CORRELATION_ID,
                         "Missing correlation id");
        return std::nullopt;
    }

    return CorrelationIdHttpHelper::extractCorrelationIdFromRequest(request);
}

bool AbstractRequestHandler::ensureRequestHasNoParameters(const httplib::Request& request,
                                                          httplib::Response& response,
                                                          const CorrelationId& correlationId,
                                                          const std::string& requestName)
{
    if (!request.params.empty())
    {
        spdlog::warn("Bad {} Request: Unexpected parameters", requestName);
        setErrorResponse(response,
                         400,
                         MessageContract::MessagePayload::ErrorCodeContract::UNEXPECTED_PARAMETERS,
                         "Unexpected parameters",
                         &correlationId);
        return false;
    }

    return true;
}

void AbstractRequestHandler::setJsonResponse(httplib::Response& response,
                                             const CorrelationId& correlationId,
                                             const std::string& responseContent)
{
    CorrelationIdHttpHelper::attachCorrelationIdToResponse(correlationId, response);
    spdlog::debug("[->] Responding with: {}, CID=[{}]", responseContent, correlationId.toString());
    response.set_content(responseContent, "application/json");
}

void AbstractRequestHandler::setErrorResponse(httplib::Response& response,
                                              const int status,
                                              const std::string& errorCode,
                                              const std::string& message,
                                              const CorrelationId* correlationId)
{
    if (correlationId != nullptr)
    {
        CorrelationIdHttpHelper::attachCorrelationIdToResponse(*correlationId, response);
    }

    ErrorResponseDto dto;
    dto.error = message;
    dto.errorCode = errorCode;

    response.status = status;
    response.set_content(ResponseFactory::makeErrorJson(dto), "application/json");
}
