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

class irc_client : public connection
{
  public:
    irc_client(const std::string & _nickname, const std::vector<std::string> & _rooms, const std::string & _host, const std::string & _port = "6667");

    virtual std::vector<std::future<std::vector<message> > > tick();
    virtual void handle_message(const message & m);
    virtual void connect();

    virtual connection& add_delegate(std::shared_ptr<delegate> & d);
  private:
    void send_message(const std::string & room, const std::string & text);

    std::vector<std::string> read_lines();
    std::vector<message> handle_line(const std::string & line);

    ssl_socket sock;
    std::string nickname;
    std::vector<std::string> rooms;
    std::vector<std::shared_ptr<delegate> > delegates;
    std::string stored_buffer;
    char read_buffer[BUFFERSIZE];
    
};
