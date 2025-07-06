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
    }

    void set_on_message_received(std::function<void(const std::string&)> callback)
    {
        on_message_received_ = callback;
    }

    void accepted()
    {
        Logger::getInstance().info("Connection accepted");
        async_receive(boost::asio::buffer(buffer_), [this](const boost::system::error_code& error, std::size_t bytes_transferred) {
            on_receive(error, bytes_transferred);
        });
    }

    void send(const std::string& message)
    {
        Logger::getInstance().info("Connection::send message: " + message);
        async_send(boost::asio::buffer(message), [this](const boost::system::error_code& error, std::size_t bytes_transferred) {
            if (error) {
                Logger::getInstance().error("Connection::send error: " + error.message());
            }
            Logger::getInstance().info("Connection::send " + std::to_string(bytes_transferred) + " bytes");
        });
    }

private:
    std::function<void(const std::string&)> on_message_received_;

    void async_send(boost::asio::const_buffer buffer, std::function<void(const boost::system::error_code&, std::size_t)> handler)
    {
        socket_.async_send(buffer, handler);
    }

    void async_receive(boost::asio::mutable_buffer buffer, std::function<void(const boost::system::error_code&, std::size_t)> handler)
    {
        socket_.async_receive(buffer, handler);
    }

    void on_receive(const boost::system::error_code& error, std::size_t bytes_transferred)
    {
        if (error == boost::asio::error::eof) {
            Logger::getInstance().info("Connection graceful shutdown by peer");
            return;
        }

        if (error) {
            Logger::getInstance().error("Connection::async_receive error: " + error.message());
            return;
        }

        Logger::getInstance().info("Connection::async_receive buffer: " + std::string(buffer_.data(), bytes_transferred));

        if (on_message_received_) {
            on_message_received_(std::string(buffer_.data(), bytes_transferred));
        }

        async_receive(boost::asio::buffer(buffer_), [this](const boost::system::error_code& error, std::size_t bytes_transferred) {
            on_receive(error, bytes_transferred);
        });
    }

    Socket socket_;
    std::vector<char> buffer_;
};

#endif // CONNECTION_H
