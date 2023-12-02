#pragma once

#include "socket_communication/connection_manager.h"
#include "message_processing/inbound_message_queue.h"
#include "currencies_exchange_rate_databank/currencies_exchange_rate_databank.h"
#include "config/config.h"
#include <atomic>

class Application
{
public:
    explicit Application(const Config& config);

private:
    void runApplication();

    void startInteractiveInputModeThread(std::atomic_bool& closeApplicationCommandReceived);
    void interactiveInputModeThread(std::atomic_bool& closeApplicationCommandReceived);

    void startClientMessageConsumingThread(ClientSocketHandler& clientSocketHandler);
    void clientMessageConsumingThread(ClientSocketHandler& clientSocketHandler);

    void startInboundMessageProcessingThread();
    [[noreturn]] void inboundMessageProcessingThread();

    void startWaitForConnectionAndAcceptThread();
    [[noreturn]] void waitForConnectionAndAcceptThread();

    const Config config_;
    std::unique_ptr<ConnectionManager> connectionManager_;
    InboundMessageQueue inboundMessageQueue_;
    CurrenciesExchangeRateDatabank currenciesDatabank_;
    std::mutex messageQueueMutex_;
};
