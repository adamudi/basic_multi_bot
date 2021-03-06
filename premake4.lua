solution("basic_multi_bot")

configurations({"Debug", "Release"})
flags({"NoRTTI"})
defines({"BUFFERSIZE=1024"})

configuration("Release")
buildoptions({"-O3"})
linkoptions({"-O4"}) -- Link time optimization
flags({"StaticRuntime"})

configuration("Debug")
flags({"Symbols"})

configuration({})

project("b64")
kind("StaticLib")
language("C")
includedirs({"3rdparty/libb64/include"})
files({"3rdparty/libb64/src/cencode.c"
       , "3rdparty/libb64/src/cdecode.c"
})

project("pugixml")
kind("StaticLib")
language("C++")
includedirs({"3rdparty/pugixml/src"})
files({"3rdparty/pugixml/src/pugiconfig.hpp"
       , "3rdparty/pugixml/src/pugixml.cpp"
       , "3rdparty/pugixml/src/pugixml.hpp"
})

project("bmbot")

configuration({"Debug"})
defines({"LOG_MASK=\"(~0)\""}) -- Log everything
configuration({})

kind("ConsoleApp")
language("C++")
buildoptions({"-std=c++14"})
linkoptions({"-pthread"})
links({"ssl", "crypto", "b64", "pugixml"})
includedirs({"src", "3rdparty/libb64/include", "3rdparty/pugixml/src", "3rdparty/rapidjson/include"})
libdirs({"/usr/local/lib"})
defines({"IRC_NAME=\"\\\"Test_Moboto\\\"\""})
files({"src/main.cpp"
       , "src/util.cpp"
       , "src/http_client.cpp"
       , "src/ssl_socket.cpp"
       , "src/irc_client.cpp"
       , "src/xmpp_client.cpp"
       , "src/connection_manager.cpp"
       , "src/xml_buffer.cpp"
       , "src/echo_delegate.cpp"
       , "src/tunnel_delegate.cpp"
})
