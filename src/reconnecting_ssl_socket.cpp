#include "reconnecting_ssl_socket.h"

reconnecting_ssl_socket::reconnecting_ssl_socket(const std::string & _host, const std::string & _port):
    sock(_host, _port),
    reconnect(false)
{}

reconnecting_ssl_socket& reconnecting_ssl_socket::connect()
{
    reconnect = true;
    _reconnect();
    return *this;
}

void reconnecting_ssl_socket::disconnect()
{
    reconnect = false;
    sock.disconnect();
}

reconnecting_ssl_socket& reconnecting_ssl_socket::write(const std::string & data)
{
    _reconnect();
    sock.write(data);
    return *this;
}

size_t reconnecting_ssl_socket::read(void* buffer, size_t length)
{
    _reconnect();
    return sock.read(buffer, length);
}

reconnecting_ssl_socket& reconnecting_ssl_socket::make_secure()
{
    _reconnect();
    sock.make_secure();
    return *this;
}

void reconnecting_ssl_socket::_reconnect()
{
    if (!reconnect || sock.is_connected())
    {
        return;
    }
    
    sock.connect();
    for (const std::function<void (reconnecting_ssl_socket &)> & callback : reconnect_functions)
    {
        callback(*this);
    }
}

reconnecting_ssl_socket& reconnecting_ssl_socket::add_connect_function(std::function<void (reconnecting_ssl_socket &)> & callback)
{
    reconnect_functions.push_back(callback);
    return *this;
}
