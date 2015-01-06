/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 */

#include "tunnel_delegate.h"
#include <algorithm>

namespace
{
    struct tunnel_pair
    {
        const address left;
        const address right;
    };

    tunnel_pair tunnel_pairs[] = {
        {{"irc", "irc.tripadvisor.com", "", "#mob"}, {"xmpp", "hipchat.tripadvisorhipchat.com", "", "1_bot_testing"}},
        {{"irc", "irc.tripadvisor.com", "", "#tr-dev"}, {"xmpp", "hipchat.tripadvisorhipchat.com", "", "1_#trdev"}},
        {{"irc", "irc.tripadvisor.com", "", "#mobile"}, {"xmpp", "hipchat.tripadvisorhipchat.com", "", "1_mobile_web"}},
    };

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

    std::vector<address> get_destinations(const address & origin)
    {
        std::vector<address> dests;

        for (const tunnel_pair & p : tunnel_pairs)
        {
            if (p.left.protocol == origin.protocol &&
                p.left.host == origin.host &&
                p.left.room == origin.room)
            {
                dests.push_back(p.right);
            }
            if (p.right.protocol == origin.protocol &&
                p.right.host == origin.host &&
                p.right.room == origin.room)
            {
                dests.push_back(p.left);
            }
        }

        return dests;
    }
}

std::vector<message> tunnel_delegate::accept_message(const message & from, const std::string & current_nick)
{
    std::vector<message> result;
    if (from.addr.username == current_nick || !match_protocol_and_room(from.addr, get_rooms()) || from.body.find("!priv") != std::string::npos)
        return result;

    for (const address & addr : get_destinations(from.addr))
    {
        address out_address = {addr.protocol, addr.host, from.addr.username, addr.room};
        message out_message = {out_address, from.addr.username + ": " + from.body, from.raw};
        result.push_back(out_message);
    }
    
    return result;
}

std::vector<message> tunnel_delegate::tick()
{
    std::vector<message> result;
    
    return result;
}

std::vector<address> tunnel_delegate::get_rooms()
{
    std::vector<address> ret;
    for (const tunnel_pair & p : tunnel_pairs)
    {
        ret.push_back(p.left);
        ret.push_back(p.right);
    }
    return ret;
    
}
