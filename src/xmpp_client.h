/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 */
#pragma once

#include "ssl_socket.h"
#include "connection.h"
#include "delegate.h"
#include <string>
#include <sstream>
#include <vector>

class xmpp_client : public connection
{
  public:
    xmpp_client(const std::string & _nickname, const std::string & _password, const std::vector<std::string> & _rooms, const std::string & _host, const std::string & _muc_host, const std::string & _alias, const std::string & _port = "5222");

    virtual std::vector<std::future<std::vector<message> > > tick();
    virtual void handle_message(const message & m);
    virtual void connect();

    virtual connection& add_delegate(std::unique_ptr<delegate> && d);

  private:
    void send_message(const std::string & room, const std::string & text);

    ssl_socket sock;
    std::string nickname;
    std::string password;
    std::string muc_host;
    std::string alias;
    std::vector<std::string> rooms;
    std::vector<std::unique_ptr<delegate> > delegates;
    std::string stored_buffer;
    char read_buffer[BUFFERSIZE];
};
