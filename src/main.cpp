/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 */
#include <iostream>
#include <memory>
#include <thread>
#include <map>
#include "http_client.h"
#include "irc_client.h"
#include "xmpp_client.h"
#include "connection_manager.h"
#include "echo_delegate.h"
#include "tunnel_delegate.h"

std::map<std::string, std::vector<std::string> > get_protocol_hosts(const std::string & protocol, const std::vector<std::shared_ptr<delegate> > & delegates)
{
    std::map<std::string, std::vector<std::string> > hosts;
    for (auto & d : delegates)
    {
        for (const address & addr : d->get_rooms())
        {
            if (addr.protocol != protocol) continue;
            hosts[addr.host].push_back(addr.room);
        }
    }
    return hosts;
}

int main(int argc, char** argv)
{
    std::string xmpp_pw;
    std::cout << "xmpp_pw: ";
    std::cin >> xmpp_pw;
    try {
        connection_manager conn_man;
        std::vector<std::shared_ptr<delegate> > delegates = {
            // std::shared_ptr<delegate>(new echo_delegate()),
            std::shared_ptr<delegate>(new tunnel_delegate()),
        };
        for (auto & irc_host : get_protocol_hosts("irc", delegates))
        {
            std::unique_ptr<irc_client> irc(new irc_client("Test_Moboto", irc_host.second, irc_host.first, "6667"));
            for (auto d : delegates)
            {
                irc->add_delegate(d);
            }
            conn_man.take_connection(std::move(irc));
        }
        for (auto & xmpp_host : get_protocol_hosts("xmpp", delegates))
        {
            std::unique_ptr<xmpp_client> xmpp(new xmpp_client("1_13", xmpp_pw, xmpp_host.second, xmpp_host.first, "conf.btf.hipchat.com", "Mr Moboto", "5222"));
            for (auto d : delegates)
            {
                xmpp->add_delegate(d);
            }
            conn_man.take_connection(std::move(xmpp));
        }

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
