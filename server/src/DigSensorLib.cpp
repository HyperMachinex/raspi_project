#include "DigSensorLib.h"
#include <iostream>

DigSensor::DigSensor() : chip(nullptr), line(nullptr), consumer("DigSensor") {}

DigSensor::~DigSensor() {
    release();
}

bool DigSensor::init(const std::string& chipname, unsigned int line_number) {
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

    if (gpiod_line_request_input(line, consumer.c_str()) < 0) {
        std::cerr << "Failed to request line as input\n";
        return false;
    }

    return true;
}

void DigSensor::release() {
    if (line) gpiod_line_release(line);
    if (chip) gpiod_chip_close(chip);
    line = nullptr;
    chip = nullptr;
}

bool DigSensor::read() {
    if (!line) return false;
    return gpiod_line_get_value(line) == 1;
}
