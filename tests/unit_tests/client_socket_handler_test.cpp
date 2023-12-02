#include <gtest/gtest.h>
#include "test_utilities.h"
#include "mocks/client_fake.h"
#include "mocks/server_fake.h"
#include "stubs/config_stub.h"
#include "stubs/response_stub.h"
#include "socket_communication/client_socket_handler.h"
#include <future>

class ClientSocketHandlerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        serverFake_ = std::make_unique<ServerFake>(ConfigStub::getValue().getPort());
    }

    void TearDown() override
    {
        /*Delay between tests*/
        delayMs(10);
    }

    void startServerWaitsForConnectionThread()
    {
        std::thread([this]()
                    {
                        ClientInfo clientInfo = serverFake_->acceptConnection();
                        clientSocketHandler_ = std::make_unique<ClientSocketHandler>(clientInfo);
                    }).detach();
    }

    std::string startServerWaitsForMessageThread()
    {
        return clientSocketHandler_->receiveMessage();
    }

    std::string startClientWaitsForMessageThread()
    {
        return clientFake_->receiveMessage();
    }

    std::unique_ptr<ServerFake> serverFake_;
    std::unique_ptr<ClientFake> clientFake_;

    /*Represents connected client socket on server side*/
    std::unique_ptr<ClientSocketHandler> clientSocketHandler_;
};

TEST_F(ClientSocketHandlerTest, ClientConnectsToServer)
{
    startServerWaitsForConnectionThread();
    multithreadingDelayMs();

    clientFake_ = std::make_unique<ClientFake>(ConfigStub::getValue().getPort());
    clientFake_->connectToServer();

    delayMs(20);

    clientFake_->disconnectFromServer();
}

TEST_F(ClientSocketHandlerTest, ServerReceivesMessageFromClient)
{
    startServerWaitsForConnectionThread();
    multithreadingDelayMs();

    clientFake_ = std::make_unique<ClientFake>(ConfigStub::getValue().getPort());
    clientFake_->connectToServer();

    auto handle = std::async(&ClientSocketHandlerTest_ServerReceivesMessageFromClient_Test::startServerWaitsForMessageThread, this);
    multithreadingDelayMs();

    const std::string messageFromClient = "Hello, Server!";
    clientFake_->sendMessage(messageFromClient);

    std::string messageReceived = handle.get();

    ASSERT_EQ(messageReceived, messageFromClient + "\n");
}

TEST_F(ClientSocketHandlerTest, ServerSendsMessageToClient)
{
    startServerWaitsForConnectionThread();
    multithreadingDelayMs();

    clientFake_ = std::make_unique<ClientFake>(ConfigStub::getValue().getPort());
    clientFake_->connectToServer();

    auto handle = std::async(&ClientSocketHandlerTest_ServerSendsMessageToClient_Test::startClientWaitsForMessageThread, this);
    multithreadingDelayMs();

    const std::string messageToClient = "Hello, Client!";

    clientSocketHandler_->sendResponse(ResponseStub::getValue(messageToClient));

    std::string messageReceived = handle.get();

    ASSERT_EQ(messageReceived, messageToClient + "\n");
}

TEST_F(ClientSocketHandlerTest, ServerCanBeReconnectedTo)
{
    startServerWaitsForConnectionThread();
    multithreadingDelayMs();

    ClientFake clientFake1(ConfigStub::getValue().getPort());
    clientFake1.connectToServer();
    delayMs(20);
    clientFake1.disconnectFromServer();

    delayMs(20);

    startServerWaitsForConnectionThread();
    multithreadingDelayMs();

    ClientFake clientFake2(ConfigStub::getValue().getPort());
    clientFake2.connectToServer();
    delayMs(20);
    clientFake2.disconnectFromServer();

    delayMs(20);

    startServerWaitsForConnectionThread();
    multithreadingDelayMs();

    ClientFake clientFake3(ConfigStub::getValue().getPort());
    clientFake3.connectToServer();
    delayMs(20);
    clientFake3.disconnectFromServer();
}
