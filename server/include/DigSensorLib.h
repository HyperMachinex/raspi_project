#ifndef DIGSENSORLIB_H
#define DIGSENSORLIB_H

#include <gpiod.h>
#include <string>

class DigSensor {
public:
    DigSensor();
    ~DigSensor();
    bool init(const std::string& chipname, unsigned int line_number);
    void release();
    bool read();

private:
    gpiod_chip* chip;
    gpiod_line* line;
    std::string consumer;
};

#endif // DIGSENSORLIB_H
