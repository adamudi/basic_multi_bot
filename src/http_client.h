/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 */
#pragma once

#include <string>
#include "util.h"
#include <vector>

namespace http_client
{
    class http_client_exception
    {
      public:
        http_client_exception(const std::string & _msg):
            msg(_msg)
        {}

        const std::string& to_string() const {return msg;}

      private:
        std::string msg;
    };

    std::tuple<std::string, std::string> get(const std::string & address, u8 retry = 5);
    std::tuple<std::string, std::string> get(const std::string & address, const std::vector<std::tuple<std::string, std::string> > & headers, u8 retry = 5);

    std::tuple<std::string, std::string> auth(const std::string username, const std::string password);
}
