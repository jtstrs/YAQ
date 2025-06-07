#ifndef MOCKS_H
#define MOCKS_H

#include "../src/acceptor.h"

struct FakeSocketImpl {
    template <typename... Args>
    FakeSocketImpl(Args&&... args)
    {
    }
};

template <typename SocketImpl>
struct FakeAcceptorSuccessImpl {
    template <typename... Args>
    FakeAcceptorSuccessImpl(Args&&... args)
    {
    }

    void async_accept(SocketImpl& socket, std::function<void(const boost::system::error_code&)> handler)
    {
        handler(boost::system::error_code());
    }
};

template <typename SocketImpl>
struct FakeAcceptorErrorImpl {
    template <typename... Args>
    FakeAcceptorErrorImpl(Args&&... args)
    {
    }

    void async_accept(SocketImpl& socket, std::function<void(const boost::system::error_code&)> handler)
    {
        handler(boost::system::error_code(boost::asio::error::connection_aborted));
    }
};

#endif // MOCKS_H
