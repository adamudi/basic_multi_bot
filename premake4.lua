solution("basic_multi_bot")

configurations({"Debug", "Release"})
flags({"NoRTTI"})

configuration("Release")
buildoptions({"-O3"})
linkoptions({"-O4"}) -- Link time optimization
flags({"StaticRuntime"})

configuration("Debug")
flags({"Symbols"})

configuration({})

project("bmbottest")
kind("ConsoleApp")
language("C++")
buildoptions({"-std=c++11"})
linkoptions({"-pthread"})
links({"ssl", "crypto"})
includedirs({"src"})
libdirs({"/usr/local/lib"})
files({"src/main.cpp"
       , "src/util.cpp"
       , "src/http_client.cpp"
       , "src/ssl_socket.cpp"
})

