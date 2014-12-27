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

project("bmbot")
kind("ConsoleApp")
language("C++")
buildoptions({"-std=c++11"})
linkoptions({"-pthread"})
links({"ssl", "crypto", "b64"})
includedirs({"src", "3rdparty/libb64/include"})
libdirs({"/usr/local/lib"})
files({"src/main.cpp"
       , "src/util.cpp"
       , "src/http_client.cpp"
       , "src/ssl_socket.cpp"
})

