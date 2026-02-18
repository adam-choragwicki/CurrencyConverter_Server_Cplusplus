#include "status_request_handler.h"
#include "messages/message_contract.h"
#include "spdlog/spdlog.h"
#include "http/request_parser.h"
#include "message_processing/response_factory.h"
#include "services/request_processing_service.h"

StatusRequestHandler::StatusRequestHandler(const IRequestProcessingService& requestProcessingService) : requestProcessingService_(requestProcessingService)
{}

void StatusRequestHandler::handle(const httplib::Request& request, httplib::Response& response) const
{
    handleRequestIfItHasCorrelationId(request, response, [&](const CorrelationId& correlationId)
    {
        spdlog::info("[<-] Received GET {} from {} query={}, CID=[{}]",
                     MessageContract::STATUS_ENDPOINT,
                     request.remote_addr,
                     request.target,
                     correlationId.toString());

        if (!ensureRequestHasNoParameters(request, response, correlationId, "Status"))
        {
            return;
        }

        const StatusRequest statusRequest = RequestParser::parseStatusRequest(request);
        const StatusResponseDto dto = requestProcessingService_.handleStatus(statusRequest);
        const std::string json = ResponseFactory::makeStatusJson(dto);
        setJsonResponse(response, correlationId, json);
    });
}
