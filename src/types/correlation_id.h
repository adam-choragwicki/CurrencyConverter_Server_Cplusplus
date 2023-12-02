#pragma once

#include "abstract_string_wrapper.h"

class CorrelationId : public AbstractStringWrapper
{
public:
    explicit CorrelationId(const std::string& string) : AbstractStringWrapper(string)
    {}
};
