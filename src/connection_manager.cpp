/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 */
#include "connection_manager.h"
#include <iostream>

connection_manager& connection_manager::take_connection(std::unique_ptr<connection> && new_connection)
{
    clients.push_back(std::move(new_connection));
    return *this;
}

bool connection_manager::tick()
{
    try
    {
        std::vector<std::future<std::vector<message> > > messages_to_process;
        // READ
        for (std::unique_ptr<connection> & conn : clients)
        {
            std::vector<std::future<std::vector<message> > > new_messages = conn->tick();
            for (std::future<std::vector<message> > & entry : new_messages)
            {
                messages_to_process.push_back(std::move(entry));
            }
        }

        // WRITE
        for (std::future<std::vector<message> > & messages_to_send : messages_to_process)
        {
            for (const message & m : messages_to_send.get())
            {
                for (std::unique_ptr<connection> & conn : clients)
                {
                    conn->handle_message(m);
                }
            }
        }
    } catch (const ssl_socket_exception & e) {
        std::cerr << e.to_string() << "\n";
        return false;
    }

    return true;
}
