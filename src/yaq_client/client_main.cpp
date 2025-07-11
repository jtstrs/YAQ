#include "client.h"
#include <boost/asio/post.hpp>
#include <iostream>
#include <string>
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
    std::string arg1;
    std::string arg2;

    while (true) {
        std::string buffer;
        std::getline(std::cin, buffer);

        std::stringstream ss(buffer);
        ss >> command >> arg1 >> arg2;

        if (command == "exit") {
            break;
        }

        if (command == "ping") {
            boost::asio::post(io_context, [&client]() {
                client->ping();
            });
        }
        if (command == "subscribe" && !arg1.empty()) {
            boost::asio::post(io_context, [&client, arg1]() {
                client->subscribe(arg1);
            });
        }
        if (command == "unsubscribe" && !arg1.empty()) {
            boost::asio::post(io_context, [&client, arg1]() {
                client->unsubscribe(arg1);
            });
        }
        if (command == "topics") {
            boost::asio::post(io_context, [&client]() {
                client->topics();
            });
        }
        if (command == "post" && !arg1.empty() && !arg2.empty()) {
            boost::asio::post(io_context, [&client, arg1, arg2]() {
                client->post(arg1, arg2);
            });
        }
    }

    io_context.stop();
    thread.join();
}