#include "GyroLib.h"
#include <wiringPiI2C.h>
Gyro::Gyro()
    : device_handle(-1), i2c_address(0x68), device_path("/dev/i2c-1") {}

Gyro::~Gyro() {
    release();
}

bool Gyro::init(const std::string& i2c_dev, int address) {
    device_path = i2c_dev;
    i2c_address = address;

    device_handle = open_device(device_path.c_str(), i2c_address);
    if (device_handle < 0) {
        std::cerr << "Failed to open I2C device." << std::endl;
        return false;
    }

    write_byte(device_handle, ACCEL_CONFIG_ADDR, 0x00);
    accel_scale_val = 2;

    write_byte(device_handle, GYRO_CONFIG_ADDR, 0x00);
    gyro_scale_val = 250;

    device_wait(100); // sensor stabilization
    return true;
}

void Gyro::release() {
    if (device_handle >= 0) {
        close_device(device_handle);
        device_handle = -1;
    }
}

double Gyro::getAccX() {
    int16_t raw = read16BitRegister(ACCEL_XOUT_H_ADDR);
    return scaleAccel(raw, accel_scale_val);
}

double Gyro::getAccY() {
    int16_t raw = read16BitRegister(ACCEL_YOUT_H_ADDR);
    return scaleAccel(raw, accel_scale_val);
}

double Gyro::getAccZ() {
    int16_t raw = read16BitRegister(ACCEL_ZOUT_H_ADDR);
    return scaleAccel(raw, accel_scale_val);
}

double Gyro::getGyroX() {
    int16_t raw = read16BitRegister(GYRO_XOUT_H_ADDR);
    return scaleGyro(raw, gyro_scale_val);
}

double Gyro::getGyroY() {
    int16_t raw = read16BitRegister(GYRO_YOUT_H_ADDR);
    return scaleGyro(raw, gyro_scale_val);
}

double Gyro::getGyroZ() {
    int16_t raw = read16BitRegister(GYRO_ZOUT_H_ADDR);
    return scaleGyro(raw, gyro_scale_val);
}

double Gyro::getTemp() {
    int16_t raw = read16BitRegister(TEMP_OUT_H_ADDR);
    return scaleTemp(raw);
}


int16_t Gyro::read16BitRegister(int reg_high) {
    int high = wiringPiI2CReadReg8(device_handle, reg_high);
    int low  = wiringPiI2CReadReg8(device_handle, reg_high + 1);
    int16_t value = (high << 8) | low;
    if (value >= 0x8000) value = -(65536 - value);
    return value;
}

int Gyro::open_device(const char* dev_path, int addr) {
    int fd = open(dev_path, O_RDWR);
    if (fd < 0) {
        std::cerr << "Failed to open device path." << std::endl;
        return -1;
    }
    if (ioctl(fd, I2C_SLAVE, addr) < 0) {
        std::cerr << "Failed to set I2C address." << std::endl;
        close(fd);
        return -1;
    }
    return fd;
}

void Gyro::close_device(int fd) {
    close(fd);
}

void Gyro::write_byte(int fd, int reg, int data) {
    wiringPiI2CWriteReg8(fd, reg, data);
}

double Gyro::scaleAccel(int raw, int scale_val) {
    double factor = 16384.0; // default for ±2g
    switch (scale_val) {
        case 2:  factor = 16384.0; break;
        case 4:  factor = 8192.0; break;
        case 8:  factor = 4096.0; break;
        case 16: factor = 2048.0; break;
    }
    return (raw / factor) * GRAVITATIONAL_CONST;
}

double Gyro::scaleGyro(int raw, int scale_val) {
    double factor = 131.0;
    switch (scale_val) {
        case 250:  factor = 131.0; break;
        case 500:  factor = 65.5; break;
        case 1000: factor = 32.8; break;
        case 2000: factor = 16.4; break;
    }
    return raw / factor;
}

double Gyro::scaleTemp(int raw) {
    return ((raw - ROOMTEMP_OFFSET) / TEMP_SENSITIVITY) + 16.0;
}

void Gyro::device_wait(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
