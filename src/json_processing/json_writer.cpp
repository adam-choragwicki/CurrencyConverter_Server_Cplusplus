#include "json_writer.h"

JsonWriter::JsonWriter()
{
    document_.SetObject();
}

std::string JsonWriter::writeJsonDOMToString() const
{
    rapidjson::StringBuffer stringBuffer;
    rapidjson::Writer writer(stringBuffer);
    document_.Accept(writer);

    return stringBuffer.GetString();
}

void JsonWriter::addKeyValuePair(const std::string& key, const std::string& value)
{
    rapidjson::Value jsonKey;
    jsonKey.SetString(key.c_str(), static_cast<rapidjson::SizeType>(key.length()), document_.GetAllocator());

    rapidjson::Value jsonValue;
    jsonValue.SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.length()), document_.GetAllocator());

    document_.AddMember(jsonKey, jsonValue, document_.GetAllocator());
}

void JsonWriter::addRawJsonObjectValue(const std::string& key, const std::string& rawJson)
{
    rapidjson::Document tempDoc;
    tempDoc.Parse(rawJson.c_str());

    if (tempDoc.HasParseError() || !tempDoc.IsObject())
    {
        addKeyValuePair(key, "INVALID_JSON_OBJECT_CONTENT");
        return;
    }

    rapidjson::Value jsonKey;
    jsonKey.SetString(key.c_str(), static_cast<rapidjson::SizeType>(key.length()), document_.GetAllocator());

    // use CopyFrom because tempDoc is local and will be destroyed
    rapidjson::Value nestedValue;
    nestedValue.CopyFrom(tempDoc, document_.GetAllocator());

    document_.AddMember(jsonKey, nestedValue, document_.GetAllocator());
}

std::string JsonWriter::toJsonString() const
{
    return writeJsonDOMToString();
}
