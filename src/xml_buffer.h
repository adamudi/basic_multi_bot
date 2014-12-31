/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 * 
 * Buffers socket reads to get xml tags
 */
#pragma once
#include "ssl_socket.h"
#include <string>
#include <vector>
#include <memory>
#include "pugixml.hpp"

class xml_buffer
{
  public:
    std::vector<std::unique_ptr<pugi::xml_document> > get_lines(ssl_socket & sock);
    
  private:
    std::string line_buff;
    char read_buffer[BUFFERSIZE];
};
