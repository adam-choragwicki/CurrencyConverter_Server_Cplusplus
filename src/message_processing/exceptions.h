#pragma once

#include <stdexcept>

class InboundMessageError : public std::runtime_error
{
public:
    explicit InboundMessageError(const std::string& message) : runtime_error(message)
    {}
};
