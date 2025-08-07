#include "SocketConLib.h"
#include <iostream>
#include <thread>
#include <chrono>

void print_menu() {
    std::cout << "\n===== Menü =====\n";
    std::cout << "1 - Sensör durumu göster\n";
    std::cout << "2 - Röleyi aç/kapat\n";
    std::cout << "3 - Keypad'ten tuş oku\n";
    std::cout << "4 - Gyro verilerini göster\n";
    std::cout << "5 - Ivmeölçer verilerini göster\n";
    std::cout << "6 - Sıcaklık verisini göster\n";
    std::cout << "7 - Otomatik denetim modu\n";
    std::cout << "0 - Çıkış\n";
    std::cout << "Seçiminiz: ";
}

int main() {
    SocketCon socket;
    const std::string ip = "192.168.1.107";  // ServerNode IP (Raspberry Pi)

    const int port = 7001;

    if (!socket.init(false, ip, port)) {
        std::cerr << "ServerNode bağlantısı kurulamadı.\n";
        return 1;
    }

    bool running = true;
    while (running) {
        print_menu();
        int secim;
        std::cin >> secim;

        std::string response;

        switch (secim) {
            case 1:
                socket.send("sensorDurum:");
                socket.receive(response);
                std::cout << ">> " << response << "\n";
                break;

            case 2:
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

            case 3:
                socket.send("key:");
                socket.receive(response);
                std::cout << ">> " << response << "\n";
                break;

            case 4:
                socket.send("gyro:");
                socket.receive(response);
                std::cout << ">> " << response << "\n";
                break;

            case 5:
                socket.send("acc:");
                socket.receive(response);
                std::cout << ">> " << response << "\n";
                break;

            case 6:
                socket.send("temp:");
                socket.receive(response);
                std::cout << ">> " << response << "\n";
                break;

            case 7: {
                std::cout << "Otomatik denetim başlatılıyor...\n";
                std::cout << "Sensör aktif olduğunda röle açılsın mı? (1=Evet, 0=Hayır): ";
                int hedef;
                std::cin >> hedef;

                bool auto_mode = true;
                while (auto_mode) {
                    socket.send("sensorDurum:");
                    socket.receive(response);
                    bool durum = response.find("1") != std::string::npos;

                    if (durum == (hedef == 1)) {
                        socket.send("relay 1:");
                    } else {
                        socket.send("relay 0:");
                    }
                    socket.receive(response); // relay cevabı
                    std::cout << "[Auto] Röle kontrol edildi: " << response << "\n";

                    std::cout << "Devam etmek için bekleniyor... (Çıkmak için 'e' girin): ";
                    std::string c;
                    std::cin >> c;
                    if (c == "e") auto_mode = false;
                }
                break;
            }

            case 0:
                socket.send("kapat:");
                socket.receive(response);
                std::cout << ">> " << response << "\n";
                running = false;
                break;

            default:
                std::cout << "Geçersiz seçim!\n";
        }
    }

    socket.release();
    std::cout << "Uygulama sonlandı.\n";
    return 0;
}
