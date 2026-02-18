#pragma once

#include "abstract_request_handler.h"

class IRequestProcessingService;

class CalculateExchangeRequestHandler : public AbstractRequestHandler
{
public:
    explicit CalculateExchangeRequestHandler(const IRequestProcessingService& requestProcessingService);

    void handle(const httplib::Request& request, httplib::Response& response) const;

private:
    const IRequestProcessingService& requestProcessingService_;
};
