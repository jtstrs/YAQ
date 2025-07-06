#include "../src/network.h"
#include "mocks.h"
#include "gtest/gtest.h"

TEST(NetworkTest, CreateNetwork)
{
    boost::asio::io_context io_context;
    NetworkBase<FakeAcceptorSuccess<FakeSocketReceiveSuccess>, FakeSocketReceiveSuccess, FakeConnection> network(io_context, "127.0.0.1", 8080);
    ASSERT_NO_FATAL_FAILURE({ network.run(); });
}

TEST(NetworkTest, StopNetwork)
{
    boost::asio::io_context io_context;
    NetworkBase<FakeAcceptorSuccess<FakeSocketReceiveSuccess>, FakeSocketReceiveSuccess, FakeConnection> network(io_context, "127.0.0.1", 8080);
    ASSERT_NO_FATAL_FAILURE({ network.stop(); });
}

TEST(NetworkTest, SetOnNewConnection)
{
    boost::asio::io_context io_context;
    NetworkBase<FakeAcceptorSuccess<FakeSocketReceiveSuccess>, FakeSocketReceiveSuccess, FakeConnection> network(io_context, "127.0.0.1", 8080);
    network.set_on_new_connection([](std::shared_ptr<FakeConnection> connection) {
        EXPECT_TRUE(connection);
    });
    ASSERT_NO_FATAL_FAILURE({ network.run(); });
}

TEST(NetworkTest, HandleAcceptError)
{
    boost::asio::io_context io_context;
    NetworkBase<FakeAcceptorError<FakeSocketReceiveSuccess>, FakeSocketReceiveSuccess, FakeConnection> network(io_context, "127.0.0.1", 8080);
    network.set_on_new_connection([](std::shared_ptr<FakeConnection> connection) {
        EXPECT_TRUE(connection);
    });
    ASSERT_NO_FATAL_FAILURE({ network.run(); });
}