/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 * 
 * Defines the interface for a connection to a service
 */
#pragma once

#include "message.h"
#include "delegate.h"
#include <vector>
#include <future>

class connection
{
  public:
    virtual std::vector<std::future<std::vector<message> > > tick() = 0;
    virtual void handle_message(const message & m) = 0;
    virtual void connect() = 0;

    virtual connection& add_delegate(std::shared_ptr<delegate> & d) = 0;

  private:
};
