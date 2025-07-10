#ifndef YAQ_CLIENT_CLIENT_H
#define YAQ_CLIENT_CLIENT_H

#include "../connection.h"
#include "../socket.h"
#include "json/json.h"
#include <boost/asio/ip/tcp.hpp>
#include <functional>
#include <sstream>
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
        Json::Value command;
        command["command"] = "ping";
        command["args"] = Json::Value(Json::arrayValue);
        connection_.send(writeJsonToString(command));
    }

    void subscribe(const std::string& topic)
    {
        Json::Value command;
        command["command"] = "subscribe";
        command["args"] = Json::Value(Json::arrayValue);
        command["args"].append(topic);
        connection_.send(writeJsonToString(command));
    }

    void unsubscribe(const std::string& topic)
    {
        Json::Value command;
        command["command"] = "unsubscribe";
        command["args"] = Json::Value(Json::arrayValue);
        command["args"].append(topic);
        connection_.send(writeJsonToString(command));
    }

    void topics()
    {
        Json::Value command;
        command["command"] = "topics";
        command["args"] = Json::Value(Json::arrayValue);
        connection_.send(writeJsonToString(command));
    }

    void post(const std::string& topic, const std::string& message)
    {
        Json::Value command;
        command["command"] = "post";
        command["args"] = Json::Value(Json::arrayValue);
        command["args"].append(topic);
        command["args"].append(message);
        connection_.send(writeJsonToString(command));
    }

    void set_on_response_received(std::function<void(const std::string&)> callback)
    {
        connection_.set_on_message_received(callback);
    }

private:
    std::string writeJsonToString(const Json::Value& json)
    {
        Json::StreamWriterBuilder writer;
        writer["indentation"] = ""; // No indentation for compact output
        return Json::writeString(writer, json);
    }

    Connection<Socket> connection_;
};

using YaqClient = YaqClientBase<Socket<boost::asio::ip::tcp::socket>>;

std::unique_ptr<YaqClient> create_yaq_client(boost::asio::io_context& io_context, const std::string& host, const std::string& port);

#endif // YAQ_CLIENT_CLIENT_H