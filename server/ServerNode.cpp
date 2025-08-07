#include "SocketConLib.h"
#include <iostream>

int main() {
    // IP adresi: Raspberry Pi'nin IP’si
    const std::string ip = "192.168.1.107";
    const int client_port = 7001;
    const int digitalio_port = 7002;
    const int gyro_port = 7003;

    // Client'tan gelen bağlantı için server soketi
    SocketCon client_socket;
    if (!client_socket.init(true, ip, client_port)) {
        std::cerr << "[ServerNode] Client bağlantısı kurulamadı.\n";
        return 1;
    }

    // DigitalIONode’a bağlan
    SocketCon digitalio_socket;
    if (!digitalio_socket.init(false, ip, digitalio_port)) {
        std::cerr << "[ServerNode] DigitalIO bağlantısı kurulamadı.\n";
        return 1;
    }

    // GyroSensorNode’a bağlan
    SocketCon gyro_socket;
    if (!gyro_socket.init(false, ip, gyro_port)) {
        std::cerr << "[ServerNode] Gyro bağlantısı kurulamadı.\n";
        return 1;
    }

    std::cout << "[ServerNode] Başlatıldı. Port: " << client_port << "\n";

    while (true) {
        std::string request;
        if (!client_socket.receive(request)) {
            std::cerr << "Client'tan mesaj alınamadı.\n";
            break;
        }

        std::string response;

        // Kapat komutu her iki node’a da iletilir
        if (request == "kapat:") {
            digitalio_socket.send(request);
            gyro_socket.send(request);

            std::string dio_reply, gyro_reply;
            digitalio_socket.receive(dio_reply);
            gyro_socket.receive(gyro_reply);

            response = "kapat ok:";
            client_socket.send(response);
            break;
        }
        // Gyro node’a ait komutlar
        else if (request == "gyro:" || request == "acc:" || request == "temp:") {
            gyro_socket.send(request);
            gyro_socket.receive(response);
        }
        // DigitalIO node’a ait komutlar
        else {
            digitalio_socket.send(request);
            digitalio_socket.receive(response);
        }

        client_socket.send(response);
    }

    client_socket.release();
    digitalio_socket.release();
    gyro_socket.release();

    std::cout << "[ServerNode] Kapatıldı.\n";
    return 0;
}
