/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 * 
 * Contains all the clients to every IRC and XMPP server
 */
#pragma once

#include <vector>
#include <memory>
#include "address.h"
#include "irc_client.h"

class connection_manager
{
  public:
    connection_manager& take_connection(std::unique_ptr<connection> && new_connection);
    bool tick();

  private:
    std::vector<std::unique_ptr<connection> > clients;
};
