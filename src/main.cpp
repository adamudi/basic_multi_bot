/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 */
#include <iostream>
#include <memory>
#include "http_client.h"
#include "irc_client.h"

int main(int argc, char** argv)
{
    try {
        // std::cout << http_client::get("http://fizz.buzz/");
        std::unique_ptr<irc_client> irc(new irc_client("Test_Moboto", "irc.tripadvisor.com", "6667"));
        irc->connect();
    } catch (const ssl_socket_exception & e) {
        std::cerr << e.to_string() << "\n";
        return 1;
    }
    return 0;
}
