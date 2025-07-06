#include "protocol.h"
#include "utils.h"

#include <sstream>
#include <vector>

std::string to_string(CommandType type)
{
    switch (type) {
    case CommandType::Subscribe:
        return "SUBSCRIBE";
    case CommandType::Unsubscribe:
        return "UNSUBSCRIBE";
    case CommandType::Topic:
        return "TOPICS";
    case CommandType::PostMessage:
        return "POST";
    case CommandType::Ping:
        return "PING";
    case CommandType::Unknown:
        return "UNKNOWN";
    }
    throw std::runtime_error("Invalid command type");
}

Command Protocol::parse(const std::string& command)
{
    std::string token;
    std::stringstream ss(command);

    CommandType type = CommandType::Unknown;
    std::vector<std::string> args;

    // Read the first token (command type)
    if (std::getline(ss, token, ',')) {
        token = utils::trim(token);
        if (!token.empty()) {
            type = parse_command_type(token);
        }
    }

    if (type == CommandType::Unknown) {
        throw std::runtime_error("Invalid command");
    }

    // Read remaining tokens (arguments)
    while (std::getline(ss, token, ',')) {
        token = utils::trim(token);
        if (!token.empty()) {
            args.push_back(token);
        }
    }

    if (type == CommandType::Subscribe || type == CommandType::Unsubscribe) {
        if (args.size() != 1) {
            throw std::runtime_error("Mismatch number of arguments for command: " + to_string(type));
        }
    }

    if (type == CommandType::PostMessage) {
        if (args.size() != 2) {
            throw std::runtime_error("Mismatch number of arguments for command: " + to_string(type));
        }
    }

    if (type == CommandType::Ping || type == CommandType::Topic) {
        if (args.size() != 0) {
            throw std::runtime_error("Mismatch number of arguments for command: " + to_string(type));
        }
    }

    return Command(type, args);
}

CommandType Protocol::parse_command_type(const std::string& command)
{
    if (command == "SUBSCRIBE") {
        return CommandType::Subscribe;
    } else if (command == "UNSUBSCRIBE") {
        return CommandType::Unsubscribe;
    } else if (command == "TOPICS") {
        return CommandType::Topic;
    } else if (command == "POST") {
        return CommandType::PostMessage;
    } else if (command == "PING") {
        return CommandType::Ping;
    } else {
        return CommandType::Unknown;
    }
}
