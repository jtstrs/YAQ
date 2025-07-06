#include "client.h"
#include <boost/asio/connect.hpp>

std::unique_ptr<YaqClient> create_yaq_client(boost::asio::io_context& io_context, const std::string& host, const std::string& port)
{
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, port);
    boost::asio::ip::tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);

    return std::make_unique<YaqClient>(std::move(socket));
}
