#include "yaq.h"
#include "boost/asio/ip/address.hpp"
#include <iostream>
#include <memory>

std::unique_ptr<Yaq> Yaq::create(const std::unordered_map<std::string, std::string>& config)
{
    try {
        const std::string host = config.at("host");
        const int32_t port = std::stoi(config.at("port"));
        return std::make_unique<Yaq>(host, port, Yaq::ConstructorTag());
    } catch (const std::exception& e) {
        throw std::runtime_error(e.what());
    }
}

Yaq::Yaq(const std::string& host, int32_t port, ConstructorTag tag)
    : io_context_()
    , acceptor_(io_context_, boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(host), port))
    , socket_(io_context_)
{
}

void Yaq::run()
{
    acceptor_.async_accept(socket_, [this](const boost::system::error_code& error) {
        handle_accept(error);
    });
    io_context_.run();
}

void Yaq::handle_accept(const boost::system::error_code& error)
{
    if (error) {
        std::cerr << "Accept error: " << error.message() << std::endl;
        return;
    }
    std::cout << "Accepted connection" << std::endl;
}
