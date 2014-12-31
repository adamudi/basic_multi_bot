/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 */

#include <regex>
#include <tuple>
#include <string>
#include "xmpp_client.h"
#include "util.h"
#include "logging.h"

namespace
{
    /**
     * Parse out the id, host, and name from a JID in the format
     * id@host/name
     */
    std::tuple<std::string, std::string, std::string> parse_jid(const std::string & jid)
    {
        std::string id, host, name, remaining;
        tie(id, remaining) = split(jid, "@");
        tie(host, name) = split(jid, "/");
        return make_tuple(id, host, name);
    }
}

xmpp_client::xmpp_client(const std::string & _username, const std::string & _password, const std::vector<std::string> & _rooms, const std::string & _host, const std::string & _muc_host, const std::string & _nickname, const std::string & _port):
    username(_username),
    password(_password),
    rooms(_rooms),
    muc_host(_muc_host),
    nickname(_nickname),
    sock(_host, _port),
    state(INIT)
{
}

std::vector<std::future<std::vector<message> > > xmpp_client::tick()
{
    std::vector<std::future<std::vector<message> > > result;
    connect();
    
    if (state != CONNECTED)
    {
        slog(XMPP, "continue_connect");
        result.push_back(std::async(&xmpp_client::continue_connect, this));
        return result;
    }

    std::vector<std::unique_ptr<pugi::xml_document> > lines = stored_buffer.get_lines(sock);
    for (std::unique_ptr<pugi::xml_document> & current_line : lines)
    {
        result.push_back(std::async(&xmpp_client::handle_line, this, std::move(current_line)));
    }
    for (std::unique_ptr<delegate> & d : delegates)
    {
        result.push_back(std::async(&delegate::tick, d.get()));
    }
    return result;
}

