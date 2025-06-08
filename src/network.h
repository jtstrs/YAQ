#ifndef NETWORK_H
#define NETWORK_H

#include "acceptor.h"
#include "boost/asio/ip/tcp.hpp"
#include "connection.h"
#include "socket.h"

template <typename AcceptorImpl, typename SocketImpl, typename ClientConnection = Connection<Socket<SocketImpl>>>
class NetworkBase {
public:
    typedef ClientConnection Connection;

    NetworkBase(boost::asio::io_context& io_context, const std::string& host, int32_t port)
        : acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(host), port))
    {
    }

    void run()
    {
        acceptor_.async_accept([this](const boost::system::error_code& error, Socket<SocketImpl> socket) {
            handle_accept(error, std::move(socket));
        });
    }

    void stop()
    {
        acceptor_.cancel();
    }

    void set_on_new_connection(std::function<void(std::unique_ptr<ClientConnection>)> callback)
    {
        on_new_connection_ = callback;
    }

private:
    Acceptor<AcceptorImpl, Socket<SocketImpl>> acceptor_;

    std::function<void(std::unique_ptr<ClientConnection>)> on_new_connection_;

    void handle_accept(const boost::system::error_code& error, Socket<SocketImpl> socket)
    {
        if (error) {
            Logger::getInstance().error("Accept error: " + error.message());
            return;
        }
        Logger::getInstance().info("Accepted connection");
        auto connection = std::make_unique<ClientConnection>(std::move(socket));

        if (on_new_connection_) {
            on_new_connection_(std::move(connection));
        }

        acceptor_.async_accept([this](const boost::system::error_code& error, Socket<SocketImpl> socket) {
            handle_accept(error, std::move(socket));
        });
    }
};

template <typename AcceptorImpl, typename SocketImpl, typename ClientConnection = Connection<Socket<SocketImpl>>>
using Network = NetworkBase<AcceptorImpl, SocketImpl, ClientConnection>;

#endif