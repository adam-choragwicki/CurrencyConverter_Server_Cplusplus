#include "get_config_request_handler.h"

#include "messages/message_contract.h"
#include "spdlog/spdlog.h"
#include "http/request_parser.h"
#include "services/request_processing_service.h"
#include "message_processing/response_factory.h"

GetConfigRequestHandler::GetConfigRequestHandler(const IRequestProcessingService& requestProcessingService) : requestProcessingService_(requestProcessingService)
{}

void GetConfigRequestHandler::handle(const httplib::Request& request, httplib::Response& response) const
{
    handleRequestIfItHasCorrelationId(request, response, [&](const CorrelationId& correlationId)
    {
        spdlog::info("[<-] Received GET {} from {} query={}, CID=[{}]",
                     MessageContract::GET_CONFIG_ENDPOINT,
                     request.remote_addr,
                     request.target,
                     correlationId.toString());

        if (!ensureRequestHasNoParameters(request, response, correlationId, "GetConfig"))
        {
            return;
        }

        const GetConfigRequest getConfigRequest = RequestParser::parseGetConfigRequest(request);
        const GetConfigResponseDto dto = requestProcessingService_.handleGetConfig(getConfigRequest);
        const std::string json = ResponseFactory::makeGetConfigJson(dto);
        setJsonResponse(response, correlationId, json);
    });
}
