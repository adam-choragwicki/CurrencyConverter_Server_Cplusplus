#include "response_factory.h"
#include "json_processing/json_writer.h"
#include "messages/message_contract.h"

std::string ResponseFactory::makeStatusJson(const StatusResponseDto& dto)
{
    JsonWriter jsonWriter;
    jsonWriter.addKeyValuePair("message", "status response from server");
    jsonWriter.addKeyValuePair(MessageContract::MessagePayload::StatusResponseContract::STATUS, dto.status);
    return jsonWriter.toJsonString();
}

std::string ResponseFactory::makeGetConfigJson(const GetConfigResponseDto& dto)
{
    JsonWriter jsonWriter;

    // scalar fields
    jsonWriter.addKeyValuePair(MessageContract::MessagePayload::GetConfigResponseContract::INITIAL_SOURCE_CURRENCY, dto.initialSourceCurrencyCode);
    jsonWriter.addKeyValuePair(MessageContract::MessagePayload::GetConfigResponseContract::INITIAL_TARGET_CURRENCY, dto.initialTargetCurrencyCode);

    // currencies_names_and_codes object
    JsonWriter currenciesObjectWriter;
    for (const auto& [name, code]: dto.currenciesNamesAndCodes)
    {
        currenciesObjectWriter.addKeyValuePair(name, code);
    }

    jsonWriter.addRawJsonObjectValue(MessageContract::MessagePayload::GetConfigResponseContract::CURRENCIES_NAMES_AND_CODES, currenciesObjectWriter.toJsonString());

    return jsonWriter.toJsonString();
}

std::string ResponseFactory::makeCalculateExchangeJson(const CalculateExchangeResponseDto& dto)
{
    JsonWriter jsonWriter;

    jsonWriter.addKeyValuePair(MessageContract::MessagePayload::StatusResponseContract::STATUS, dto.status);
    jsonWriter.addKeyValuePair(MessageContract::MessagePayload::CalculateExchangeResponseContract::SOURCE_CURRENCY, dto.sourceCurrencyCode);
    jsonWriter.addKeyValuePair(MessageContract::MessagePayload::CalculateExchangeResponseContract::TARGET_CURRENCY, dto.targetCurrencyCode);
    jsonWriter.addKeyValuePair(MessageContract::MessagePayload::CalculateExchangeResponseContract::SOURCE_CURRENCY_AMOUNT, dto.sourceCurrencyAmount);
    jsonWriter.addKeyValuePair(MessageContract::MessagePayload::CalculateExchangeResponseContract::EXCHANGE_RATE, dto.exchangeRate);
    jsonWriter.addKeyValuePair(MessageContract::MessagePayload::CalculateExchangeResponseContract::EXCHANGE_RESULT, dto.exchangeResult);
    jsonWriter.addKeyValuePair(MessageContract::MessagePayload::CalculateExchangeResponseContract::EXCHANGE_RATE_TIMESTAMP, dto.exchangeRateTimestamp);

    return jsonWriter.toJsonString();
}

std::string ResponseFactory::makeUpdateCacheJson(const UpdateCacheResponseDto& dto)
{
    JsonWriter jsonWriter;

    jsonWriter.addKeyValuePair(MessageContract::MessagePayload::UpdateCacheResponseContract::STATUS, dto.status);

    return jsonWriter.toJsonString();
}

std::string ResponseFactory::makeUpdateCacheProgressJson(const UpdateCacheProgressResponseDto& dto)
{
    JsonWriter jsonWriter;

    jsonWriter.addKeyValuePair(MessageContract::MessagePayload::UpdateCacheProgressResponseContract::STATUS, dto.status);
    jsonWriter.addKeyValuePair(MessageContract::MessagePayload::UpdateCacheProgressResponseContract::COMPLETED, dto.completed);
    jsonWriter.addKeyValuePair(MessageContract::MessagePayload::UpdateCacheProgressResponseContract::TOTAL, dto.total);

    if (!dto.error.empty())
    {
        jsonWriter.addKeyValuePair(MessageContract::MessagePayload::ErrorResponseContract::ERROR_MESSAGE, dto.error);
    }

    return jsonWriter.toJsonString();
}

std::string ResponseFactory::makeErrorJson(const ErrorResponseDto& dto)
{
    JsonWriter jsonWriter;
    jsonWriter.addKeyValuePair(MessageContract::MessagePayload::ErrorResponseContract::ERROR_MESSAGE, dto.error);
    jsonWriter.addKeyValuePair(MessageContract::MessagePayload::ErrorResponseContract::ERROR_CODE, dto.errorCode);
    return jsonWriter.toJsonString();
}
