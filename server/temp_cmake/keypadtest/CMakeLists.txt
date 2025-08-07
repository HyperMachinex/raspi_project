cmake_minimum_required(VERSION 3.10)
project(KeypadTest)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(KeypadTest
    KeypadTest.cpp
    src/KeypadLib.cpp
)

target_link_libraries(KeypadTest gpiod)
