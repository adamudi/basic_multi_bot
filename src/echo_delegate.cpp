/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 */
#include "echo_delegate.h"
#include "logging.h"

namespace
{
    bool match_protocol_and_room(const address & needle, const std::vector<address> & haystack)
    {
        for (const address & addr : haystack)
        {
            if (addr.protocol == needle.protocol && addr.host == needle.host)
            {
                return true;
            }
        }
        return false;
    }
}

std::vector<message> echo_delegate::accept_message(const message & from, const std::string & current_nick)
{
    std::vector<message> result;
    if (from.addr.username == current_nick || !match_protocol_and_room(from.addr, get_rooms()))
        return result;

    address out_address = {from.addr.protocol, from.addr.host, "", from.addr.room};
    message out_message = {out_address, from.body, from.raw};
    result.push_back(out_message);
    return result;
}

std::vector<message> echo_delegate::tick()
{
    std::vector<message> result;
    
    return result;
}

std::vector<address> echo_delegate::get_rooms()
{
    return {
        {"irc", "irc.dev.tripadvisor.com", "", "#mob"},
        {"xmpp", "hipchat.tripadvisorhipchat.com", "", "1_bot_testing"}            
    };
}
