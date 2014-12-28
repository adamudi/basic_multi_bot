/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 */

#include <regex>
#include "irc_client.h"
#include "util.h"
#include "logging.h"

namespace
{
    std::tuple<std::string, std::string, std::vector<std::string> > parse_command(const std::string & inp)
    {
        std::string host_header, command;
        std::string current, remaining;
        std::vector<std::string> params;
        std::string final_param;
        if (inp[0] == ':')
        {
            tie(host_header, remaining) = split(inp, " ");
        } else {
            remaining = inp;
        }

        tie(command, remaining) = split(remaining, " ");
        tie(remaining, final_param) = split(remaining, ":");
        while (!remaining.empty())
        {
            tie(current, remaining) = split(remaining, " ");
            params.push_back(current);
        }
        if (!final_param.empty())
            params.push_back(final_param);
        return make_tuple(host_header, command, params);
    }

    const std::regex username_parser(R"XXX(^:([^!]+)!)XXX", std::regex::icase | std::regex::optimize);

    std::string get_username_from_host_header(const std::string & inp)
    {
        std::smatch match;
        regex_search(inp, match, username_parser);
        if (match.size() < 1) return "";

        return match[1].str();
    }
}

irc_client::irc_client(const std::string & _nickname, const std::vector<std::string> & _rooms, const std::string & _host, const std::string & _port):
    nickname(_nickname),
    rooms(_rooms),
    sock(_host, _port)
{
}

std::vector<std::future<std::vector<message> > > irc_client::tick()
{
    std::vector<std::future<std::vector<message> > > result;

    for (const std::string & line : read_lines())
    {
        result.push_back(std::async(&irc_client::handle_line, this, line));
    }
    for (std::unique_ptr<delegate> & d : delegates)
    {
        result.push_back(std::async(&delegate::tick, d.get()));
    }
    return result;
}

std::vector<message> irc_client::handle_line(const std::string & line)
{
    std::vector<message> result;

    std::string host;
    std::string command;
    std::vector<std::string> params;
    tie(host, command, params) = parse_command(line);

    if (command == "PING")
    {
        address addr = {"irc", sock.get_host(), nickname, ""};
        message m = {addr, "PONG :" + params[0] + "\n", true};
        result.push_back(m);
    } else if (command == "MODE") {
        for (const std::string & room : rooms)
        {
            address addr = {"irc", sock.get_host(), nickname, ""};
            message m = {addr, "JOIN :" + room + "\n", true};
            result.push_back(m);
        }
    } else if (command == "PRIVMSG") {
        std::string sender = get_username_from_host_header(host);
        std::string& room = params[0];
        std::string& text = params[1];
        if (!text.empty())
        {
            address addr = {"irc", sock.get_host(), sender, room};
            message m = {addr, text, false};
            for (std::unique_ptr<delegate> & d : delegates)
            {
                for (const message & new_message : d->accept_message(m, nickname))
                {
                    result.push_back(new_message);
                }
            }
        }
    }


    return result;
}

void irc_client::handle_message(const message & m)
{
    connect();
    if (m.addr.protocol != "irc")
        return;

    if (m.addr.host != sock.get_host())
        return;

    if (m.raw)
    {
        sock.write(m.body);
    } else {
        send_message(m.addr.room, m.body);
    }
}

void irc_client::send_message(const std::string & room, const std::string & text)
{
    connect();
    std::string current;
    std::string remaining = text;
    do
    {
        tie(current, remaining) = split(remaining, "\n");
        sock.write("PRIVMSG " + room + " :" + current + "\n");
    } while (!remaining.empty());
}

void irc_client::connect()
{
    if (sock.is_connected())
    {
        return;
    }

    log(IRC, "Connecting to IRC");
    sock.connect();
    sock.write("NICK :" + nickname + "\n");
    sock.write("USER " + nickname + " 0 0 :" + nickname + "\n");
    
    for (const std::string & room : {"#mob", "#mobile", "#rna-dev", "#fancy", "#sitex-dev", "#vr-dev", "#bldev", "#cdsdev", "#tr-dev"})
    {
        sock.write("JOIN :" + room + "\n");
    }
}

std::vector<std::string> irc_client::read_lines()
{
    connect();
    
    std::vector<std::string> lines;
    size_t read_bytes = sock.read(read_buffer, BUFFERSIZE);
    stored_buffer << std::string(read_buffer, read_bytes);

    std::string buffer = stored_buffer.str();
    while (buffer.find('\n') != std::string::npos)
    {
        std::string line;
        std::getline(stored_buffer, line);
        lines.push_back(line);
    }
    return lines;
}

connection& irc_client::add_delegate(std::unique_ptr<delegate> && d)
{
    delegates.push_back(std::move(d));
    return *this;
}
