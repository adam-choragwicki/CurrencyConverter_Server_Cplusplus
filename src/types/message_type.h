#pragma once

#include "abstract_string_wrapper.h"

class MessageType : public AbstractStringWrapper
{
public:
    explicit MessageType(const std::string& string) : AbstractStringWrapper(string)
    {}
};
