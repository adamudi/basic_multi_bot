#include "http_client.h"
#include <tuple>

namespace
{
    /**
     * Break a URL down into its components
     * 
     * @param url The URL to parse
     * @returns tuple(protocol http/https, host, path, query, port)
     */
    std::tuple<std::string, std::string, std::string, std::string, u16> parse_url(const std::string & url)
    {
        std::string protocol;
        std::string host;
        std::string path;
        std::string query;
        u16 port;
        
        std::string remaining;
        tie(protocol, remaining) = split(url, "://"); // Grab the protocol
        if (remaining.find("/") < remaining.find(":")) // If there is no port
        {
            port = -1;
            tie(host, remaining) = split(remaining, "/");
        } else { // There is a port
            std::string temp_port;
            tie(host, remaining) = split(remaining, ":");
            tie(temp_port, remaining) = split(remaining, "/");
            port = atoi(temp_port.c_str());
        }
        // Get the path and query
        tie(path, query) = split(remaining, "?");
        
        path = "/" + path;
        return std::make_tuple(protocol, host, path, query, port);
    }
}

namespace http_client
{
    /**
     * Perform an HTTP request and return the result
     * 
     * @param address The URL to request
     * @param retry Number of times to try again on failed attempts
     */
    std::string get(const std::string & address, u8 retry)
    {
        std::string protocol;
        std::string host;
        std::string path;
        std::string query;
        u16 port;
        tie(protocol, host, path, query, port) = parse_url(address);
        return protocol + " | " + host + " | " + std::to_string(port) + " | " + path + " | " + query + "\n";
    }
}
