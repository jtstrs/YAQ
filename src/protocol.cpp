#include "protocol.h"
#include "utils.h"
#include <algorithm>

constexpr std::string_view COMMAND_FIELD_KEY = "command";
constexpr std::string_view ARGS_FIELD_KEY = "args";

Command Protocol::parse(const std::string& command)
{
    if (command.empty()) {
        throw std::runtime_error("Command is empty");
    }

    Json::CharReaderBuilder builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    Json::Value root;
    std::string errors;

    const bool success = reader->parse(command.c_str(), command.c_str() + command.size(), &root, &errors);
    if (!success) {
        throw std::runtime_error("Failed to parse command. Parsing error: " + errors);
    }

    validate(root);

    return extract_command(root);
}

Command Protocol::extract_command(const Json::Value& root)
{
    const std::string command = utils::to_lower(root.get(COMMAND_FIELD_KEY.data(), "").asString());
    const CommandType command_type = parse_command_type(command);

    if (command_type == CommandType::Unknown) {
        throw std::runtime_error("Unknown command type: " + command);
    }

    const Json::Value args_storage = root.get(ARGS_FIELD_KEY.data(), "");

    std::vector<std::string> args;
    args.reserve(args_storage.size());

    std::transform(args_storage.begin(), args_storage.end(), std::back_inserter(args), [](const Json::Value& value) {
        if (!value.isString()) {
            return value.asString();
        } else if (value.isInt()) {
            return std::to_string(value.asInt());
        } else if (value.isDouble()) {
            return std::to_string(value.asDouble());
        } else if (value.isBool()) {
            return std::to_string(value.asBool());
        }
        throw std::runtime_error("Command args is not convertible to string");
    });

    return Command(command_type, args);
}

void Protocol::validate(const Json::Value& root)
{
    if (root.empty()) {
        throw std::runtime_error("Command is empty");
    }

    if (!root.isMember(COMMAND_FIELD_KEY.data())) {
        throw std::runtime_error("Command type is missing");
    }

    if (!root.get(COMMAND_FIELD_KEY.data(), "").isString()) {
        throw std::runtime_error("Command type is not a string");
    }

    if (!root.isMember(ARGS_FIELD_KEY.data())) {
        throw std::runtime_error("Command args is missing");
    }

    if (!root.get(ARGS_FIELD_KEY.data(), "").isArray()) {
        throw std::runtime_error("Command args is not an array");
    }
}

CommandType Protocol::parse_command_type(const std::string& command)
{
    if (command == "subscribe") {
        return CommandType::Subscribe;
    } else if (command == "unsubscribe") {
        return CommandType::Unsubscribe;
    } else if (command == "topics") {
        return CommandType::Topic;
    } else if (command == "post") {
        return CommandType::PostMessage;
    } else if (command == "ping") {
        return CommandType::Ping;
    } else {
        return CommandType::Unknown;
    }
    throw std::runtime_error("Unknown command type: " + command);
}