#include "../src/connection.h"
#include "mocks.h"
#include "gtest/gtest.h"

struct SocketReceiveSuccess {
    int32_t received_count = 0;

    void async_receive(boost::asio::mutable_buffer buffer, std::function<void(const boost::system::error_code&, std::size_t)> handler)
    {
        if (received_count == 2) {
            return;
        }
        // To check receive after receive is called
        received_count++;
        handler(boost::system::error_code(), 0);
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
