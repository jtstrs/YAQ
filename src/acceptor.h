#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <boost/asio/ip/tcp.hpp>
#include <functional>

template <typename AcceptorImpl, typename SocketImpl>
class Acceptor {
public:
    template <typename... Args>
    explicit Acceptor(Args&&... args)
        : acceptor_impl_(std::forward<Args>(args)...)
    {
    }

    void async_accept(SocketImpl& socket, std::function<void(const boost::system::error_code&)> handler)
    {
        acceptor_impl_.async_accept(socket, handler);
    }

private:
    AcceptorImpl acceptor_impl_;
};

#endif