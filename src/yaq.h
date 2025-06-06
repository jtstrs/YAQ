#ifndef YAQ_H
#define YAQ_H

#include "boost/asio/ip/tcp.hpp"
#include <boost/asio.hpp>
#include <boost/asio/ip/address.hpp>
#include <cstdint>
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

private:
    void handle_accept(const boost::system::error_code& error);

    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket socket_;
};

#endif // YAQ_H