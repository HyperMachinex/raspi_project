#include "RelayLib.h"
#include <iostream>

Relay::Relay() : chip(nullptr), line(nullptr), consumer("Relay") {}

Relay::~Relay() {
    release();
}

bool Relay::init(const std::string& chipname, unsigned int line_number) {
    chip = gpiod_chip_open_by_name(chipname.c_str());
    if (!chip) {
        std::cerr << "Failed to open GPIO chip\n";
        return false;
    }

    line = gpiod_chip_get_line(chip, line_number);
    if (!line) {
        std::cerr << "Failed to get GPIO line\n";
        return false;
    }

    if (gpiod_line_request_output(line, consumer.c_str(), 0) < 0) {
        std::cerr << "Failed to request line as output\n";
        return false;
    }

    return true;
}

void Relay::release() {
    if (line) gpiod_line_release(line);
    if (chip) gpiod_chip_close(chip);
    line = nullptr;
    chip = nullptr;
}

bool Relay::set(bool value) {
    if (!line) return false;
    return gpiod_line_set_value(line, value) == 0;
}
