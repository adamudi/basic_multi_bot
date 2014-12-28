/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 */
#include "echo_delegate.h"

std::vector<message> echo_delegate::accept_message(const message & from, const std::string & current_nick)
{
    std::vector<message> result;
    address out_address = {from.addr.protocol, from.addr.host, "", from.addr.room};
    message out_message = {out_address, from.body, from.raw};
    result.push_back(out_message);
    return result;
}
