#include "json_writer.h"

JsonWriter::JsonWriter()
{
    document_.SetObject();
}

std::string JsonWriter::writeJsonDOMToString() const
{
    rapidjson::StringBuffer stringBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(stringBuffer);
    document_.Accept(writer);

    return stringBuffer.GetString();
}

void JsonWriter::addKeyValuePair(const std::string& key, const std::string& value)
{
    document_.AddMember(rapidjson::StringRef(key), rapidjson::StringRef(value), document_.GetAllocator());
}

std::string JsonWriter::toJsonString() const
{
    return writeJsonDOMToString();
}
