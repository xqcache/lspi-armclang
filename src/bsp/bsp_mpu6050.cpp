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

    write(MPU6050_RA_INT_ENABLE, 0x00); // 关闭所有中断
    write(MPU6050_RA_USER_CTRL, 0x00); // I2C主模式关闭
    write(MPU6050_RA_FIFO_EN, 0x00); // 关闭FIFO
    write(MPU6050_RA_INT_PIN_CFG, 0x80); // INT引脚低电平有效

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

bool MPU6050::getAccelValue(int16_t& ax, int16_t& ay, int16_t& az) const
{
    uint8_t buffer[6] = { 0 };

    if (read(MPU6050_RA_ACCEL_XOUT_H, 6, buffer) != sizeof(buffer)) {
        return false;
    }

    ax = ((int16_t)buffer[0] << 8) | buffer[1];
    ay = ((int16_t)buffer[2] << 8) | buffer[3];
    az = ((int16_t)buffer[4] << 8) | buffer[5];

    return true;
}
