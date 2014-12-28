/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 * 
 * Defines the interface for a connection to a service
 */
#include "message.h"
#include <vector>
#include <future>

class connection
{
  public:
    virtual std::vector<std::future<std::vector<message> > > tick() = 0;
    virtual void connect() = 0;

  private:
};
