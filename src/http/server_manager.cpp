#include "server_manager.h"
#include "http/request_handlers/status_request_handler.h"
#include "http/request_handlers/get_config_request_handler.h"
#include "http/request_handlers/calculate_exchange_request_handler.h"
#include "http/request_handlers/update_cache_request_handler.h"
#include "http/request_handlers/update_cache_progress_request_handler.h"
#include "messages/message_contract.h"
#include "spdlog/spdlog.h"

ServerManager::ServerManager(const std::string& host,
                             const int port,
                             StatusRequestHandler& statusHandler,
                             GetConfigRequestHandler& configHandler,
                             CalculateExchangeRequestHandler& calculateHandler,
                             UpdateCacheRequestHandler& updateCacheHandler,
                             UpdateCacheProgressRequestHandler& updateCacheProgressHandler
)
    : host_(host),
      port_(port),
      statusHandler_(statusHandler),
      configHandler_(configHandler),
      calculateHandler_(calculateHandler),
      updateCacheHandler_(updateCacheHandler),
      updateCacheProgressHandler_(updateCacheProgressHandler)
{
    setStatusEndpoint();
    setConfigEndpoint();
    setCalculateExchangeEndpoint();
    setUpdateCacheEndpoint();
    setUpdateCacheProgressEndpoint();
}

void ServerManager::listen()
{
    if (port_ <= 0 || port_ > 65535)
    {
        throw std::runtime_error("Connection port is out of range");
    }

    spdlog::info("[*] Serving content at http://{}:{}", host_, port_);

    server_.listen(host_, port_); // blocking call
}

void ServerManager::setStatusEndpoint()
{
    server_.Get(MessageContract::STATUS_ENDPOINT, [this](const httplib::Request& request, httplib::Response& response)
    {
        statusHandler_.handle(request, response);
    });
}

void ServerManager::setConfigEndpoint()
{
    server_.Get(MessageContract::GET_CONFIG_ENDPOINT, [this](const httplib::Request& request, httplib::Response& response)
    {
        configHandler_.handle(request, response);
    });
}

void ServerManager::setCalculateExchangeEndpoint()
{
    server_.Get(MessageContract::CALCULATE_EXCHANGE_ENDPOINT, [this](const httplib::Request& request, httplib::Response& response)
    {
        calculateHandler_.handle(request, response);
    });
}

void ServerManager::setUpdateCacheEndpoint()
{
    server_.Get(MessageContract::UPDATE_CACHE_ENDPOINT, [this](const httplib::Request& request, httplib::Response& response)
    {
        updateCacheHandler_.handle(request, response);
    });
}

void ServerManager::setUpdateCacheProgressEndpoint()
{
    server_.Get(MessageContract::UPDATE_CACHE_PROGRESS_ENDPOINT, [this](const httplib::Request& request, httplib::Response& response)
    {
        updateCacheProgressHandler_.handle(request, response);
    });
}
