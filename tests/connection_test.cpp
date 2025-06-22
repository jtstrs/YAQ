#include "../src/connection.h"
#include "mocks.h"
#include "gtest/gtest.h"

struct SocketReceiveSuccess {
    int32_t received_count = 0;
    std::string received_message;

    explicit SocketReceiveSuccess(const std::string& message = "")
        : received_message(message)
    {
    }

    void async_receive(boost::asio::mutable_buffer buffer, std::function<void(const boost::system::error_code&, std::size_t)> handler)
    {
        if (received_count == 2) {
            return;
        }
        // To check receive after receive is called
        received_count++;
        boost::asio::buffer_copy(buffer, boost::asio::buffer(received_message), received_message.size());
        handler(boost::system::error_code(), received_message.size());
    }
};

struct SocketReceiveEof {
    void async_receive(boost::asio::mutable_buffer buffer, std::function<void(const boost::system::error_code&, std::size_t)> handler)
    {
        handler(boost::asio::error::eof, 0);
    }
};

struct SocketReceiveError {
    void async_receive(boost::asio::mutable_buffer buffer, std::function<void(const boost::system::error_code&, std::size_t)> handler)
    {
        handler(boost::asio::error::not_connected, 0);
    }
};

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