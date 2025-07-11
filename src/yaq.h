#ifndef YAQ_H
#define YAQ_H

#include "acceptor.h"
#include "connection.h"
#include "logger.h"
#include "network.h"
#include "protocol.h"
#include "socket.h"
#include <boost/asio/ip/tcp.hpp>
#include <cstdint>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>

template <typename Network, typename Protocol>
class YaqBase {
    struct ConstructorTag {
        explicit ConstructorTag() = default;
    };

public:
    YaqBase(const std::string& host, int32_t port, ConstructorTag tag)
        : network_(io_context_, host, port)
    {
        network_.set_on_new_connection([this](std::shared_ptr<typename Network::Connection> connection) {
            handle_new_connection(connection);
        });
    }

    static std::unique_ptr<YaqBase<Network, Protocol>> create(const std::unordered_map<std::string, std::string>& config)
    {
        try {
            const std::string host = config.at("host");
            const int32_t port = std::stoi(config.at("port"));
            return std::make_unique<YaqBase<Network, Protocol>>(host, port, ConstructorTag());
        } catch (const std::exception& e) {
            throw std::runtime_error(e.what());
        }
    }

    void run()
    {
        network_.run();
        io_context_.run();
    }

    void stop()
    {
        network_.stop();
        io_context_.stop();
    }

private:
    void handle_new_connection(std::shared_ptr<typename Network::Connection> connection)
    {
        Logger::getInstance().info("New connection");
        connection->set_on_message_received([this](const std::string& message) {
            handle_message(connections_pool_.back(), message);
        });
        connections_pool_.push_back(connection);
    }

    void handle_message(const std::shared_ptr<typename Network::Connection>& connection, const std::string& message)
    {
        Logger::getInstance().info("Message: " + message);
        Command command;
        try {
            command = protocol_.parse(message);
        } catch (const std::exception& e) {
            Logger::getInstance().error("Error parsing message: " + std::string(e.what()));
        }
        switch (command.type) {
        case CommandType::Subscribe:
            handle_subscribe(connection, command.args[0]);
            break;
        case CommandType::Unsubscribe:
            handle_unsubscribe(connection, command.args[0]);
            break;
        case CommandType::PostMessage:
            handle_post_message(connection, command.args[0], command.args[1]);
            break;
        case CommandType::Ping:
            handle_ping(connection);
            break;
        case CommandType::Topic:
            handle_topic(connection);
            break;
        case CommandType::Unknown:
            break;
        }
    }

    void handle_ping(const std::shared_ptr<typename Network::Connection>& connection)
    {
        connection->send("PONG");
    }

    void handle_subscribe(const std::shared_ptr<typename Network::Connection>& connection, const std::string& topic)
    {
        topic_subscriptions_[topic].push_back(connection);
    }

    void handle_unsubscribe(const std::shared_ptr<typename Network::Connection>& connection, const std::string& topic)
    {
        auto& topic_connections = topic_subscriptions_[topic];
        auto subscribed_client = std::find_if(topic_connections.begin(), topic_connections.end(), [connection](const auto& weak_connection) {
            if (weak_connection.expired()) {
                return false;
            }
            auto locked_connection = weak_connection.lock();
            return locked_connection == connection;
        });

        if (subscribed_client != topic_connections.end()) {
            topic_connections.erase(subscribed_client);
        }
    }

    void handle_topic(const std::shared_ptr<typename Network::Connection>& connection)
    {
        std::stringstream topics_builder;
        std::for_each(topic_subscriptions_.begin(), topic_subscriptions_.end(), [&topics_builder](const auto& topic_connections) {
            topics_builder << topic_connections.first << ",";
        });
        connection->send("TOPIC " + topics_builder.str());
    }

    void handle_post_message(const std::shared_ptr<typename Network::Connection>& connection, const std::string& topic, const std::string& message)
    {
        Logger::getInstance().info("Posting message to topic: " + topic + " - " + message);

        auto topic_it = topic_subscriptions_.find(topic);
        if (topic_it == topic_subscriptions_.end()) {
            Logger::getInstance().info("No subscribers for topic: " + topic);
            return;
        }

        // Build the message to send to subscribers
        std::string post_message = "POST " + topic + " " + message;

        // Send message to all subscribers of this topic
        auto& topic_connections = topic_it->second;
        auto it = topic_connections.begin();
        while (it != topic_connections.end()) {
            if (it->expired()) {
                // Remove expired connections
                it = topic_connections.erase(it);
            } else {
                auto locked_connection = it->lock();
                if (locked_connection) {
                    Logger::getInstance().info("Sending message to subscriber: " + post_message);
                    locked_connection->send(post_message);
                }
                ++it;
            }
        }

        // Clean up empty topic if no subscribers remain
        if (topic_connections.empty()) {
            topic_subscriptions_.erase(topic_it);
        }
    }

    boost::asio::io_context io_context_;
    Network network_;
    Protocol protocol_;

    std::vector<std::shared_ptr<typename Network::Connection>> connections_pool_;
    std::unordered_map<std::string, std::vector<std::weak_ptr<typename Network::Connection>>> topic_subscriptions_;
};

// Use boost::asio::ip::tcp::acceptor by default
using Yaq = YaqBase<Network<boost::asio::ip::tcp::acceptor, boost::asio::ip::tcp::socket>, Protocol>;

#endif // YAQ_H