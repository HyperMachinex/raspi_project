#ifndef GYROLIB_H
#define GYROLIB_H

#include <string>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <chrono>
#include <thread>

class Gyro {
public:
    Gyro();
    ~Gyro();

    bool init(const std::string& i2c_device = "/dev/i2c-1", int address = 0x68);
    void release();

    // gyro
    double getGyroX();
    double getGyroY();
    double getGyroZ();

    // ivme degerleri
    double getAccX();
    double getAccY();
    double getAccZ();

    // sicaklik
    double getTemp();

private:
    int device_handle;
    int i2c_address;
    std::string device_path;

    // I2C
    int16_t read16BitRegister(int reg_high);
    int open_device(const char* dev_path, int addr);
    void close_device(int fd);
    void write_byte(int fd, int reg, int data);

    // Scaling
    double scaleAccel(int raw, int scale_val);
    double scaleGyro(int raw, int scale_val);
    double scaleTemp(int raw);

    void device_wait(int ms);

    // I2C
    static constexpr int ACCEL_XOUT_H_ADDR = 0x3B;
    static constexpr int ACCEL_YOUT_H_ADDR = 0x3D;
    static constexpr int ACCEL_ZOUT_H_ADDR = 0x3F;
    static constexpr int ACCEL_CONFIG_ADDR = 0x1C;

    static constexpr int GYRO_XOUT_H_ADDR = 0x43;
    static constexpr int GYRO_YOUT_H_ADDR = 0x45;
    static constexpr int GYRO_ZOUT_H_ADDR = 0x47;
    static constexpr int GYRO_CONFIG_ADDR = 0x1B;

    static constexpr int TEMP_OUT_H_ADDR = 0x41;
    static constexpr double GRAVITATIONAL_CONST = 9.7803;
    static constexpr double TEMP_SENSITIVITY = 333.87;
    static constexpr double ROOMTEMP_OFFSET = 4.0;

    int accel_scale_val = 2;  
    int gyro_scale_val = 250; 
};

#endif // GYROLIB_H
