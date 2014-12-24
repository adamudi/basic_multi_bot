#include <iostream>
#include "http_client.h"

int main(int argc, char** argv)
{
    std::cout << http_client::get("http://fizz.buzz/");
    return 0;
}
