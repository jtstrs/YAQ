#include "client.h"
#include <iostream>

int main(int32_t argc, char** argv)
{
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <host> <port>" << std::endl;
        return 1;
    }

    auto client = create_yaq_client(argv[1], argv[2]);
    client->ping();
    client->set_on_response_received([](const std::string& response) {
        std::cout << "Response: " << response << std::endl;
    });
}