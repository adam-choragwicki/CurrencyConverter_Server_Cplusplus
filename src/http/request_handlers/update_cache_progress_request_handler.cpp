#include "update_cache_progress_request_handler.h"
#include "message_processing/response_factory.h"
#include "messages/message_contract.h"
#include "services/request_processing_service.h"
#include "spdlog/spdlog.h"

UpdateCacheProgressRequestHandler::UpdateCacheProgressRequestHandler(const IRequestProcessingService& requestProcessingService)
    : requestProcessingService_(requestProcessingService)
{}

void UpdateCacheProgressRequestHandler::handle(const httplib::Request& request, httplib::Response& response) const
{
    handleRequestIfItHasCorrelationId(request, response, [&](const CorrelationId& correlationId)
    {
        spdlog::debug("[<-] Received GET {} from {} query={}, CID=[{}]",
                      MessageContract::UPDATE_CACHE_PROGRESS_ENDPOINT,
                      request.remote_addr,
                      request.target,
                      correlationId.toString());

        if (!ensureRequestHasNoParameters(request, response, correlationId, "UpdateCacheProgress"))
        {
            return;
        }

        const UpdateCacheProgressResponseDto dto = requestProcessingService_.handleUpdateCacheProgress();
        const std::string json = ResponseFactory::makeUpdateCacheProgressJson(dto);
        setJsonResponse(response, correlationId, json);
    });
}

