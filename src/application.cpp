#include "application.h"
#include "socket_communication/exceptions.h"
#include "message_processing/exceptions.h"
#include "message_processing/request_parser.h"
#include "message_processing/request_processor.h"

#include "messages/responses/get_config_response.h"
#include "messages/responses/calculate_exchange_response.h"
#include "messages/responses/update_cache_response.h"

#include "config/config.h"
#include "spdlog/spdlog.h"
#include "utilities/files_helper.h"
#include <iostream>

using namespace std::chrono_literals;

Application::Application(const Config& config) : config_(config)
{
    connectionManager_ = std::make_unique<ConnectionManager>(config_);

    auto loadCurrenciesFileContent = [this](const std::string& path)
    {
        if(FilesHelper::fileExists(Paths::CurrenciesExchangeRatesDatabankConfig::CURRENCIES_LIST_FILE_PATH))
        {
            spdlog::info("Loading '{}'", Paths::CurrenciesExchangeRatesDatabankConfig::CURRENCIES_LIST_FILE_PATH);

            currenciesListFileContent_ = CurrenciesListFileContent(FilesHelper::loadFileContent(Paths::CurrenciesExchangeRatesDatabankConfig::CURRENCIES_LIST_FILE_PATH));

            return currenciesListFileContent_;
        }
        else
        {
            spdlog::critical("File '" + Paths::CurrenciesExchangeRatesDatabankConfig::CURRENCIES_LIST_FILE_PATH + "' does not exist");
            exit(1);
        }
    };

    const CurrenciesListFileContent currenciesListFileContent = loadCurrenciesFileContent(Paths::CurrenciesExchangeRatesDatabankConfig::CURRENCIES_LIST_FILE_PATH);

    spdlog::debug("Currencies exchange rate databank initialized");

    currenciesExchangeRatesDatabank_ = std::make_unique<CurrenciesExchangeRatesDatabank>(currenciesListFileContent);
    downloadManager_ = std::make_unique<DownloadManager>();

    spdlog::debug("Currency converter server initialized, listening on port " + std::to_string(config_.getPort()));
    runApplication();
}

void Application::runApplication()
{
    std::atomic_bool closeApplicationCommandReceived = false;

    startInteractiveInputModeThread(closeApplicationCommandReceived);
    startInboundMessageProcessingThread();
    startWaitForConnectionAndAcceptThread();

    while(true)
    {
        if(closeApplicationCommandReceived)
        {
            return;
        }

        std::this_thread::sleep_for(500ms);
    }
}

void Application::startInteractiveInputModeThread(std::atomic_bool& closeApplicationCommandReceived)
{
    std::thread(&Application::interactiveInputModeThread, this, std::ref(closeApplicationCommandReceived)).detach();
}

void Application::startInboundMessageProcessingThread()
{
    std::thread(&Application::inboundMessageProcessingThread, this).detach();
}

void Application::startWaitForConnectionAndAcceptThread()
{
    std::thread(&Application::waitForConnectionAndAcceptThread, this).detach();
}

void Application::startClientMessageConsumingThread(ClientSocketHandler& clientSocketHandler)
{
    std::thread(&Application::clientMessageConsumingThread, this, std::ref(clientSocketHandler)).detach();
}

[[noreturn]] void Application::waitForConnectionAndAcceptThread()
{
    while(true)
    {
        ClientSocketHandler& clientSocketHandler = connectionManager_->waitForConnectionAndAccept();
        startClientMessageConsumingThread(clientSocketHandler);
    }
}

