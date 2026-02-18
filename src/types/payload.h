#pragma once

#include "abstract_string_wrapper.h"

class Payload : public AbstractStringWrapper<Payload>
{
public:
    explicit Payload(const std::string& string) : AbstractStringWrapper(string)
    {}
};
