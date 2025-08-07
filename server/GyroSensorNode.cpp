#include "SocketConLib.h"
#include "GyroLib.h"
#include <iostream>

int main() {
    const std::string ip = "192.168.1.107";  // Raspberry Pi IP

    const int port = 7003;

    SocketCon socket;
    Gyro gyro;

    std::cout << "[GyroSensorNode] Başlatılıyor... Port: " << port << std::endl;

    if (!gyro.init()) {
        std::cerr << "Gyro sensörü başlatılamadı.\n";
        return 1;
    }

    if (!socket.init(true, ip, port)) {
        std::cerr << "Soket başlatılamadı.\n";
        return 1;
    }

    while (true) {
        std::string request;
        if (!socket.receive(request)) {
            std::cerr << "Veri alınamadı.\n";
            break;
        }

        std::string response;

        if (request == "temp:") {
            double temp = gyro.getTemp();
            response = "temp " + std::to_string(temp) + ":";
        } 
        else if (request == "gyro:") {
            double x = gyro.getGyroX();
            double y = gyro.getGyroY();
            double z = gyro.getGyroZ();
            response = "gyro " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + ":";
        } 
        else if (request == "acc:") {
            double x = gyro.getAccX();
            double y = gyro.getAccY();
            double z = gyro.getAccZ();
            response = "acc " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + ":";
        } 
        else if (request == "kapat:") {
            response = "kapat ok:";
            socket.send(response);
            break;
        } 
        else {
            response = "bilinmeyen komut:";
        }

        socket.send(response);
    }

    socket.release();
    gyro.release();
    std::cout << "[GyroSensorNode] Kapatıldı.\n";
    return 0;
}
