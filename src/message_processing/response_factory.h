#pragma once

#include "messages/response_dto.h"

class ResponseFactory
{
public:
    static std::string makeStatusJson(const StatusResponseDto& dto);
    static std::string makeGetConfigJson(const GetConfigResponseDto& dto);
    static std::string makeCalculateExchangeJson(const CalculateExchangeResponseDto& dto);
    static std::string makeUpdateCacheJson(const UpdateCacheResponseDto& dto);
    static std::string makeUpdateCacheProgressJson(const UpdateCacheProgressResponseDto& dto);

    static std::string makeErrorJson(const ErrorResponseDto& dto);
};