[[noreturn]] void Application::inboundMessageProcessingThread()
{
    inboundMessageQueue_ = std::make_unique<InboundMessageQueue>();

    while(true)
    {
        while(inboundMessageQueue_->hasMessages())
        {
            messageQueueMutex_.lock();
            const RawInboundMessage& rawInboundMessage = inboundMessageQueue_->popMessage();
            messageQueueMutex_.unlock();

            try
            {
                ParsedInboundMessage parsedInboundMessage = InboundMessageParser::parseRawInboundMessage(rawInboundMessage);

                spdlog::info(" [<-] Processing " + parsedInboundMessage.getMessageType().toString() + " [" + parsedInboundMessage.getCorrelationId().toString() + "]");
                spdlog::debug(parsedInboundMessage.getMessageBody().toString());

                if(parsedInboundMessage.getMessageType() == MessageContract::MessageType::RequestType::GET_CONFIG_REQUEST)
                {
                    const auto& request = InboundMessageParser::parseToGetConfigRequest(parsedInboundMessage);
                    const auto& response = RequestProcessor::processRequest(request, *currenciesExchangeRatesDatabank_, currenciesListFileContent_);

                    connectionManager_->sendResponse(response, parsedInboundMessage.getSenderId());
                }
                else if(parsedInboundMessage.getMessageType() == MessageContract::MessageType::RequestType::CALCULATE_EXCHANGE_REQUEST)
                {
                    const auto& request = InboundMessageParser::parseToCalculateExchangeRequest(parsedInboundMessage);
                    const auto& response = RequestProcessor::processRequest(request, *currenciesExchangeRatesDatabank_);

                    connectionManager_->sendResponse(response, parsedInboundMessage.getSenderId());
                }
                else if(parsedInboundMessage.getMessageType() == MessageContract::MessageType::RequestType::UPDATE_CACHE_REQUEST)
                {
                    const auto& request = InboundMessageParser::parseToUpdateCacheRequest(parsedInboundMessage);
                    const auto& response = RequestProcessor::processRequest(request, *currenciesExchangeRatesDatabank_, *downloadManager_);

                    connectionManager_->sendResponse(response, parsedInboundMessage.getSenderId());
                }
                else
                {
                    throw InboundMessageError("Error, unsupported message type");
                }
            }
            catch(const InboundMessageError& exception)
            {
                spdlog::error(exception.what() + std::string(". Message discarded"));
                spdlog::debug("Discarded message: {}", rawInboundMessage.getMessageBody().toString());
            }
        }

        std::this_thread::sleep_for(50ms);
    }
}

void Application::clientMessageConsumingThread(ClientSocketHandler& clientSocketHandler)
{
    try
    {
        spdlog::info(" [*] Starting consuming messages");

        while(true)
        {
            /*Blocking*/
            MessageBody receivedMessageBody = MessageBody(clientSocketHandler.receiveMessage());

            RawInboundMessage rawInboundMessage(receivedMessageBody, clientSocketHandler.getId());

            spdlog::debug(" [<-] Received " + std::to_string(rawInboundMessage.getMessageBody().toString().size()) + " bytes: " + rawInboundMessage.getMessageBody().toString());

            std::lock_guard<std::mutex> lock(messageQueueMutex_);
            inboundMessageQueue_->addMessage(rawInboundMessage);
        }
    }
    catch(const ConnectionClosedByClient& exception)
    {
        spdlog::info(" [*] Connection closed by client");
        connectionManager_->closeConnection(clientSocketHandler);
    }
}

void Application::interactiveInputModeThread(std::atomic_bool& closeApplicationCommandReceived)
{
    spdlog::info("Started interactive input mode. Please type 'exit' to stop server or 'clients' to display list of currently connected clients");

    std::string userInput;

    while(std::getline(std::cin, userInput))
    {
        std::transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower);

        if(userInput == "exit")
        {
            spdlog::info("Received exit command");
            closeApplicationCommandReceived = true;
            return;
        }
        else if(userInput == "clients")
        {
            const auto& connectedClients = connectionManager_->getConnectedClients();

            if(connectedClients.empty())
            {
                spdlog::info("No connected clients");
            }
            else
            {
                spdlog::info("Connected clients:");

                for(const ClientInfo& clientInfo : connectedClients)
                {
                    spdlog::info("ID: {}, IP: {}, PORT: {}, FD: {}", clientInfo.getId().toString(), clientInfo.getAddress(), clientInfo.getPort(), clientInfo.getSocketFD());
                }
            }
        }
        else
        {
            spdlog::warn("Unrecognized command. Please type 'exit' to stop server or 'clients' to display list of currently connected clients");
        }
    }
}
