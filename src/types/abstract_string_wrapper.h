#pragma once

#include <string>
#include <stdexcept>

class AbstractStringWrapper
{
public:
    explicit AbstractStringWrapper(const std::string& string) : string_(string)
    {
        if(string.empty())
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
        std::transform(string_.begin(), string_.end(), std::back_inserter(upperCode), ::toupper);
        return upperCode;
    }

    template <typename T>
    requires std::is_base_of<AbstractStringWrapper, T>::value || std::is_same_v<T, std::string>
    bool operator==(const T& other) const
    {
        if constexpr (std::is_same_v<T, std::string>)
        {
            return string_ == other;
        }
        else
        {
            return string_ == other.toString();
        }
    }

    template <typename T>
    requires std::is_base_of<AbstractStringWrapper, T>::value || std::is_same_v<T, std::string>
    bool operator<(const T& other) const
    {
        if constexpr (std::is_same_v<T, std::string>)
        {
            return string_ < other;
        }
        else
        {
            return string_ < other.toString();
        }
    }

private:
    std::string string_;
};
