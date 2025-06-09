#ifndef MOCKS_H
#define MOCKS_H

#include "../src/acceptor.h"

struct FakeConnection {
    template <typename... Args>
    FakeConnection(Args&&... args)
    {
    }

    void accepted()
    {
    }

    void async_receive(boost::asio::mutable_buffer buffer, std::function<void(const boost::system::error_code&, std::size_t)> handler)
    {
        handler(boost::system::error_code(), 0);
    }
};

struct FakeSocket {
    template <typename... Args>
    FakeSocket(Args&&... args)
    {
    }

    void async_receive(boost::asio::mutable_buffer buffer, std::function<void(const boost::system::error_code&, std::size_t)> handler)
    {
        handler(boost::system::error_code(), 0);
    }

    bool is_open() const
    {
        return true;
    }

    void close()
    {
    }
};

template <typename Socket>
struct FakeAcceptorSuccess {
    template <typename... Args>
    FakeAcceptorSuccess(Args&&... args)
    {
    }

    void async_accept(std::function<void(const boost::system::error_code&, Socket)> handler)
    {
        static bool already_accepted = false;
        if (already_accepted) {
            return;
        }
        already_accepted = true;
        handler(boost::system::error_code(), Socket());
    }

    void cancel()
    {
    }
};

template <typename Socket>
struct FakeAcceptorError {
    template <typename... Args>
    FakeAcceptorError(Args&&... args)
    {
    }

    void async_accept(std::function<void(const boost::system::error_code&, Socket)> handler)
    {
        handler(boost::system::error_code(boost::asio::error::connection_aborted), Socket());
    }

    void cancel()
    {
    }
};

#endif // MOCKS_H
