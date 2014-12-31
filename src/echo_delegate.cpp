/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 */
#include "echo_delegate.h"
#include "logging.h"

std::vector<message> echo_delegate::accept_message(const message & from, const std::string & current_nick)
{
    std::vector<message> result;
    if (from.addr.username == current_nick) return result;
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
