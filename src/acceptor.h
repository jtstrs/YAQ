#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <boost/asio/ip/tcp.hpp>
#include <functional>

template <typename Impl, typename Socket>
class Acceptor {
public:
    template <typename... Args>
    explicit Acceptor(Args&&... args)
        : acceptor_impl_(std::forward<Args>(args)...)
    {
    }

    void async_accept(std::function<void(const boost::system::error_code&, Socket)> handler)
    {
        acceptor_impl_.async_accept(handler);
    }

    void cancel()
    {
        acceptor_impl_.cancel();
    }

private:
    Impl acceptor_impl_;
};

#endif