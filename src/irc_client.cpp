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

irc_client::irc_client(const std::string & _nickname, const std::string & _host, const std::string & _port):
    nickname(_nickname),
    sock(_host, _port)
{
}

void irc_client::tick()
{
    for (const std::string & line : read_lines())
    {

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
