#include <iostream>
#include <unistd.h>
#include "DigSensorLib.h"
#include "RelayLib.h"

int main() {
    const std::string chipname = "gpiochip0";
    const unsigned int SENSOR_PIN = 17; // Giriş pini
    const unsigned int RELAY_PIN = 27;  // Çıkış pini

    DigSensor sensor;
    Relay relay;

    // Başlat
    if (!sensor.init(chipname, SENSOR_PIN)) {
        std::cerr << "Sensor initialization failed!" << std::endl;
        return 1;
    }

    if (!relay.init(chipname, RELAY_PIN)) {
        std::cerr << "Relay initialization failed!" << std::endl;
        return 1;
    }

    while (true) {
        bool sensorValue = sensor.read();
        if (!sensorValue) {
            std::cout << "Relay is closed..." << std::endl;
            relay.set(true);
        } else {
            std::cout << "Relay is open..." << std::endl;
            relay.set(false);
        }
        usleep(100000); // 100 ms bekle
    }

    // Serbest bırak (normalde bu noktaya gelinmez, ama temsilen bırakıldı)
    sensor.release();
    relay.release();

    return 0;
}
