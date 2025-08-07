cmake_minimum_required(VERSION 3.10)
project(RaspberryPiProcesses)

set(CMAKE_CXX_STANDARD 17)

# Header dosyalarının bulunduğu dizini belirt
include_directories(include)

# Kaynak dizin
set(SRC_DIR src)

# Ortak kullanılan kaynaklar
set(COMMON_SOURCES
    ${SRC_DIR}/SocketConLib.cpp
    ${SRC_DIR}/DigSensorLib.cpp
    ${SRC_DIR}/RelayLib.cpp
    ${SRC_DIR}/KeypadLib.cpp
    ${SRC_DIR}/GyroLib.cpp
)

# DigitalIONode (GPIO + GyroLib kullandığı için wiringPi + gpiod gerek)
add_executable(DigitalIONode
    DigitalIONode.cpp
    ${COMMON_SOURCES}
)
target_link_libraries(DigitalIONode gpiod wiringPi)

# GyroSensorNode (GyroLib kullandığı için wiringPi gerek)
add_executable(GyroSensorNode
    GyroSensorNode.cpp
    ${SRC_DIR}/SocketConLib.cpp
    ${SRC_DIR}/GyroLib.cpp
)
target_link_libraries(GyroSensorNode wiringPi)

# ServerNode (sadece soket kullanır, wiringPi gerekmez)
add_executable(ServerNode
    ServerNode.cpp
    ${SRC_DIR}/SocketConLib.cpp
)
