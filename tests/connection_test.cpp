#include "../src/connection.h"
#include "mocks.h"
#include "gtest/gtest.h"

TEST(ConnectionTest, CreateConnection)
{
    ASSERT_NO_FATAL_FAILURE({ Connection<SocketReceiveSuccess> connection((SocketReceiveSuccess())); });
}

TEST(ConnectionTest, AcceptConnectionSucces)
{
    Connection<SocketReceiveSuccess> connection((SocketReceiveSuccess()));
    ASSERT_NO_FATAL_FAILURE({ connection.accepted(); });
}

TEST(ConnectionTest, AcceptConnectionEof)
{
    Connection<SocketReceiveEof> connection((SocketReceiveEof()));
    ASSERT_NO_FATAL_FAILURE({ connection.accepted(); });
}

TEST(ConnectionTest, AcceptConnectionError)
{
    Connection<SocketReceiveError> connection((SocketReceiveError()));
    ASSERT_NO_FATAL_FAILURE({ connection.accepted(); });
}

TEST(ConnectionTest, OnMessageCallback)
{
    Connection<SocketReceiveSuccess> connection((SocketReceiveSuccess("test")));
    connection.set_on_message_received([](const std::string& message) {
        EXPECT_EQ(message, "test");
    });
    ASSERT_NO_FATAL_FAILURE({ connection.accepted(); });
}