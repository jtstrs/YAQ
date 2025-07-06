#include "../src/connection.h"
#include "mocks.h"
#include "gtest/gtest.h"

TEST(ConnectionTest, CreateConnection)
{
    ASSERT_NO_FATAL_FAILURE({ Connection<FakeSocketReceiveSuccess> connection((FakeSocketReceiveSuccess())); });
}

TEST(ConnectionTest, AcceptConnectionSucces)
{
    ASSERT_NO_FATAL_FAILURE({ Connection<FakeSocketReceiveSuccess> connection((FakeSocketReceiveSuccess())); });
}

TEST(ConnectionTest, AcceptConnectionEof)
{
    ASSERT_NO_FATAL_FAILURE({ Connection<FakeSocketReceiveEof> connection((FakeSocketReceiveEof())); });
}

TEST(ConnectionTest, AcceptConnectionError)
{
    ASSERT_NO_FATAL_FAILURE({ Connection<FakeSocketReceiveError> connection((FakeSocketReceiveError())); });
}

struct MessageSource {
    static constexpr std::string_view message = "test";
};

TEST(ConnectionTest, OnMessageCallback)
{
    using ConnectionType = Connection<FakeSocketSuccessReceiveMessage<MessageSource>>;
    ASSERT_NO_FATAL_FAILURE({
        ConnectionType connection((FakeSocketSuccessReceiveMessage<MessageSource>()));
        connection.set_on_message_received([](const std::string& message) {
            EXPECT_EQ(message, "test");
        });
    });
}