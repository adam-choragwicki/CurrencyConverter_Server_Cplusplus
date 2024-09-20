#include "common_test_fixture.h"
#include "json_processing/json_parser.h"

class JsonParserTest : public CommonTestFixture
{
};

TEST_F(JsonParserTest, EmptyCurlyBraces)
{
    std::string emptyJsonString("{}");

    EXPECT_TRUE(JsonParser::isValidJsonString(emptyJsonString));
}

TEST_F(JsonParserTest, EmptyString)
{
    std::string emptyString;

    EXPECT_FALSE(JsonParser::isValidJsonString(emptyString));
}

TEST_F(JsonParserTest, MalformedString)
{
    std::string malformedString("{");

    EXPECT_FALSE(JsonParser::isValidJsonString(malformedString));
}

TEST_F(JsonParserTest, SimpleKeyValue)
{
    std::string jsonString = R"({"key" : "value"})";

    EXPECT_TRUE(JsonParser::isValidJsonString(jsonString));
}

TEST_F(JsonParserTest, UnquotedKey)
{
    std::string jsonString = R"({key : "value"})";

    EXPECT_FALSE(JsonParser::isValidJsonString(jsonString));
}

TEST_F(JsonParserTest, UnquotedNonStringValue)
{
    std::string jsonString = R"({"key" : value})";

    EXPECT_FALSE(JsonParser::isValidJsonString(jsonString));
}

TEST_F(JsonParserTest, NumericValue)
{
    std::string jsonString = R"({"key" : 5})";

    EXPECT_TRUE(JsonParser::isValidJsonString(jsonString));
}

TEST_F(JsonParserTest, BooleanValueLowerCase)
{
    std::string jsonString = R"({"key" : true})";

    EXPECT_TRUE(JsonParser::isValidJsonString(jsonString));
}

TEST_F(JsonParserTest, BooleanValueUpperCase)
{
    std::string jsonString = R"({"key" : True})";

    EXPECT_FALSE(JsonParser::isValidJsonString(jsonString));
}
