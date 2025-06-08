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

TEST(ConnectionTest, Constructor)
{
    Connection<SocketReceiveSuccess> connection((SocketReceiveSuccess()));
}

TEST(ConnectionTest, ConstructorEof)
{
    Connection<SocketReceiveEof> connection((SocketReceiveEof()));
}

TEST(ConnectionTest, ConstructorError)
{
    Connection<SocketReceiveError> connection((SocketReceiveError()));
}
