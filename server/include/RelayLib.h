#ifndef RELAYLIB_H
#define RELAYLIB_H

#include <gpiod.h>
#include <string>

class Relay {
public:
    Relay();
    ~Relay();
    bool init(const std::string& chipname, unsigned int line_number);
    void release();
    bool set(bool value);

private:
    gpiod_chip* chip;
    gpiod_line* line;
    std::string consumer;
};

#endif // RELAYLIB_H
