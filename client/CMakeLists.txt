cmake_minimum_required(VERSION 3.10)
project(ClientNodeApp)

set(CMAKE_CXX_STANDARD 17)

# Header dosyalarının bulunduğu dizini belirt
include_directories(include)

# Uygulama hedefi: ClientNode
add_executable(ClientNode
    ClientNode.cpp
    src/SocketConLib.cpp
)

# Ek kütüphane gerekmez (sadece std + sockets)
