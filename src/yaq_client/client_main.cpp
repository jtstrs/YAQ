#include "client.h"

int main()
{
    auto client = create_yaq_client("localhost", "8080");
    client->ping();
}