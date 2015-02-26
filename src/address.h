/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 * 
 * Format for addresses to allow for cross-protocol communication
 */
#pragma once
#include <string>
#include <tuple>

struct address
{
    const std::string protocol;
    const std::string host;
    const std::string username;
    const std::string room;
};

inline bool match_except_username(const address & a, const address & b)
{
    return std::tie(a.protocol, a.host, a.room) == std::tie(b.protocol, b.host, b.room);
}
