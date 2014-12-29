/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 */
#include <iostream>
#include <memory>
#include <thread>
#include "http_client.h"
#include "irc_client.h"
#include "connection_manager.h"
#include "echo_delegate.h"

int main(int argc, char** argv)
{
    try {
        connection_manager conn_man;
        std::unique_ptr<irc_client> irc(new irc_client("Test_Moboto", {"#mob"}, "irc.tripadvisor.com", "6667"));
        irc->add_delegate(std::unique_ptr<echo_delegate>(new echo_delegate()));
        conn_man.take_connection(std::move(irc));
        while (conn_man.tick())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    } catch (const ssl_socket_exception & e) {
        std::cerr << e.to_string() << "\n";
        return 1;
    }
    return 0;
}
