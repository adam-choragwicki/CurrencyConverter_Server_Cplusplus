#pragma once

#include "abstract_string_wrapper.h"

class Timestamp : public AbstractStringWrapper<Timestamp>
{
public:
    explicit Timestamp(const std::string& string) : AbstractStringWrapper(string)
    {}
};
