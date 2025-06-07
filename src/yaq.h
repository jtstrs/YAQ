#ifndef YAQ_H
#define YAQ_H

#include "acceptor.h"
#include <boost/asio/ip/tcp.hpp>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

template <typename AcceptorImpl, typename SocketImpl>
class YaqBase {
    struct ConstructorTag {
        explicit ConstructorTag() = default;
    };

public:
    YaqBase(const std::string& host, int32_t port, ConstructorTag tag)
        : acceptor_(io_context_, boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(host), port))
        , socket_(io_context_)
    {
    }

    static std::unique_ptr<YaqBase<AcceptorImpl, SocketImpl>> create(const std::unordered_map<std::string, std::string>& config)
    {
        try {
            const std::string host = config.at("host");
            const int32_t port = std::stoi(config.at("port"));
            return std::make_unique<YaqBase<AcceptorImpl, SocketImpl>>(host, port, ConstructorTag());
        } catch (const std::exception& e) {
            throw std::runtime_error(e.what());
        }
    }

    void run()
    {
        acceptor_.async_accept(socket_, [this](const boost::system::error_code& error) {
            handle_accept(error);
        });
        io_context_.run();
    }

    void set_accepted_callback(std::function<void(const boost::system::error_code&)> callback)
    {
        accepted_callback_ = callback;
    }

private:
    using TcpSocket = SocketImpl;
    using TcpAcceptor = Acceptor<AcceptorImpl, TcpSocket>;

    std::function<void(const boost::system::error_code&)> accepted_callback_;

    boost::asio::io_context io_context_;
    TcpAcceptor acceptor_;
    TcpSocket socket_;

    void handle_accept(const boost::system::error_code& error)
    {
        accepted_callback_(error);
        if (error) {
            std::cerr << "Accept error: " << error.message() << std::endl;
            return;
        }
        std::cout << "Accepted connection" << std::endl;
    }
};

// Use boost::asio::ip::tcp::acceptor by default
using Yaq = YaqBase<boost::asio::ip::tcp::acceptor, boost::asio::ip::tcp::socket>;

#endif // YAQ_H