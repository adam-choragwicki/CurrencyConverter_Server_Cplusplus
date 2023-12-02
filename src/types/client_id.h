#pragma once

#include "abstract_string_wrapper.h"

class ClientId : public AbstractStringWrapper
{
public:
    explicit ClientId(const std::string& string) : AbstractStringWrapper(string)
    {}
};
