#pragma once

#include "socket_utilities.h"

class ServerSocketHandler
{
public:
    explicit ServerSocketHandler(int port);
    virtual ~ServerSocketHandler();
    [[nodiscard]] int getSocketFD() const;

private:
    const int serverSocketFD_{-1};
};
