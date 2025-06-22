#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include <variant>
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
    std::vector<std::variant<std::string>> args;

    Command(CommandType type, std::vector<std::variant<std::string>> args)
        : type(type)
        , args(args)
    {
    }
};

class Protocol {
public:
    Command parse(const std::string& command);

private:
    CommandType parse_command_type(const std::string& command);
};

#endif // PROTOCOL_H