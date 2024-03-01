#include "bsp_mpu6050.h"
#include "gd32f4xx_gpio.h"
#include "systick.h"

MPU6050::MPU6050(I2C& i2c)
    : i2c_(i2c)
{
}

bool MPU6050::init(uint32_t slave_addr)
{
    addr_ = slave_addr;
    delay_1ms(10);
    write(MPU6050_RA_PWR_MGMT_1, 0x80);
    delay_1ms(100);
    write(MPU6050_RA_PWR_MGMT_1, 0x00);
    write(MPU6050_RA_PWR_MGMT_2, 0x00);

    write(MPU6050_RA_CONFIG, 0x03);

    setAccelRange(MPU6050_ACCEL_FS_2);
    setGyroRange(MPU6050_GYRO_FS_250);

    return getId() != 0;
}

bool MPU6050::write(uint32_t reg_addr, uint8_t value) const
{
    i2c_.start();
    if (!i2c_.write(addr_ << 1)) {
        i2c_.stop();
        return false;
    }
    if (!i2c_.write(reg_addr)) {
        i2c_.stop();
        return false;
    }
    if (!i2c_.write(value)) {
        i2c_.stop();
        return false;
    }
    i2c_.stop();
    return false;
}

uint32_t MPU6050::write(uint32_t reg_addr, uint32_t num, const uint8_t* buffer) const
{
    i2c_.start();
    if (!i2c_.write(addr_ << 1)) {
        i2c_.stop();
        return 0;
    }
    if (!i2c_.write(reg_addr)) {
        i2c_.stop();
        return 0;
    }
    for (uint32_t i = 0; i < num; ++i) {
        if (!i2c_.write(buffer[i])) {
            i2c_.stop();
            return i;
        }
    }
    i2c_.stop();

    return num;
}

uint8_t MPU6050::read(uint32_t reg_addr) const
{
    uint8_t value = 0;
    i2c_.start();
    if (!i2c_.write(addr_ << 1)) {
        i2c_.stop();
        return value;
    }
    if (!i2c_.write(reg_addr)) {
        i2c_.stop();
        return value;
    }
    i2c_.start();
    if (!i2c_.write((addr_ << 1) | 1)) {
        i2c_.stop();
        return value;
    }
    value = i2c_.read();
    i2c_.nack();
    i2c_.stop();

    return value;
}

uint32_t MPU6050::read(uint32_t reg_addr, uint32_t num, uint8_t* buffer) const
{
    i2c_.start();
    if (!i2c_.write(addr_ << 1)) {
        i2c_.stop();
        return 0;
    }
    if (!i2c_.write(reg_addr)) {
        i2c_.stop();
        return 0;
    }
    i2c_.start();
    if (!i2c_.write((addr_ << 1) | 1)) {
        i2c_.stop();
        return 0;
    }
    for (uint32_t i = 0; i < num; ++i) {
        buffer[i] = i2c_.read();
        if (i < num - 1) {
            i2c_.ack();
        }
    }
    i2c_.nack();
    i2c_.stop();

    return num;
}

float MPU6050::getAccelRange() const
{
    return decodeAccelRange(read(MPU6050_RA_ACCEL_CONFIG));
}

bool MPU6050::setAccelRange(uint8_t range_def)
{
    if (range_def > 0x03) {
        return false;
    }
    accel_range_ = decodeAccelRange(range_def);
    return write(MPU6050_RA_ACCEL_CONFIG, range_def);
}

float MPU6050::getGyroRange() const
{
    return decodeGyroRange(read(MPU6050_RA_GYRO_CONFIG));
}

bool MPU6050::setGyroRange(uint8_t range_def)
{
    if (range_def > 0x03) {
        return false;
    }
    gyro_range_ = decodeGyroRange(range_def);
    return write(MPU6050_RA_GYRO_CONFIG, range_def);
}

float MPU6050::getTemperature() const
{
    uint8_t buffer[2] = { 0 };
    if (read(MPU6050_RA_TEMP_OUT_H, 2, buffer) != 2) {
        return -40.0f;
    }
    int16_t raw_value = ((int16_t)buffer[0] << 8) | buffer[1];
    return (float)raw_value / 340.0f + 36.53f;
}

uint8_t MPU6050::getId() const
{
    return read(MPU6050_RA_WHO_AM_I);
}

bool MPU6050::getAccelValue(float& ax, float& ay, float& az) const
{
    uint8_t buffer[6] = { 0 };

    if (read(MPU6050_RA_ACCEL_XOUT_H, 6, buffer) != sizeof(buffer)) {
        return false;
    }

    ax = (float)(((int16_t)buffer[0] << 8) | buffer[1]) / 32768.0f * accel_range_;
    ay = (float)(((int16_t)buffer[2] << 8) | buffer[3]) / 32768.0f * accel_range_;
    az = (float)(((int16_t)buffer[4] << 8) | buffer[5]) / 32768.0f * accel_range_;

    return true;
}

bool MPU6050::getGyroValue(float& gx, float& gy, float& gz) const
{
    uint8_t buffer[6] = { 0 };

    if (read(MPU6050_RA_GYRO_XOUT_H, 6, buffer) != sizeof(buffer)) {
        return false;
    }

    gx = (float)(((int16_t)buffer[0] << 8) | buffer[1]) / 32768.0f * gyro_range_;
    gy = (float)(((int16_t)buffer[2] << 8) | buffer[3]) / 32768.0f * gyro_range_;
    gz = (float)(((int16_t)buffer[4] << 8) | buffer[5]) / 32768.0f * gyro_range_;

    return true;
}

bool MPU6050::getAccelRawValue(uint16_t& ax, uint16_t& ay, uint16_t& az) const
{
    uint8_t buffer[6] = { 0 };

    if (read(MPU6050_RA_GYRO_XOUT_H, 6, buffer) != sizeof(buffer)) {
        return false;
    }

    ax = (uint16_t)buffer[0] << 8 | buffer[1];
    ay = (uint16_t)buffer[2] << 8 | buffer[3];
    az = (uint16_t)buffer[4] << 8 | buffer[5];

    return true;
}

float MPU6050::decodeAccelRange(uint8_t accel_cfg)
{
    switch (accel_cfg) {
    case MPU6050_ACCEL_FS_2:
        return 2.0f;
    case MPU6050_ACCEL_FS_4:
        return 4.0f;
    case MPU6050_ACCEL_FS_8:
        return 8.0f;
    case MPU6050_ACCEL_FS_16:
        return 16.0f;
    default:
        return 0.0f;
    }
}

float MPU6050::decodeGyroRange(uint8_t gyro_cfg)
{
    switch (gyro_cfg) {
    case MPU6050_GYRO_FS_250:
        return 250.0f;
    case MPU6050_GYRO_FS_500:
        return 500.0f;
    case MPU6050_GYRO_FS_1000:
        return 1000.0f;
    case MPU6050_GYRO_FS_2000:
        return 2000.0f;
    default:
        return 0.0f;
    }
    return 0.0f;
}
