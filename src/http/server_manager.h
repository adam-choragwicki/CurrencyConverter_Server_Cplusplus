#pragma once

#include "httplib.h"

class StatusRequestHandler;
class GetConfigRequestHandler;
class CalculateExchangeRequestHandler;
class UpdateCacheRequestHandler;
class UpdateCacheProgressRequestHandler;

class ServerManager
{
public:
    ServerManager(const std::string& host,
                  int port,
                  StatusRequestHandler& statusHandler,
                  GetConfigRequestHandler& configHandler,
                  CalculateExchangeRequestHandler& calculateHandler,
                  UpdateCacheRequestHandler& updateCacheHandler,
                  UpdateCacheProgressRequestHandler& updateCacheProgressHandler
    );
    void listen();

private:
    void setStatusEndpoint();
    void setConfigEndpoint();
    void setCalculateExchangeEndpoint();
    void setUpdateCacheEndpoint();
    void setUpdateCacheProgressEndpoint();

    httplib::Server server_;

    const std::string host_;
    const int port_{};

    StatusRequestHandler& statusHandler_;
    GetConfigRequestHandler& configHandler_;
    CalculateExchangeRequestHandler& calculateHandler_;
    UpdateCacheRequestHandler& updateCacheHandler_;
    UpdateCacheProgressRequestHandler& updateCacheProgressHandler_;
};