void xmpp_client::handle_message(const message & m)
{
    connect();
    if (m.addr.protocol != "xmpp")
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

void xmpp_client::send_message(const std::string & room, const std::string & text)
{
    connect();
    std::unique_ptr<pugi::xml_document> fake_doc(new pugi::xml_document());
    pugi::xml_node message = fake_doc->append_child();
    message.set_name("message");
    std::string full_room = room + "@" + muc_host;
    message.append_attribute("to") = full_room.c_str();
    std::string full_user = username + "@" + sock.get_host() + "/" + nickname;
    message.append_attribute("from") = full_user.c_str();
    message.append_attribute("type") = "groupchat";
    pugi::xml_node body = message.append_child();
    body.set_name("body");
    body.text().set(text.c_str());

    std::stringstream buffer;
    fake_doc->print(buffer);

    sock.write(buffer.str());
}

void xmpp_client::connect()
{
    if (sock.is_connected())
    {
        return;
    }

    log(XMPP, "Connecting to XMPP");
    sock.connect();
    state = INIT;
}

connection& xmpp_client::add_delegate(std::unique_ptr<delegate> && d)
{
    delegates.push_back(std::move(d));
    return *this;
}

std::vector<message> xmpp_client::continue_connect()
{
    slog(XMPP, "state: " << state);
    std::vector<message> ret;
    address addr = {"xmpp", sock.get_host(), nickname, ""};
    switch (state)
    {
      case WAIT_FOR_TAG:
      {
          slog(XMPP, "Waiting for " << waiting_for_tag);
          std::vector<std::unique_ptr<pugi::xml_document> > lines = stored_buffer.get_lines(sock);
          for (std::unique_ptr<pugi::xml_document> & tag : lines)
          {
              log(XMPP, "Checking tag");
              if (tag->child(waiting_for_tag.c_str()))
              {
                  state = next_state;
                  break;
              }
          }
      }
      break;
      case INIT:
      {
          message m = {addr, "<stream:stream xmlns:stream='http://etherx.jabber.org/streams' xmlns='jabber:client' to='" + sock.get_host() + "' version='1.0'>", true};
          ret.push_back(m);
        
          waiting_for_tag = "stream";
          state = WAIT_FOR_TAG;
          next_state = START_TLS;
      }
      break;
      case START_TLS:
      {
          message m = {addr, "<starttls xmlns='urn:ietf:params:xml:ns:xmpp-tls'/>", true};
          ret.push_back(m);

          waiting_for_tag = "proceed";
          state = WAIT_FOR_TAG;
          next_state = MAKE_SECURE;
      }
      break;
      case MAKE_SECURE:
      {
          sock.make_secure();
          message m = {addr, "<stream:stream xmlns:stream='http://etherx.jabber.org/streams' xmlns='jabber:client' to='" + sock.get_host() + "' version='1.0'>", true};
          ret.push_back(m);

          waiting_for_tag = "stream";
          state = WAIT_FOR_TAG;
          next_state = AUTH;
      }
      break;
      case AUTH:
      {
          message m = {addr, "<auth xmlns='urn:ietf:params:xml:ns:xmpp-sasl' mechanism='PLAIN'>" + get_auth_string() + "</auth>", true};
          ret.push_back(m);
          
          waiting_for_tag = "success";
          state = WAIT_FOR_TAG;
          next_state = ENCRYPTED_STREAM;
      }
      break;
      case ENCRYPTED_STREAM:
      {
          message m = {addr, "<stream:stream xmlns:stream='http://etherx.jabber.org/streams' xmlns='jabber:client' to='" + sock.get_host() + "' version='1.0'>", true};
          ret.push_back(m);
          
          waiting_for_tag = "stream";
          state = WAIT_FOR_TAG;
          next_state = SET_RESOURCE;
      }
      break;
      case SET_RESOURCE:
      {
          message m = {addr, "<iq type='set' id='H_0'><bind xmlns='urn:ietf:params:xml:ns:xmpp-bind'><resource>" + nickname + "</resource></bind></iq>", true};
          ret.push_back(m);
          
          waiting_for_tag = "iq";
          state = WAIT_FOR_TAG;
          next_state = IQ;
      }
      break;
      case IQ:
      {
          message m = {addr, "<iq type='set' id='H_1'><session xmlns='urn:ietf:params:xml:ns:xmpp-session'/></iq>", true};
          ret.push_back(m);
          
          waiting_for_tag = "iq";
          state = WAIT_FOR_TAG;
          next_state = PRESENCE;
      }
      break;
      case PRESENCE:
      {
          for (const std::string & room : {"1_bot_testing"/*, "1_mobile_web", "1_api", "1_#trdev"*/})
          {
              message m = {addr, "<presence to='" + room + "@" + muc_host + "/" + nickname + "'><x xmlns='http://jabber.org/protocol/muc'/></presence>", true};
              ret.push_back(m);
          }
          state = CONNECTED;
      }
      break;
      case CONNECTED:
        elog(XMPP, "Already connected");
        break;
    }
    return ret;
}

std::string xmpp_client::get_auth_string()
{
    std::string original = " " + username + " " + password;
    original[0] = '\0';
    original[username.size()+1] = '\0';
    return base64encode(original);
}

std::vector<message> xmpp_client::handle_line(const std::unique_ptr<pugi::xml_document> & current_line)
{
    std::vector<message> result;
    pugi::xml_node message = current_line->child("message");
    if (!message || message.child("delay")) return result;
    std::string room, sender, ignore;
    tie(room, ignore, sender) = parse_jid(message.attribute("from").value());
    std::string text = message.child_value("body");
    if (!text.empty())
    {
        address addr = {"xmpp", sock.get_host(), sender, room};
        struct message m = {addr, text, false};
        for (std::unique_ptr<delegate> & d : delegates)
        {
            for (const struct message & new_message : d->accept_message(m, nickname))
            {
                result.push_back(new_message);
            }
        }
    }
    return result;
}
