/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 */

#include <regex>
#include "xmpp_client.h"
#include "util.h"
#include "logging.h"

namespace
{

}

xmpp_client::xmpp_client(const std::string & _nickname, const std::string & _password, const std::vector<std::string> & _rooms, const std::string & _host, const std::string & _muc_host, const std::string & _alias, const std::string & _port):
    nickname(_nickname),
    password(_password),
    rooms(_rooms),
    muc_host(_muc_host),
    alias(_alias),
    sock(_host, _port)
{
}

std::vector<std::future<std::vector<message> > > xmpp_client::tick()
{
    std::vector<std::future<std::vector<message> > > result;

    // for (const std::string & line : read_lines())
    // {
    //     result.push_back(std::async(&xmpp_client::handle_line, this, line));
    // }
    for (std::unique_ptr<delegate> & d : delegates)
    {
        result.push_back(std::async(&delegate::tick, d.get()));
    }
    return result;
}

void xmpp_client::handle_message(const message & m)
{
    connect();
    if (m.addr.protocol != "xmpp")
        return;

    if (m.addr.host != sock.get_host())
        return;

    if (m.raw)
    {
        sock.write(m.body);
    } else {
        send_message(m.addr.room, m.body);
    }
}

void xmpp_client::send_message(const std::string & room, const std::string & text)
{
    connect();
    
}

void xmpp_client::connect()
{
    if (sock.is_connected())
    {
        return;
    }

    log(XMPP, "Connecting to XMPP");
    sock.connect();
}

connection& xmpp_client::add_delegate(std::unique_ptr<delegate> && d)
{
    delegates.push_back(std::move(d));
    return *this;
}
