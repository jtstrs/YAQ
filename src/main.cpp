#include "yaq.h"
#include <iostream>

int main(int32_t argc, char** argv)
{
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <host> <port>" << std::endl;
        return 1;
    }

    auto yaq = Yaq::create({ { "host", argv[1] }, { "port", argv[2] } });
    yaq->run();
    return 0;
}