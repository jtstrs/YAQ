#ifndef CONNECTION_H
#define CONNECTION_H

#include "logger.h"
#include "socket.h"

template <typename Socket>
class Connection {
public:
    Connection(Socket socket)
        : socket_(std::move(socket))
    {
        Logger::getInstance().info("Connection::Connection");
        async_receive(boost::asio::buffer(buffer_), [this](const boost::system::error_code& error, std::size_t bytes_transferred) {
            Logger::getInstance().info("Connection::async_receive buffer: " + std::to_string(bytes_transferred));
        });
    }

    void async_receive(boost::asio::mutable_buffer buffer, std::function<void(const boost::system::error_code&, std::size_t)> handler)
    {
        socket_.async_receive(buffer, handler);
    }

private:
    Socket socket_;
    boost::asio::mutable_buffer buffer_;
};

#endif // CONNECTION_H
