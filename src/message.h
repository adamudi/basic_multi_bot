/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 */
#pragma once

#include "address.h"
#include <string>

struct message
{
    const address addr;
    const std::string & body;
    const bool raw; // Write raw body to the socket
};
