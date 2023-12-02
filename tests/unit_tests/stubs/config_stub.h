#pragma once

#include "config/config.h"

class ConfigStub
{
public:
    static Config getValue()
    {
        return Config(50000, true);
    }
};
