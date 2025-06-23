#ifndef MOCKS_H
#define MOCKS_H

#include "../src/acceptor.h"
#include "../src/protocol.h"
#include <string_view>

struct FakeProtocol {
    Command parse(const std::string& message)
    {
        return Command();
    }
};

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

    void set_on_message_received(std::function<void(const std::string&)> handler)
    {
    }
};

struct DefaultMessageSource {
    static constexpr std::string_view message = "";
};

template <typename MessageSource = DefaultMessageSource>
struct FakeSocketSuccessReceiveMessage {
    int32_t received_count = 0;
    std::string_view received_message = MessageSource::message;

    void async_receive(boost::asio::mutable_buffer buffer, std::function<void(const boost::system::error_code&, std::size_t)> handler)
    {
        if (received_count == 2) {
            return;
        }
        // To check receive after receive is called
        received_count++;
        boost::asio::buffer_copy(buffer, boost::asio::buffer(received_message), received_message.size());
        handler(boost::system::error_code(), received_message.size());
    }

    bool is_open() const
    {
        return true;
    }

    void close()
    {
    }
};

using FakeSocketReceiveSuccess = FakeSocketSuccessReceiveMessage<DefaultMessageSource>;

struct FakeSocketReceiveEof {
    void async_receive(boost::asio::mutable_buffer buffer, std::function<void(const boost::system::error_code&, std::size_t)> handler)
    {
        handler(boost::asio::error::eof, 0);
    }

    bool is_open() const
    {
        return true;
    }

    void close()
    {
    }
};

struct FakeSocketReceiveError {
    void async_receive(boost::asio::mutable_buffer buffer, std::function<void(const boost::system::error_code&, std::size_t)> handler)
    {
        handler(boost::asio::error::not_connected, 0);
    }

    bool is_open() const
    {
        return true;
    }

    void close()
    {
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
