#pragma once

#include "types/correlation_id.h"
#include "types/client_id.h"

class ClientIdStub
{
public:
    static ClientId getValue()
    {
        return ClientId("1");
    }
};
