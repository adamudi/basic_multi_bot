/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 */
#include "http_client.h"
#include <tuple>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <openssl/err.h>
#include <sys/ioctl.h>
#include <iostream>
#include <sstream>
#include <thread>
#include "ssl_socket.h"
#include "util.h"

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
            port = 0;
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

    std::string read_full_response(ssl_socket & sock)
    {
        std::stringstream out;
        char buffer[BUFFERSIZE];

        while (sock.is_connected())
        {
            size_t length = sock.read(buffer, BUFFERSIZE);
            if (length == 0 && sock.is_connected())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            } else {
                out << std::string(buffer, length);
            }
        }

        return out.str();
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
        return get(address, std::make_tuple("", ""), retry);
    }

    std::string get(const std::string & address, const std::tuple<std::string, std::string> & auth, u8 retry)
    {
        std::string protocol;
        std::string host;
        std::string path;
        std::string query;
        u16 _port;
        tie(protocol, host, path, query, _port) = parse_url(address);
        std::string port = _port == 0 ? protocol : std::to_string(_port);

        ssl_socket sock(host, port);

        std::string http_query = "GET / HTTP/1.1\r\n";
        if (!std::get<0>(auth).empty())
        {
            http_query += "Authorization: Basic " + base64encode(std::get<0>(auth) + ":" + std::get<1>(auth)) + "\r\n";
        }
        http_query += "User-Agent: curl/7.37.0\r\n" \
            "Host: " + host + "\r\n\r\n";

        for (u8 i = 0; i < retry; ++i)
        {
            try
            {
                sock.connect();
                if (protocol == "https")
                {
                    sock.make_secure();
                }
                sock.write(http_query);
                return read_full_response(sock);
            } catch (const ssl_socket_exception & e) {
                std::cerr << e.to_string() << '\n';
            }
        }
        
        throw http_client_exception("Failed after " + std::to_string(retry) + " tries");
    }
}
