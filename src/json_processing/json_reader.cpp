#include "json_reader.h"
#include "rapidjson/error/en.h"
#include "exceptions.h"

JsonReader::JsonReader(const std::string& string) : document_(parseStringToDOM(string))
{}

bool JsonReader::hasKey(const std::string& key) const
{
    return document_.HasMember(key);
}

std::string JsonReader::getStringValue(const std::string& key) const
{
    if(document_.HasMember(key))
    {
        return document_[key].GetString();
    }
    else
    {
        throw std::runtime_error("Error, key does not exist: " + key);
    }
}

std::string JsonReader::getStringValue(const std::string& key1, const std::string& key2) const
{
    if(document_.HasMember(key1))
    {
        if(document_[key1].HasMember(key2))
        {
            return document_[key1][key2].GetString();
        }
        else
        {
            throw std::runtime_error("Error, key does not exist: " + key2);
        }
    }
    else
    {
        throw std::runtime_error("Error, key does not exist: " + key1);
    }
}

std::string JsonReader::getNumericValueAsString(const std::string& key) const
{
    if(document_.HasMember(key))
    {
        return std::to_string(document_[key].GetDouble());
    }
    else
    {
        throw std::runtime_error("Error, key does not exist: " + key);
    }
}

std::string JsonReader::getNumericValueAsString(const std::string& key1, const std::string& key2) const
{
    if(document_.HasMember(key1))
    {
        if(document_[key1].HasMember(key2))
        {
            return std::to_string(document_[key1][key2].GetDouble());
        }
        else
        {
            throw std::runtime_error("Error, key does not exist: " + key2);
        }
    }
    else
    {
        throw std::runtime_error("Error, key does not exist: " + key1);
    }
}

[[maybe_unused]] std::string JsonReader::getBooleanValueAsString(const std::string& key) const
{
    if(document_.HasMember(key))
    {
        return std::to_string(document_[key].GetBool());
    }
    else
    {
        throw std::runtime_error("Error, key does not exist: " + key);
    }
}

std::vector<std::pair<std::string, std::string>> JsonReader::getKeyValuePairs() const
{
    std::vector<std::pair<std::string, std::string>> members;

    for(auto it = document_.MemberBegin(); it != document_.MemberEnd(); ++it)
    {
        std::pair<std::string, std::string> member;
        members.emplace_back(it->name.GetString(), it->value.GetString());
    }

    return members;
}

rapidjson::Document JsonReader::parseStringToDOM(const std::string& string) const
{
    rapidjson::Document document;
    rapidjson::ParseResult parseResult = document.Parse(string);

    if(parseResult.IsError())
    {
        size_t offset = parseResult.Offset();
        std::string message = "Failed to parse JSON at offset " + std::to_string(offset) + ". " + rapidjson::GetParseError_En(parseResult.Code());
        throw JsonParseError(message);
    }

    return document;
}
