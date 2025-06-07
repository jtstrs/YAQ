#ifndef YAQ_H
#define YAQ_H

#include "acceptor.h"
#include <boost/asio/ip/tcp.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

class Yaq {
    struct ConstructorTag {
        explicit ConstructorTag() = default;
    };

public:
    Yaq(const std::string& host, int32_t port, ConstructorTag tag);
    static std::unique_ptr<Yaq> create(const std::unordered_map<std::string, std::string>& config);
    void run();

    void set_accepted_callback(std::function<void(const boost::system::error_code&)> callback);

private:
    using TcpSocket = boost::asio::ip::tcp::socket;
    using TcpAcceptor = Acceptor<boost::asio::ip::tcp::acceptor, TcpSocket>;

    std::function<void(const boost::system::error_code&)> accepted_callback_;

    boost::asio::io_context io_context_;
    TcpAcceptor acceptor_;
    TcpSocket socket_;
    void handle_accept(const boost::system::error_code& error);
};

#endif // YAQ_H