/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 */
#pragma once

#include "ssl_socket.h"
#include "connection.h"
#include "delegate.h"
#include "util.h"
#include "xml_buffer.h"
#include <string>
#include <sstream>
#include <vector>

enum connected_state
{
    INIT,
    WAIT_FOR_TAG,
    START_TLS,
    MAKE_SECURE,
    AUTH,
    ENCRYPTED_STREAM,
    SET_RESOURCE,
    IQ,
    PRESENCE,
    CONNECTED
};

class xmpp_client_exception : ssl_socket_exception
{
    using ssl_socket_exception::ssl_socket_exception;
};

class xmpp_client : public connection
{
  public:
    xmpp_client(const std::string & _username, const std::string & _password, const std::vector<std::string> & _rooms, const std::string & _host, const std::string & _muc_host, const std::string & _nickname, const std::string & _port = "5222");

    virtual std::vector<std::future<std::vector<message> > > tick();
    virtual void handle_message(const message & m);
    virtual void connect();

    virtual connection& add_delegate(std::unique_ptr<delegate> && d);

  private:
    void send_message(const std::string & room, const std::string & text);
    std::vector<message> continue_connect();
    std::string get_auth_string();
    std::vector<message> handle_line(const std::unique_ptr<pugi::xml_document> & current_line);

    connected_state state;
    ssl_socket sock;
    std::string username;
    std::string password;
    std::string muc_host;
    std::string nickname;
    std::vector<std::string> rooms;
    std::vector<std::unique_ptr<delegate> > delegates;
    xml_buffer stored_buffer;

    // For connection
    std::string waiting_for_tag;
    connected_state next_state;
};
