/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 */
#include "util.h"
#include "b64/decode.h"
#include "b64/encode.h"
#include <sstream>

/**
 * Split the haystack at most 1 time
 * 
 * @return A tuple containing the text preceding the needle and the
 * text succeeding the needle. If the needle is not found then the
 * tuple will contain the full haystack and an empty string
 */
std::tuple<std::string, std::string> split(const std::string & haystack, const std::string & needle)
{
    size_t at = haystack.find(needle);
    if (at == std::string::npos) return std::make_tuple(haystack, "");
    
    return std::make_tuple(haystack.substr(0, at), haystack.substr(at + needle.size()));
}

std::string base64decode(const std::string & encoded)
{
    std::stringstream source(encoded);
    std::stringstream result;
    base64::decoder dec;
    dec.decode(source, result);
    return result.str();
}

std::string base64encode(const std::string & decoded)
{
    std::stringstream source(decoded);
    std::stringstream result;
    base64::encoder enc;
    enc.encode(source, result);
    std::string out = result.str();
    trim(out);
    return out;
}

std::string get_env(const std::string & name)
{
    char* value = getenv(name.c_str());
    if (value == nullptr) return "";
    return std::string(value);
}

