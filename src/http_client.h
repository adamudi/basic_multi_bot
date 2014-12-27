#pragma once

#include <string>
#include "util.h"

namespace http_client
{
    std::string get(const std::string & address, u8 retry = 5);
    std::string get(const std::string & address, const std::tuple<std::string, std::string> & auth, u8 retry = 5);
}
