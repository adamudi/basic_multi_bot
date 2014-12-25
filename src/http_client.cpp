#include "http_client.h"
#include <tuple>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <openssl/err.h>
#include <sys/ioctl.h>

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

        struct addrinfo* address_info;
        int error = getaddrinfo(host.c_str(), protocol.c_str(), nullptr, &address_info);
        if (error != 0)
        {
            throw std::string("Error getting address info: ") + std::string(gai_strerror(error));
        }
        int connection = socket(address_info->ai_family, address_info->ai_socktype, address_info->ai_protocol);
        if (!connection)
        {
            throw std::string("Unable to open socket");
        }

        // Establish a connection
        error = connect(connection, address_info->ai_addr, address_info->ai_addrlen);
        if (error == -1)
        {
            throw std::string("Unable to connect");
        }
        
        std::string http_query = "GET " + path + " HTTP/1.1\r\n"    \
            "Host: " + host + "\r\n\r\n";
        char buffer[1024];

        send(connection, http_query.c_str(), http_query.size(), 0);
        ssize_t read_size = recv(connection, buffer, 1024, 0);
        return std::string(buffer, read_size);
    }
}