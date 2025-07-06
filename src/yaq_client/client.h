#ifndef YAQ_CLIENT_CLIENT_H
#define YAQ_CLIENT_CLIENT_H

#include "../connection.h"
#include "../socket.h"
#include <boost/asio/ip/tcp.hpp>
#include <functional>
#include <string>

template <typename Socket>
class YaqClientBase {
public:
    YaqClientBase(Socket socket)
        : connection_(std::move(socket))
    {
    }

    void ping()
    {
        connection_.send("PING");
    }

    void set_on_response_received(std::function<void(const std::string&)> callback)
    {
        connection_.set_on_message_received(callback);
    }

private:
    Connection<Socket> connection_;
};

using YaqClient = YaqClientBase<Socket<boost::asio::ip::tcp::socket>>;

std::unique_ptr<YaqClient> create_yaq_client(const std::string& host, const std::string& port);

#endif // YAQ_CLIENT_CLIENT_H