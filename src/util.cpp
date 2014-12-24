#include "util.h"

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
