#include "SocketConLib.h"
#include "DigSensorLib.h"
#include "RelayLib.h"
#include "KeypadLib.h"
#include <iostream>

int main() {
    const std::string ip = "192.168.1.107";  // Raspberry Pi IP

    const int port = 7002;

    // Nesne oluştur
    SocketCon socket;
    DigSensor sensor;
    Relay relay;
    Keypad keypad;

    // Donanım başlat
    sensor.init("gpiochip0", 17); // sensör 
    relay.init("gpiochip0", 27);  // relay
    keypad.init();                // default pinler
    std::cout << "[DigitalIONode] Başlatıldı. Port: " << port << std::endl;

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

        if (request == "sensorDurum:") {
            bool durum = sensor.read();
            response = "sensorDurum " + std::to_string(durum) + ":";
        } 
        else if (request == "sensorTip:") {
            response = "sensorTip SICAKLIK:";
        } 
        else if (request.rfind("relay ", 0) == 0) {
            // Örn: "relay 1:"
            if (request.find("1") != std::string::npos) {
                if (relay.set(true)) response = "relay ok:";
                else response = "relay err:";
            } else {
                if (relay.set(false)) response = "relay ok:";
                else response = "relay err:";
            }
        } 
        else if (request == "relayDurum:") {
            // Okuma için rölenin son durumunu takip etmiyorsak sabit yanıt dönebilir
            response = "relay 0:"; 
        } 
        else if (request == "key:") {
            char key = keypad.getKey();
            std::string k(1, key);
            response = "key " + k + ":";
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

    // Kapat
    sensor.release();
    relay.release();
    keypad.release();
    socket.release();

    std::cout << "[DigitalIONode] Kapatıldı.\n";
    return 0;
}
