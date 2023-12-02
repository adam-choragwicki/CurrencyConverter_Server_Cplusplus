#pragma once

#include "abstract_string_wrapper.h"

class MessageBody : public AbstractStringWrapper
{
public:
    explicit MessageBody(const std::string& string) : AbstractStringWrapper(string)
    {}
};
