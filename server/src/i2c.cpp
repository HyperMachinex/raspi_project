#include "../include/i2c.h"

int read_word(int device_handle, int address){
    int value;
    value = wiringPiI2CReadReg8(device_handle, address);
    value = value << 8;
    value += wiringPiI2CReadReg8(device_handle, address+1);
    if (value >= 0x8000)
        value = -(65536 - value);
    return value;
}