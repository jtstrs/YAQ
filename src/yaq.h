#ifndef YAQ_H
#define YAQ_H

#include "acceptor.h"
#include "connection.h"
#include "logger.h"
#include "network.h"
#include "socket.h"
#include <boost/asio/ip/tcp.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

template <typename Network>
class YaqBase {
    struct ConstructorTag {
        explicit ConstructorTag() = default;
    };

public:
    YaqBase(const std::string& host, int32_t port, ConstructorTag tag)
        : network_(io_context_, host, port)
    {
        network_.set_on_new_connection([this](std::unique_ptr<typename Network::Connection> connection) {
            handle_new_connection(std::move(connection));
        });
    }

    static std::unique_ptr<YaqBase<Network>> create(const std::unordered_map<std::string, std::string>& config)
    {
        try {
            const std::string host = config.at("host");
            const int32_t port = std::stoi(config.at("port"));
            return std::make_unique<YaqBase<Network>>(host, port, ConstructorTag());
        } catch (const std::exception& e) {
            throw std::runtime_error(e.what());
        }
    }

    void run()
    {
        network_.run();
        io_context_.run();
    }

    void stop()
    {
        network_.stop();
        io_context_.stop();
    }

private:
    void handle_new_connection(std::unique_ptr<typename Network::Connection> connection)
    {
        Logger::getInstance().info("New connection");
        connection->accepted();
        connections_.push_back(std::move(connection));
    }

    std::vector<std::unique_ptr<typename Network::Connection>> connections_;
    boost::asio::io_context io_context_;
    Network network_;
};

// Use boost::asio::ip::tcp::acceptor by default
using Yaq = YaqBase<Network<boost::asio::ip::tcp::acceptor, boost::asio::ip::tcp::socket>>;

#endif // YAQ_H