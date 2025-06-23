#include "../src/connection.h"
#include "mocks.h"
#include "gtest/gtest.h"

TEST(ConnectionTest, CreateConnection)
{
    ASSERT_NO_FATAL_FAILURE({ Connection<FakeSocketReceiveSuccess> connection((FakeSocketReceiveSuccess())); });
}

TEST(ConnectionTest, AcceptConnectionSucces)
{
    Connection<FakeSocketReceiveSuccess> connection((FakeSocketReceiveSuccess()));
    ASSERT_NO_FATAL_FAILURE({ connection.accepted(); });
}

TEST(ConnectionTest, AcceptConnectionEof)
{
    Connection<FakeSocketReceiveEof> connection((FakeSocketReceiveEof()));
    ASSERT_NO_FATAL_FAILURE({ connection.accepted(); });
}

TEST(ConnectionTest, AcceptConnectionError)
{
    Connection<FakeSocketReceiveError> connection((FakeSocketReceiveError()));
    ASSERT_NO_FATAL_FAILURE({ connection.accepted(); });
}

TEST(ConnectionTest, OnMessageCallback)
{
    Connection<FakeSocketReceiveSuccess> connection((FakeSocketReceiveSuccess("test")));
    connection.set_on_message_received([](const std::string& message) {
        EXPECT_EQ(message, "test");
    });
    ASSERT_NO_FATAL_FAILURE({ connection.accepted(); });
}