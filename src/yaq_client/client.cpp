#include "client.h"

std::unique_ptr<YaqClient> create_yaq_client(const std::string& host, const std::string& port)
{
    static boost::asio::io_context io_context;
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, port);
    boost::asio::ip::tcp::socket socket(io_context);
    socket.connect(endpoints);

    return std::make_unique<YaqClient>(std::move(socket));
}
