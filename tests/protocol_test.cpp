#include "../src/protocol.h"
#include <gtest/gtest.h>

class ProtocolTest : public ::testing::Test {
protected:
    Protocol protocol;
};

// Test valid commands
TEST_F(ProtocolTest, ValidSubscribeCommand)
{
    auto cmd = protocol.parse("subscribe, news");
    EXPECT_EQ(cmd.type, CommandType::Subscribe);
    EXPECT_EQ(cmd.args.size(), 1);
    EXPECT_EQ(std::get<std::string>(cmd.args[0]), "news");
}

TEST_F(ProtocolTest, ValidUnsubscribeCommand)
{
    auto cmd = protocol.parse("unsubscribe, sports");
    EXPECT_EQ(cmd.type, CommandType::Unsubscribe);
    EXPECT_EQ(cmd.args.size(), 1);
    EXPECT_EQ(std::get<std::string>(cmd.args[0]), "sports");
}

TEST_F(ProtocolTest, ValidTopicsCommand)
{
    auto cmd = protocol.parse("topics");
    EXPECT_EQ(cmd.type, CommandType::Topic);
    EXPECT_EQ(cmd.args.size(), 0);
}

TEST_F(ProtocolTest, ValidPostMessageCommand)
{
    auto cmd = protocol.parse("post, tech, New release available");
    EXPECT_EQ(cmd.type, CommandType::PostMessage);
    EXPECT_EQ(cmd.args.size(), 2);
    EXPECT_EQ(std::get<std::string>(cmd.args[0]), "tech");
    EXPECT_EQ(std::get<std::string>(cmd.args[1]), "New release available");
}

TEST_F(ProtocolTest, ValidPingCommand)
{
    auto cmd = protocol.parse("ping");
    EXPECT_EQ(cmd.type, CommandType::Ping);
    EXPECT_EQ(cmd.args.size(), 0);
}

// Test invalid commands
TEST_F(ProtocolTest, InvalidCommandType)
{
    EXPECT_THROW(protocol.parse("invalid"), std::runtime_error);
    EXPECT_THROW(protocol.parse("unknown"), std::runtime_error);
}

TEST_F(ProtocolTest, InvalidSubscribeArguments)
{
    EXPECT_THROW(protocol.parse("subscribe"), std::runtime_error);
    EXPECT_THROW(protocol.parse("subscribe, news, sports"), std::runtime_error);
}

TEST_F(ProtocolTest, InvalidUnsubscribeArguments)
{
    EXPECT_THROW(protocol.parse("unsubscribe"), std::runtime_error);
    EXPECT_THROW(protocol.parse("unsubscribe, news, sports"), std::runtime_error);
}

TEST_F(ProtocolTest, InvalidPostMessageArguments)
{
    EXPECT_THROW(protocol.parse("post"), std::runtime_error);
    EXPECT_THROW(protocol.parse("post, news"), std::runtime_error);
    EXPECT_THROW(protocol.parse("post, news, message, extra"), std::runtime_error);
}

TEST_F(ProtocolTest, InvalidTopicsArguments)
{
    EXPECT_THROW(protocol.parse("topics, news"), std::runtime_error);
}

TEST_F(ProtocolTest, InvalidPingArguments)
{
    EXPECT_THROW(protocol.parse("ping, server"), std::runtime_error);
}

// Test command parsing with extra spaces
TEST_F(ProtocolTest, ExtraSpaces)
{
    auto cmd = protocol.parse("  subscribe  ,  news  ");
    EXPECT_EQ(cmd.type, CommandType::Subscribe);
    EXPECT_EQ(cmd.args.size(), 1);
    EXPECT_EQ(std::get<std::string>(cmd.args[0]), "news");
}

// Test command parsing with multiple spaces between arguments
TEST_F(ProtocolTest, MultipleSpaces)
{
    auto cmd = protocol.parse("post, tech, New   release   available");
    EXPECT_EQ(cmd.type, CommandType::PostMessage);
    EXPECT_EQ(cmd.args.size(), 2);
    EXPECT_EQ(std::get<std::string>(cmd.args[0]), "tech");
    EXPECT_EQ(std::get<std::string>(cmd.args[1]), "New   release   available");
}

// Test command parsing with multiple spaces between arguments
TEST_F(ProtocolTest, TooManyPostsArguments)
{
    EXPECT_THROW(protocol.parse("post, tech, New release available, extra"), std::runtime_error);
}

TEST_F(ProtocolTest, TooManyTopicsArguments)
{
    EXPECT_THROW(protocol.parse("topics, news, extra"), std::runtime_error);
}

TEST_F(ProtocolTest, TooManyPingArguments)
{
    EXPECT_THROW(protocol.parse("ping, server, extra"), std::runtime_error);
}
