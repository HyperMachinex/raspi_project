#include "SocketConLib.h"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

SocketCon::SocketCon()
    : socket_fd(-1), conn_fd(-1), is_server_mode(false), port_number(0) {}

SocketCon::~SocketCon() {
    release();
}

bool SocketCon::init(bool is_server, const std::string& ip, int port) {
    is_server_mode = is_server;
    ip_address = ip;
    port_number = port;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        std::cerr << "Socket oluşturulamadı.\n";
        return false;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());

    if (is_server_mode) {
        if (bind(socket_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
            std::cerr << "Bind başarısız.\n";
            return false;
        }

        if (listen(socket_fd, 1) < 0) {
            std::cerr << "Listen başarısız.\n";
            return false;
        }

        socklen_t len = sizeof(addr);
        conn_fd = accept(socket_fd, (sockaddr*)&addr, &len);
        if (conn_fd < 0) {
            std::cerr << "Accept başarısız.\n";
            return false;
        }

    } 
    else {
        if (connect(socket_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
            std::cerr << "Bağlantı başarısız.\n";
            return false;
        }
        conn_fd = socket_fd;
    }

    return true;
}

void SocketCon::release() {
    cleanup();
}

bool SocketCon::send(const std::string& message) {
    int result = write(conn_fd, message.c_str(), message.length());
    return (result == (int)message.length());
}

bool SocketCon::receive(std::string& response) {
    char buffer[1024] = {0};
    int n = read(conn_fd, buffer, sizeof(buffer));
    if (n <= 0) return false;
    response = std::string(buffer, n);
    return true;
}

void SocketCon::cleanup() {
    if (conn_fd >= 0) close(conn_fd);
    if (socket_fd >= 0 && is_server_mode) close(socket_fd);
    conn_fd = -1;
    socket_fd = -1;
}
