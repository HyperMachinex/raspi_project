cmake_minimum_required(VERSION 3.10)
project(SensorRelayTest)

# C++ standardını belirle
set(CMAKE_CXX_STANDARD 17)

# Header dosyalarının bulunduğu dizini belirt
include_directories(include)

# .cpp dosyalarını tek tek tanımla
add_executable(SensorRelayTest
    SensorRelayTest.cpp
    src/DigSensorLib.cpp
    src/RelayLib.cpp
)

# GPIOD kütüphanesiyle linkle
target_link_libraries(SensorRelayTest gpiod)
