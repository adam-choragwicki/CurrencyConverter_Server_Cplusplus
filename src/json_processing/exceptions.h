#pragma once

#pragma once

#include <stdexcept>

class JsonParseError : public std::runtime_error
{
public:
    explicit JsonParseError(const std::string& message) : runtime_error(message)
    {}
};
