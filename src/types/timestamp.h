#pragma once

#include "abstract_string_wrapper.h"

class Timestamp : public AbstractStringWrapper
{
public:
    explicit Timestamp(const std::string& string = "NULL") : AbstractStringWrapper(string)
    {}
};
