#include "KeypadLib.h"
#include <iostream>
#include <unistd.h>

int main() {
    Keypad keypad;
    keypad.init();

    char keypressed = 'n';
    char prevKeypressed = keypressed;

    while (keypressed != 'A') {
        prevKeypressed = keypressed;
        keypressed = keypad.getKey();

        if (keypressed != 'n' && prevKeypressed == 'n') {
            std::cout << "Key: " << keypressed << std::endl;
        }

        usleep(100000); // 100 ms bekleme
    }

    keypad.release();
    return 0;
}
