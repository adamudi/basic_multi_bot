/*
Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
#pragma once

#include <vector>
#include <functional>
#include "ssl_socket.h"

class reconnecting_ssl_socket
{
  public:
    reconnecting_ssl_socket(const std::string & _host, const std::string & _port);
    reconnecting_ssl_socket(reconnecting_ssl_socket const&) = delete;
    reconnecting_ssl_socket& operator=(reconnecting_ssl_socket const&) = delete;

    reconnecting_ssl_socket& connect();
    void disconnect();

    reconnecting_ssl_socket& write(const std::string & data);
    size_t read(void* buffer, size_t length);

    bool is_secure() const { return sock.is_secure(); }
    
    reconnecting_ssl_socket& make_secure();

    reconnecting_ssl_socket& add_connect_function(std::function<void (reconnecting_ssl_socket &)> callback);

  private:
    void _reconnect();

    ssl_socket sock;
    bool reconnect;
    std::vector<std::function<void (reconnecting_ssl_socket &)> > reconnect_functions;
};
