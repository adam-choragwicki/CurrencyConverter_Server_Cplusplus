#include "common_test_fixture.h"
#include "types/currency_code.h"
#include "json_processing/json_validator.h"

class JsonValidatorTest : public CommonTestFixture
{
};

TEST_F(JsonValidatorTest, EmptyCurlyBraces)
{
    std::string emptyJsonString("{}");

    EXPECT_TRUE(JsonValidator::isValidJsonString(emptyJsonString));
}

TEST_F(JsonValidatorTest, EmptyString)
{
    std::string emptyString;

    EXPECT_FALSE(JsonValidator::isValidJsonString(emptyString));
}

TEST_F(JsonValidatorTest, MalformedString)
{
    std::string malformedString("{");

    EXPECT_FALSE(JsonValidator::isValidJsonString(malformedString));
}

TEST_F(JsonValidatorTest, SimpleKeyValue)
{
    std::string jsonString = R"({"key" : "value"})";

    EXPECT_TRUE(JsonValidator::isValidJsonString(jsonString));
}

TEST_F(JsonValidatorTest, UnquotedKey)
{
    std::string jsonString = R"({key : "value"})";

    EXPECT_FALSE(JsonValidator::isValidJsonString(jsonString));
}

TEST_F(JsonValidatorTest, UnquotedNonStringValue)
{
    std::string jsonString = R"({"key" : value})";

    EXPECT_FALSE(JsonValidator::isValidJsonString(jsonString));
}

TEST_F(JsonValidatorTest, NumericValue)
{
    std::string jsonString = R"({"key" : 5})";

    EXPECT_TRUE(JsonValidator::isValidJsonString(jsonString));
}

TEST_F(JsonValidatorTest, BooleanValueLowerCase)
{
    std::string jsonString = R"({"key" : true})";

    EXPECT_TRUE(JsonValidator::isValidJsonString(jsonString));
}

TEST_F(JsonValidatorTest, BooleanValueUpperCase)
{
    std::string jsonString = R"({"key" : True})";

    EXPECT_FALSE(JsonValidator::isValidJsonString(jsonString));
}
