#pragma once

#include <string>
#include <map>

struct StatusResponseDto
{
    std::string status;
};

struct GetConfigResponseDto
{
    std::string initialSourceCurrencyCode;
    std::string initialTargetCurrencyCode;
    std::map<std::string, std::string> currenciesNamesAndCodes;
};

struct CalculateExchangeResponseDto
{
    std::string status;
    std::string sourceCurrencyCode;
    std::string targetCurrencyCode;
    std::string sourceCurrencyAmount;
    std::string exchangeRate;
    std::string exchangeResult;
    std::string exchangeRateTimestamp;
};

struct UpdateCacheResponseDto
{
    std::string status;
};

struct UpdateCacheProgressResponseDto
{
    std::string status;
    std::string completed;
    std::string total;
    std::string error; // optional, empty when not failed
};

struct ErrorResponseDto
{
    std::string error;
    std::string errorCode;
};
