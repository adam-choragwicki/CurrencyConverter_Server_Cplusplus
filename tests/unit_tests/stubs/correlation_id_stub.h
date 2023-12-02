#pragma once

#include "types/correlation_id.h"

class CorrelationIdStub
{
public:
    static CorrelationId getValue()
    {
        return CorrelationId("2c25a1bd-8407-4aea-9ccd-221fb011784d");
    }
};
