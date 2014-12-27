#include "reconnecting_ssl_socket.h"

reconnecting_ssl_socket::reconnecting_ssl_socket(const std::string & _host, const std::string & _port):
    sock(_host, _port),
    reconnect(false)
{}

reconnecting_ssl_socket& reconnecting_ssl_socket::connect()
{
    sock.connect();
    reconnect = true;
    return *this;
}

void reconnecting_ssl_socket::disconnect()
{
    reconnect = false;
    sock.disconnect();
}

reconnecting_ssl_socket& reconnecting_ssl_socket::write(const std::string & data)
{
    sock.write(data);
    return *this;
}

size_t reconnecting_ssl_socket::read(void* buffer, size_t length)
{
    return sock.read(buffer, length);
}

reconnecting_ssl_socket& reconnecting_ssl_socket::make_secure()
{
    sock.make_secure();
    return *this;
}

void reconnecting_ssl_socket::_reconnect()
{
    if (!reconnect || sock.is_connected())
    {
        return;
    }
}
