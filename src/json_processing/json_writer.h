#pragma once

#include "json_processing_config.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"

class JsonWriter
{
public:
    JsonWriter();
    [[nodiscard]] std::string writeJsonDOMToString() const;
    void addRawJsonObjectValue(const std::string& key, const std::string& rawJson);
    void addKeyValuePair(const std::string& key, const std::string& value);
    [[nodiscard]] std::string toJsonString() const;

private:
    rapidjson::Document document_;
};
