#include "../src/protocol.h"
#include <gtest/gtest.h>

class ProtocolTest : public ::testing::Test {
protected:
    Protocol protocol;
};

// Test valid commands
TEST_F(ProtocolTest, ValidSubscribeCommand)
{
    auto cmd = protocol.parse("{\"command\": \"subscribe\", \"args\": [\"news\"]}");
    EXPECT_EQ(cmd.type, CommandType::Subscribe);
    EXPECT_EQ(cmd.args.size(), 1);
    EXPECT_EQ(cmd.args[0], "news");
}

TEST_F(ProtocolTest, ValidUnsubscribeCommand)
{
    auto cmd = protocol.parse("{\"command\": \"unsubscribe\", \"args\": [\"sports\"]}");
    EXPECT_EQ(cmd.type, CommandType::Unsubscribe);
    EXPECT_EQ(cmd.args.size(), 1);
    EXPECT_EQ(cmd.args[0], "sports");
}

TEST_F(ProtocolTest, ValidTopicsCommand)
{
    auto cmd = protocol.parse("{\"command\": \"topics\", \"args\": []}");
    EXPECT_EQ(cmd.type, CommandType::Topic);
    EXPECT_EQ(cmd.args.size(), 0);
}

TEST_F(ProtocolTest, ValidPostMessageCommand)
{
    auto cmd = protocol.parse("{\"command\": \"post\", \"args\": [\"tech\", \"New release available\"]}");
    EXPECT_EQ(cmd.type, CommandType::PostMessage);
    EXPECT_EQ(cmd.args.size(), 2);
    EXPECT_EQ(cmd.args[0], "tech");
    EXPECT_EQ(cmd.args[1], "New release available");
}

TEST_F(ProtocolTest, ValidPingCommand)
{
    auto cmd = protocol.parse("{\"command\": \"ping\", \"args\": []}");
    EXPECT_EQ(cmd.type, CommandType::Ping);
    EXPECT_EQ(cmd.args.size(), 0);
}

// Test invalid commands
TEST_F(ProtocolTest, InvalidCommandType)
{
    EXPECT_THROW(protocol.parse("{\"command\": \"invalid\", \"args\": []}"), std::runtime_error);
    EXPECT_THROW(protocol.parse("{\"command\": \"unknown\", \"args\": []}"), std::runtime_error);
}

TEST_F(ProtocolTest, InvalidSubscribeArguments)
{
    EXPECT_THROW(protocol.parse("{\"command\": \"subscribe\", \"args\": []}"), std::runtime_error);
    EXPECT_THROW(protocol.parse("{\"command\": \"subscribe\", \"args\": [\"news\", \"sports\"]}"), std::runtime_error);
}

TEST_F(ProtocolTest, InvalidUnsubscribeArguments)
{
    EXPECT_THROW(protocol.parse("{\"command\": \"unsubscribe\", \"args\": []}"), std::runtime_error);
    EXPECT_THROW(protocol.parse("{\"command\": \"unsubscribe\", \"args\": [\"news\", \"sports\"]}"), std::runtime_error);
}

TEST_F(ProtocolTest, InvalidPostMessageArguments)
{
    EXPECT_THROW(protocol.parse("{\"command\": \"post\", \"args\": []}"), std::runtime_error);
    EXPECT_THROW(protocol.parse("{\"command\": \"post\", \"args\": [\"news\"]}"), std::runtime_error);
    EXPECT_THROW(protocol.parse("{\"command\": \"post\", \"args\": [\"news\", \"message\", \"extra\"]}"), std::runtime_error);
}

TEST_F(ProtocolTest, InvalidTopicsArguments)
{
    EXPECT_THROW(protocol.parse("{\"command\": \"topics\", \"args\": [\"news\"]}"), std::runtime_error);
}

TEST_F(ProtocolTest, InvalidPingArguments)
{
    EXPECT_THROW(protocol.parse("{\"command\": \"ping\", \"args\": [\"server\"]}"), std::runtime_error);
}

// Test JSON parsing errors
TEST_F(ProtocolTest, InvalidJsonFormat)
{
    EXPECT_THROW(protocol.parse("invalid json"), std::runtime_error);
    EXPECT_THROW(protocol.parse("{invalid}"), std::runtime_error);
    EXPECT_THROW(protocol.parse(""), std::runtime_error);
}

TEST_F(ProtocolTest, MissingCommandField)
{
    EXPECT_THROW(protocol.parse("{\"args\": [\"news\"]}"), std::runtime_error);
}

TEST_F(ProtocolTest, MissingArgsField)
{
    EXPECT_THROW(protocol.parse("{\"command\": \"subscribe\"}"), std::runtime_error);
}

TEST_F(ProtocolTest, InvalidCommandTypeField)
{
    EXPECT_THROW(protocol.parse("{\"command\": 123, \"args\": []}"), std::runtime_error);
}

TEST_F(ProtocolTest, InvalidArgsField)
{
    EXPECT_THROW(protocol.parse("{\"command\": \"subscribe\", \"args\": \"not_an_array\"}"), std::runtime_error);
}

// Test command parsing with different data types in args
TEST_F(ProtocolTest, ArgsWithDifferentTypes)
{
    // Test with a command that accepts exactly 2 arguments but verify type conversion
    auto cmd = protocol.parse("{\"command\": \"post\", \"args\": [\"topic\", 123]}");
    EXPECT_EQ(cmd.type, CommandType::PostMessage);
    EXPECT_EQ(cmd.args.size(), 2);
    EXPECT_EQ(cmd.args[0], "topic");
    EXPECT_EQ(cmd.args[1], "123");
}

// Test data type conversion for different types
TEST_F(ProtocolTest, DataTypeConversion)
{
    // Test int conversion
    auto cmd1 = protocol.parse("{\"command\": \"post\", \"args\": [\"topic\", 123]}");
    EXPECT_EQ(cmd1.args[1], "123");

    // Test double conversion
    auto cmd2 = protocol.parse("{\"command\": \"post\", \"args\": [\"topic\", 45.67]}");
    const std::string expected = "45.67";
    EXPECT_TRUE(cmd2.args[1].find(expected) != std::string::npos);

    // Test bool conversion
    auto cmd3 = protocol.parse("{\"command\": \"post\", \"args\": [\"topic\", true]}");
    EXPECT_EQ(cmd3.args[1], "1");

    auto cmd4 = protocol.parse("{\"command\": \"post\", \"args\": [\"topic\", false]}");
    EXPECT_EQ(cmd4.args[1], "0");
}

// Test case insensitive command parsing
TEST_F(ProtocolTest, CaseInsensitiveCommands)
{
    auto cmd1 = protocol.parse("{\"command\": \"SUBSCRIBE\", \"args\": [\"news\"]}");
    EXPECT_EQ(cmd1.type, CommandType::Subscribe);

    auto cmd2 = protocol.parse("{\"command\": \"Subscribe\", \"args\": [\"news\"]}");
    EXPECT_EQ(cmd2.type, CommandType::Subscribe);

    auto cmd3 = protocol.parse("{\"command\": \"subscribe\", \"args\": [\"news\"]}");
    EXPECT_EQ(cmd3.type, CommandType::Subscribe);
}
