/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 * 
 * Interface for delegates that process messages and respond
 */
#pragma once
#include "message.h"
#include <vector>
#include <string>

class delegate
{
  public:
    virtual std::vector<message> accept_message(const message & from, const std::string & current_nick) = 0;
    virtual std::vector<message> tick() = 0;

    virtual std::vector<address> get_rooms() = 0;
};
