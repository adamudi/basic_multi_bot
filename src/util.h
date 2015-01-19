/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 */
#pragma once

#include <cinttypes>
#include <tuple>
#include <string>
#include <algorithm>
#include <sstream>

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int_fast8_t fs8;
typedef int_fast16_t fs16;
typedef int_fast32_t fs32;
typedef int_fast64_t fs64;
typedef uint_fast8_t fu8;
typedef uint_fast16_t fu16;
typedef uint_fast32_t fu32;
typedef uint_fast64_t fu64;

#define maxs8 INT8_MAX
#define maxs16 INT16_MAX
#define maxs32 INT32_MAX
#define maxs64 INT64_MAX
#define maxu8 UINT8_MAX
#define maxu16 UINT16_MAX
#define maxu32 UINT32_MAX
#define maxu64 UINT64_MAX
#define maxfs8 INT_FAST8_MAX
#define maxfs16 INT_FAST16_MAX
#define maxfs32 INT_FAST32_MAX
#define maxfs64 INT_FAST64_MAX
#define maxfu8 UINT_FAST8_MAX
#define maxfu16 UINT_FAST16_MAX
#define maxfu32 UINT_FAST32_MAX
#define maxfu64 UINT_FAST64_MAX

std::tuple<std::string, std::string> split(const std::string & haystack, const std::string & needle);
std::string base64encode(const std::string & decoded);
std::string base64decode(const std::string & encoded);
std::string get_env(const std::string & name);

inline std::string & ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

inline std::string & rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

inline std::string & trim(std::string &s) {
    return ltrim(rtrim(s));
}

inline std::string replace_all(const std::string & haystack, const std::string & needle, const std::string & replacement = "")
{
    std::stringstream out;
    size_t offset = 0;
    while (true)
    {
        size_t needle_pos = haystack.find(needle, offset);
        if (needle_pos == std::string::npos)
        {
            out << haystack.substr(offset);
            break;
        }
        
        out << haystack.substr(offset, needle_pos - offset) << replacement;
        offset = needle_pos + needle.size();
    }
    return out.str();
}
