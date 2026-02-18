#pragma once

#include "abstract_request_handler.h"

class IRequestProcessingService;

class UpdateCacheProgressRequestHandler : public AbstractRequestHandler
{
public:
    explicit UpdateCacheProgressRequestHandler(const IRequestProcessingService& requestProcessingService);
    void handle(const httplib::Request& request, httplib::Response& response) const;

private:
    const IRequestProcessingService& requestProcessingService_;
};

