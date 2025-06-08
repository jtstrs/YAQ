#ifndef SOCKET_H
#define SOCKET_H

#include <boost/asio/ip/tcp.hpp>
#include <functional>

template <typename Impl>
class Socket {
public:
    template <typename... Args>
    Socket(Args&&... args)
        : socket_(std::forward<Args>(args)...)
    {
    }

    void async_receive(boost::asio::mutable_buffer buffer, std::function<void(const boost::system::error_code&, std::size_t)> handler)
    {
        socket_.async_receive(buffer, handler);
    }

    bool is_open() const
    {
        return socket_.is_open();
    }

private:
    Impl socket_;
};
#endif // SOCKET_H