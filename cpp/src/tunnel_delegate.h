/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 * 
 * Marshals messages between IRC and XMPP
 */
#pragma once
#include "delegate.h"

class tunnel_delegate : public delegate
{
  public:
    virtual std::vector<message> accept_message(const message & from, const std::string & current_nick);

    virtual std::vector<message> tick();

    virtual std::vector<address> get_rooms();
  private:
};
