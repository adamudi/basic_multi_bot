/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 * 
 * Echos messages it recieves
 */
#pragma once
#include "delegate.h"

class echo_delegate : public delegate
{
  public:
    std::vector<message> accept_message(const message & from, const std::string & current_nick);

    virtual std::vector<message> tick();
  private:
};
