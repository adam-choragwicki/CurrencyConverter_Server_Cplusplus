#pragma once

#include <stdexcept>

class JsonParseError : public std::runtime_error
{
public:
    explicit JsonParseError(const std::string& message) : runtime_error(message)
    {}
};

class JsonMissingKeyError : public std::runtime_error
{
public:
    explicit JsonMissingKeyError(const std::string& key) : runtime_error("Error, JSON does not contain key: " + key)
    {}
};
