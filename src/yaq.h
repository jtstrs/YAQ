#ifndef YAQ_H
#define YAQ_H

#include "acceptor.h"
#include "connection.h"
#include "logger.h"
#include "socket.h"
#include <boost/asio/ip/tcp.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

template <typename AcceptorImpl, typename SocketImpl, typename ClientConnection = Connection<Socket<SocketImpl>>>
class YaqBase {
    struct ConstructorTag {
        explicit ConstructorTag() = default;
    };

public:
    YaqBase(const std::string& host, int32_t port, ConstructorTag tag)
        : acceptor_(io_context_, boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(host), port))
    {
    }

    static std::unique_ptr<YaqBase<AcceptorImpl, SocketImpl, ClientConnection>> create(const std::unordered_map<std::string, std::string>& config)
    {
        try {
            const std::string host = config.at("host");
            const int32_t port = std::stoi(config.at("port"));
            return std::make_unique<YaqBase<AcceptorImpl, SocketImpl, ClientConnection>>(host, port, ConstructorTag());
        } catch (const std::exception& e) {
            throw std::runtime_error(e.what());
        }
    }

    void run()
    {
        acceptor_.async_accept([this](const boost::system::error_code& error, Socket<SocketImpl> socket) {
            handle_accept(error, std::move(socket));
        });
        io_context_.run();
    }

    void set_accepted_callback(std::function<void(const boost::system::error_code&)> callback)
    {
        accepted_callback_ = callback;
    }

private:
    using TcpAcceptor = Acceptor<AcceptorImpl, Socket<SocketImpl>>;
    std::function<void(const boost::system::error_code&)> accepted_callback_;

    boost::asio::io_context io_context_;
    TcpAcceptor acceptor_;

    std::vector<std::unique_ptr<ClientConnection>> connections_;

    void handle_accept(const boost::system::error_code& error, Socket<SocketImpl> socket)
    {
        if (accepted_callback_) {
            accepted_callback_(error);
        }

        if (error) {
            Logger::getInstance().error("Accept error: " + error.message());
            return;
        }
        Logger::getInstance().info("Accepted connection");
        auto connection = std::make_unique<ClientConnection>(std::move(socket));
        connections_.push_back(std::move(connection));
    }
};

// Use boost::asio::ip::tcp::acceptor by default
using Yaq = YaqBase<boost::asio::ip::tcp::acceptor, boost::asio::ip::tcp::socket>;

#endif // YAQ_H