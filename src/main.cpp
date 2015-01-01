/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 */
#include <iostream>
#include <memory>
#include <thread>
#include "http_client.h"
#include "irc_client.h"
#include "xmpp_client.h"
#include "connection_manager.h"
#include "echo_delegate.h"

int main(int argc, char** argv)
{
    std::string xmpp_pw;
    std::cout << "xmpp_pw: ";
    std::cin >> xmpp_pw;
    try {
        connection_manager conn_man;
        std::unique_ptr<irc_client> irc(new irc_client("Test_Moboto", {"#mob"}, "irc.tripadvisor.com", "6667"));
        std::unique_ptr<xmpp_client> xmpp(new xmpp_client("1_13", xmpp_pw, {"1_bot_testing"}, "hipchat.tripadvisorhipchat.com", "conf.btf.hipchat.com", "Mr Moboto", "5222"));
        irc->add_delegate(std::unique_ptr<echo_delegate>(new echo_delegate()));
        xmpp->add_delegate(std::unique_ptr<echo_delegate>(new echo_delegate()));

        conn_man.take_connection(std::move(irc));
        conn_man.take_connection(std::move(xmpp));
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
