#pragma once

#include "abstract_request_handler.h"

class IRequestProcessingService;

class GetConfigRequestHandler : public AbstractRequestHandler
{
public:
    explicit GetConfigRequestHandler(const IRequestProcessingService& requestProcessingService);

    void handle(const httplib::Request& request, httplib::Response& response) const;

private:
    const IRequestProcessingService& requestProcessingService_;
};
