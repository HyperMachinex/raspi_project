#ifndef SOCKETCONLIB_H
#define SOCKETCONLIB_H

#include <string>

class SocketCon {
public:
    SocketCon();
    ~SocketCon();

    // is_server: true ise sunucu, false ise istemci
    bool init(bool is_server, const std::string& ip, int port);
    void release();

    bool send(const std::string& message);
    bool receive(std::string& response);

private:
    int socket_fd;
    int conn_fd;
    bool is_server_mode;
    std::string ip_address;
    int port_number;

    void cleanup();
};

#endif // SOCKETCONLIB_H
