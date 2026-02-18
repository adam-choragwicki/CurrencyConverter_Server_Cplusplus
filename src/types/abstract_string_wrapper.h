#pragma once

#include <string>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <iterator>

template<typename T> // for CRTP
class AbstractStringWrapper
{
public:
    explicit AbstractStringWrapper(const std::string& string) : string_(string)
    {
        if (string.empty())
        {
            throw std::runtime_error("Error, trying to construct empty string wrapper");
        }
    }

    [[nodiscard]] const std::string& toString() const
    {
        return string_;
    }

    [[nodiscard]] std::string toUpperCase() const
    {
        std::string upperCode;
        upperCode.reserve(string_.size());

        std::ranges::transform(string_, std::back_inserter(upperCode), [](const unsigned char c) { return static_cast<char>(std::toupper(c)); });

        return upperCode;
    }

    // CRTP enforces strong type comparison safety
    auto operator<=>(const AbstractStringWrapper&) const = default;

private:
    using StrongTypeTag = T; // silence unused parameter type T warning
    std::string string_;
};
