/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 */
#pragma once

#include "ssl_socket.h"
#include <string>
#include <sstream>
#include <vector>

class irc_client
{
  public:
    irc_client(const std::string & _nickname, const std::string & _host, const std::string & _port = "6667");

    void tick();
    void connect();
  private:
    void send_message(const std::string & room, const std::string & text);

    std::vector<std::string> read_lines();

    ssl_socket sock;
    std::string nickname;
    std::stringstream stored_buffer;
    char read_buffer[BUFFERSIZE];
    
};
