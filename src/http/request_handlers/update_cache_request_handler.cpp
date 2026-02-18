#include "update_cache_request_handler.h"
#include "http/request_parser.h"
#include "message_processing/response_factory.h"
#include "messages/message_contract.h"
#include "spdlog/spdlog.h"
#include "services/request_processing_service.h"

UpdateCacheRequestHandler::UpdateCacheRequestHandler(const IRequestProcessingService& requestProcessingService) : requestProcessingService_(requestProcessingService)
{}

void UpdateCacheRequestHandler::handle(const httplib::Request& request, httplib::Response& response) const
{
    handleRequestIfItHasCorrelationId(request, response, [&](const CorrelationId& correlationId)
    {
        spdlog::info("[<-] Received GET {} from {} query={}, CID=[{}]",
                     MessageContract::UPDATE_CACHE_ENDPOINT,
                     request.remote_addr,
                     request.target,
                     correlationId.toString());

        if (!ensureRequestHasNoParameters(request, response, correlationId, "UpdateCache"))
        {
            return;
        }

        const UpdateCacheRequest updateCacheRequest = RequestParser::parseUpdateCacheRequest(request);
        const UpdateCacheRequestResult updateCacheServiceResult = requestProcessingService_.handleUpdateCache(updateCacheRequest);

        if (!updateCacheServiceResult.success)
        {
            if (updateCacheServiceResult.alreadyInProgress)
            {
                setErrorResponse(response,
                                 409,
                                 MessageContract::MessagePayload::ErrorCodeContract::UPDATE_CACHE_ALREADY_IN_PROGRESS,
                                 "Update cache already in progress",
                                 &correlationId);
                return;
            }

            setErrorResponse(response,
                             500,
                             MessageContract::MessagePayload::ErrorCodeContract::UPDATE_CACHE_FAILED,
                             "Update cache failed",
                             &correlationId);
            return;
        }

        const UpdateCacheResponseDto& dto = updateCacheServiceResult.response;
        const std::string json = ResponseFactory::makeUpdateCacheJson(dto);
        setJsonResponse(response, correlationId, json);
    });
}
