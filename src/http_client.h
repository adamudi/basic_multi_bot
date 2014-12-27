#pragma once

#include <string>
#include "util.h"

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

    std::string get(const std::string & address, u8 retry = 5);
    std::string get(const std::string & address, const std::tuple<std::string, std::string> & auth, u8 retry = 5);
}
