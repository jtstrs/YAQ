#include "yaq.h"
int main()
{
    auto yaq = Yaq::create({ { "host", "0.0.0.0" }, { "port", "1111" } });
    yaq->run();
    return 0;
}