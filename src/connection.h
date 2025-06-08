#ifndef CONNECTION_H
#define CONNECTION_H

#include "logger.h"
#include "socket.h"
#include <vector>

template <typename Socket, int32_t buffer_size = 1024>
class Connection {
public:
    Connection(Socket socket)
        : socket_(std::move(socket))
        , buffer_(buffer_size)
    {
        async_receive(boost::asio::buffer(buffer_), [this](const boost::system::error_code& error, std::size_t bytes_transferred) {
        });
    }

    void async_receive(boost::asio::mutable_buffer buffer, std::function<void(const boost::system::error_code&, std::size_t)> handler)
    {
        socket_.async_receive(buffer, handler);
    }

private:
    Socket socket_;
    std::vector<char> buffer_;
};

#endif // CONNECTION_H
