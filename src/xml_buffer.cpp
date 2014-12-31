/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 */

#include "xml_buffer.h"
#include "logging.h"

std::vector<std::unique_ptr<pugi::xml_document> > xml_buffer::get_lines(ssl_socket & sock)
{
    std::vector<std::unique_ptr<pugi::xml_document> > ret;

    size_t read_bytes = sock.read(read_buffer, BUFFERSIZE);
    line_buff += std::string(read_buffer, read_bytes);
    
    if (line_buff.empty())
        return ret;

    std::unique_ptr<pugi::xml_document> doc(new pugi::xml_document());

    pugi::xml_parse_result result = doc->load_buffer(line_buff.data(), line_buff.size());

    if (!result)
    {
        size_t stream_location = line_buff.find("<stream:stream");
        if (stream_location != std::string::npos)
        {
            // Create a fake stream tag
            std::unique_ptr<pugi::xml_document> fake_doc(new pugi::xml_document());
            pugi::xml_node node = fake_doc->append_child();
            node.set_name("stream");
            ret.push_back(move(fake_doc));
            line_buff = "";
        }
    } else {
        ret.push_back(move(doc));
        line_buff = "";
    }
    
    return ret;
}
