/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 * 
 * Format for addresses to allow for cross-protocol communication
 */
#pragma once
#include <string>

struct address
{
    const std::string protocol;
    const std::string host;
    const std::string username;
    const std::string room;
};
