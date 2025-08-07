#include "SocketConLib.h"
#include <iostream>
#include <thread>
#include <chrono>

void print_menu() {
    std::cout << "\n===== Menu =====\n";
    std::cout << "1 - Sensör durumu göster\n";
    std::cout << "2 - Röleyi ac/kapat\n";
    std::cout << "3 - Keypad'ten tuş oku\n";
    std::cout << "4 - Gyro verilerini göster\n";
    std::cout << "5 - Ivmeölcer verilerini göster\n";
    std::cout << "6 - Sicaklik verisini göster\n";
    std::cout << "7 - Otomatik denetim modu\n";
    std::cout << "0 - Cikis\n";
    std::cout << "Seciminiz: ";
}

int main() {
    SocketCon socket;
    const std::string ip = "192.168.1.107";  // ServerNode IP (Raspberry Pi)

    const int port = 7001;

    if (!socket.init(false, ip, port)) {
        std::cerr << "ServerNode bbaglantisi kurulamadi.\n";
        return 1;
    }

    bool running = true;
    while (running) {
        print_menu();
        int secim;
        std::cin >> secim;

        std::string response;

        switch (secim) {
            case 1:     // sensor durumu prinnt
                socket.send("sensorDurum:");
                socket.receive(response);
                std::cout << ">> " << response << "\n";
                break;

            case 2:     // relay control
                socket.send("relayDurum:");
                socket.receive(response);
                std::cout << ">> Mevcut Durum: " << response << "\n";
                std::cout << "Açmak için 1, kapatmak için 0 girin: ";
                int val;
                std::cin >> val;
                socket.send("relay " + std::to_string(val) + ":");
                socket.receive(response);
                std::cout << ">> " << response << "\n";
                break;

            case 3: {       // keypad okuma
                std::string full_key_sequence;
                while (true) {
                    socket.send("key:");
                    socket.receive(response);
                    std::string key = response.substr(4, response.find(":") - 4);
                    if (key == "A") {
                        break;
                    }
                    if (key != "n") {
                        full_key_sequence += key;
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
                }           
                std::cout << "KEY: " << full_key_sequence << "\n";
                break;
            }       // gyro degerleri
            case 4:
                socket.send("gyro:");
                socket.receive(response);
                std::cout << ">> " << response << "\n";
                break;

            case 5:         // ivme degerleri
                socket.send("acc:");
                socket.receive(response);
                std::cout << ">> " << response << "\n";
                break;

            case 6:     // sicaklik degerleri
                socket.send("temp:");
                socket.receive(response);
                std::cout << ">> " << response << "\n";
                break;

            case 7: {           // sensro 1 veya 0da relay acilip kapaniyor
                std::cout << "Basit GPIO davranisi: Sensör 0 ise röle acilir, sensör 1 ise kapatilir.\n";
                std::cout << "Cikmak için 'e' tuslayin.\n";

                bool auto_mode = true;
                std::thread input_thread([&]() {
                    std::string input;
                    while (true) {
                        std::cin >> input;
                        if (input == "e") {
                            auto_mode = false;
                            break;
                        }
                    }
                });

                while (auto_mode) {
                    socket.send("sensorDurum:");
                    socket.receive(response);

                    bool sensor_state = response.find("1") != std::string::npos;
                    std::cout << "[DEBUG] sensor_state: " << sensor_state << "\n";

                    if (!sensor_state) {
                        std::cout << "[Client] Sensör pasif. Röle aciliyor...\n";
                        socket.send("relay 1:");
                    } else {
                        std::cout << "[Client] Sensör aktif. Röle kapaniyor...\n";
                        socket.send("relay 0:");
                    }

                    socket.receive(response);
                    std::cout << ">> Röle durumu: " << response << "\n";

                    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
                }

                input_thread.join();
                break;
            }

            case 0:         // exit
                socket.send("kapat:");
                socket.receive(response);
                std::cout << ">> " << response << "\n";
                running = false;
                break;

            default:
                std::cout << "Gecersiz secim!\n";
        }
    }

    socket.release();
    std::cout << "Uygulama sonlandi.\n";
    return 0;
}
