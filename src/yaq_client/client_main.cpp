#include "client.h"
#include <boost/asio/post.hpp>
#include <iostream>
#include <thread>

int main(int32_t argc, char** argv)
{
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <host> <port>" << std::endl;
        return 1;
    }

    boost::asio::io_context io_context;
    auto client = create_yaq_client(io_context, argv[1], argv[2]);
    client->set_on_response_received([](const std::string& response) {
        std::cout << "Response: " << response << std::endl;
    });

    std::thread thread([&io_context]() {
        io_context.run();
    });

    std::string command;
    while (true) {
        std::getline(std::cin, command);
        if (command == "exit") {
            break;
        }
        if (command == "ping") {
            boost::asio::post(io_context, [&client]() {
                client->ping();
            });
        }
    }

    io_context.stop();
    thread.join();
}