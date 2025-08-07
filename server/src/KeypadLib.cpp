#include "KeypadLib.h"
#include <iostream>
#include <unistd.h>

Keypad::Keypad() : chipName("gpiochip0"), chip(nullptr) {}

Keypad::~Keypad() {
    release();
}

void Keypad::init() {
    chip = gpiod_chip_open_by_name(chipName.c_str());
    if (!chip) {
        std::cerr << "Failed to open chip\n";
        return;
    }

    for (int i = 0; i < 4; i++)
        lineKeyCol[i] = gpiod_chip_get_line(chip, keyGPIOCol[i]);

    for (int i = 0; i < 4; i++)
        lineKeyRow[i] = gpiod_chip_get_line(chip, keyGPIORow[i]);

    for (int i = 0; i < 4; i++)
        gpiod_line_request_output(lineKeyCol[i], "COL", 0);

    for (int i = 0; i < 4; i++)
        gpiod_line_request_input(lineKeyRow[i], "ROW");
}

char Keypad::getKey() {
    for (int i = 0; i < 4; i++) {
        gpiod_line_set_value(lineKeyCol[i], 1);
        for (int j = 0; j < 4; j++) {
            if (gpiod_line_get_value(lineKeyRow[j]) == 1) {
                gpiod_line_set_value(lineKeyCol[i], 0);
                return key[j][i];
            }
        }
        gpiod_line_set_value(lineKeyCol[i], 0);
    }
    return 'n'; // No key pressed
}

void Keypad::release() {
    for (int i = 0; i < 4; i++) {
        if (lineKeyCol[i]) gpiod_line_release(lineKeyCol[i]);
        if (lineKeyRow[i]) gpiod_line_release(lineKeyRow[i]);
    }

    if (chip) gpiod_chip_close(chip);
    chip = nullptr;
}

void Keypad::setChipName(const std::string& ch) {
    chipName = ch;
}

void Keypad::setKeyGPIOCol(int col1, int col2, int col3, int col4) {
    keyGPIOCol[0] = col1;
    keyGPIOCol[1] = col2;
    keyGPIOCol[2] = col3;
    keyGPIOCol[3] = col4;
}

void Keypad::setKeyGPIORow(int row1, int row2, int row3, int row4) {
    keyGPIORow[0] = row1;
    keyGPIORow[1] = row2;
    keyGPIORow[2] = row3;
    keyGPIORow[3] = row4;
}
