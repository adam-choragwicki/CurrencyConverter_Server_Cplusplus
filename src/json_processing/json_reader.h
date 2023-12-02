#pragma once

#include "json_processing_config.h"
#include "rapidjson/document.h"
#include <vector>

class JsonReader
{
public:
    explicit JsonReader(const std::string& string);

    [[nodiscard]] bool hasKey(const std::string& key) const;
    [[nodiscard]] std::string getStringValue(const std::string& key) const;
    [[nodiscard]] std::string getStringValue(const std::string& key1, const std::string& key2) const;
    [[nodiscard]] std::string getNumericValueAsString(const std::string& key) const;
    [[nodiscard]] std::string getNumericValueAsString(const std::string& key1, const std::string& key2) const;
    [[maybe_unused]] [[nodiscard]] std::string getBooleanValueAsString(const std::string& key) const;
    [[nodiscard]] std::vector<std::pair<std::string, std::string>> getKeyValuePairs() const;

private:
    [[nodiscard]] rapidjson::Document parseStringToDOM(const std::string& string) const;

    const rapidjson::Document document_;
};
