#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "json/json.h"
#include <string>
#include <vector>

enum class CommandType {
    Subscribe, // subscribe <topic>
    Unsubscribe, // unsubscribe <topic>
    Topic, // topics
    PostMessage, // post <topic> <message>
    Ping, // ping
    Unknown
};

struct Command {
    CommandType type;
    std::vector<std::string> args;

    Command()
        : type(CommandType::Unknown)
        , args()
    {
    }

    Command(CommandType type, std::vector<std::string> args)
        : type(type)
        , args(args)
    {
    }
};

class Protocol {
public:
    Command parse(const std::string& command);

private:
    Command extract_command(const Json::Value& root);
    void validate(const Json::Value& root);
    CommandType parse_command_type(const std::string& command);
    void validate_argument_count(CommandType command_type, const std::vector<std::string>& args);
};

#endif // PROTOCOL_H