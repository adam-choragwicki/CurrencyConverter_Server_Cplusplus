#pragma once

#include "types/client_id.h"

class UniqueIdGenerator
{
public:
    static ClientId generateID()
    {
        return ClientId(std::to_string(counter_++));
    }

private:
    inline static unsigned int counter_{};
};
