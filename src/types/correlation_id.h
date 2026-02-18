#pragma once

#include "abstract_string_wrapper.h"

class CorrelationId : public AbstractStringWrapper<CorrelationId>
{
public:
    explicit CorrelationId(const std::string& string) : AbstractStringWrapper(string)
    {}
};
