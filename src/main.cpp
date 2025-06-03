#include "lib.hpp"
#include <iostream>

int main()
{
    YAQ::Example example;
    std::cout << "2 + 3 = " << example.add(2, 3) << std::endl;
    return 0;
}