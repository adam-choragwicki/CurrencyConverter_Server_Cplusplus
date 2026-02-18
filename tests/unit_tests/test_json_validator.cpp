#include <gtest/gtest.h>
#include "json_processing/json_validator.h"

TEST(JsonValidatorTest, EmptyCurlyBraces)
{
    const std::string emptyJsonString = "{}";
    EXPECT_TRUE(JsonValidator::isValidJsonString(emptyJsonString));
}

TEST(JsonValidatorTest, EmptyString)
{
    const std::string emptyString;
    EXPECT_FALSE(JsonValidator::isValidJsonString(emptyString));
}

TEST(JsonValidatorTest, MalformedString)
{
    const std::string malformedString = "{";
    EXPECT_FALSE(JsonValidator::isValidJsonString(malformedString));
}

TEST(JsonValidatorTest, SimpleKeyValue)
{
    const std::string jsonString = R"({"key":"value"})";
    EXPECT_TRUE(JsonValidator::isValidJsonString(jsonString));
}

TEST(JsonValidatorTest, UnquotedKey)
{
    const std::string jsonString = R"({key:"value"})";
    EXPECT_FALSE(JsonValidator::isValidJsonString(jsonString));
}

TEST(JsonValidatorTest, UnquotedNonStringValue)
{
    const std::string jsonString = R"({"key":value})";
    EXPECT_FALSE(JsonValidator::isValidJsonString(jsonString));
}

TEST(JsonValidatorTest, NumericValue)
{
    const std::string jsonString = R"({"key":5})";
    EXPECT_TRUE(JsonValidator::isValidJsonString(jsonString));
}

TEST(JsonValidatorTest, BooleanValueLowerCase)
{
    const std::string jsonString = R"({"key":true})";
    EXPECT_TRUE(JsonValidator::isValidJsonString(jsonString));
}

TEST(JsonValidatorTest, BooleanValueUpperCase)
{
    const std::string jsonString = R"({"key":True})";
    EXPECT_FALSE(JsonValidator::isValidJsonString(jsonString));
}
