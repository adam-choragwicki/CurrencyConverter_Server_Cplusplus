#pragma once

#include "socket_communication/connection_manager.h"
#include "message_processing/inbound_message_queue.h"
#include "currencies_exchange_rate_databank/currencies_exchange_rate_databank.h"
#include "config/config.h"
#include "downloader/download_manager.h"
#include "types/currencies_list_file_content.h"
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
    std::unique_ptr<CurrenciesExchangeRateDatabank> currenciesExchangeRateDatabank_;
    std::unique_ptr<DownloadManager> downloadManager_;
    std::unique_ptr<InboundMessageQueue> inboundMessageQueue_;

    std::mutex messageQueueMutex_;

    CurrenciesListFileContent currenciesListFileContent_;
};
