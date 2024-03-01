#include "bsp_i2c.h"
#include "gd32f4xx_gpio.h"

I2C::I2C(rcu_periph_enum rcu_sda, uint32_t port_sda, uint32_t pin_sda, rcu_periph_enum rcu_scl, uint32_t port_scl, uint32_t pin_scl)
    : rcu_sda_(rcu_sda)
    , port_sda_(port_sda)
    , pin_sda_(pin_sda)
    , rcu_scl_(rcu_scl)
    , port_scl_(port_scl)
    , pin_scl_(pin_scl)
{
}

void I2C::init()
{
    rcu_periph_clock_enable(rcu_sda_);
    gpio_mode_set(port_sda_, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, pin_sda_);
    gpio_output_options_set(port_sda_, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, pin_sda_);

    rcu_periph_clock_enable(rcu_scl_);
    gpio_mode_set(port_scl_, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, pin_scl_);
    gpio_output_options_set(port_scl_, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, pin_scl_);

    sda(0);
    scl(0);
}

void I2C::start() const
{
    sdaOutput();
    scl(0);
    sda(1);
    delay();
    scl(1);
    delay();
    sda(0);
    delay();
    scl(0);
    delay();
}

void I2C::stop() const
{
    sdaOutput();
    scl(0);
    sda(0);
    delay();
    scl(1);
    delay();
    sda(1);
    delay();
    scl(0);
    delay();
}

void I2C::nack() const
{
    sdaOutput();
    scl(0);
    sda(1);
    delay();
    scl(1);
    delay();
    scl(0);
    delay();
}

void I2C::ack() const
{
    sdaOutput();
    scl(0);
    sda(0);
    delay();
    scl(1);
    delay();
    scl(0);
    delay();
}

bool I2C::waitack() const
{
    int try_count = 0;
    uint8_t value = 1;
    sdaInput();
    scl(1);
    for (try_count = 0; try_count < 3 && value != 0; ++try_count) {
        delay();
        value = sdaGet();
    }
    scl(0);
    delay();
    return value == 0;
}

bool I2C::write(uint8_t value) const
{
    sdaOutput();
    for (int i = 0; i < 8; ++i) {
        sda((value & 0x80) != 0);
        delay();
        scl(1);
        delay();
        scl(0);
        delay();
        value <<= 1;
    }
    return waitack();
}

uint8_t I2C::read() const
{
    uint8_t value = 0;
    sdaInput();
    for (int i = 0; i < 8; ++i) {
        scl(1);
        delay();
        value <<= 1;
        if (sdaGet()) {
            value |= 1;
        }
        scl(0);
        delay();
    }
    return value;
}

void I2C::delay()
{
    for (int i = 0; i < 1000; ++i) {
        __NOP();
        __NOP();
        __NOP();
    }
}

inline void I2C::sdaInput() const
{
    gpio_mode_set(port_sda_, GPIO_MODE_INPUT, GPIO_PUPD_NONE, pin_sda_);
}

inline void I2C::sdaOutput() const
{
    gpio_mode_set(port_sda_, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, pin_sda_);
}

inline uint8_t I2C::sdaGet() const
{
    return gpio_input_bit_get(port_sda_, pin_sda_);
}

inline void I2C::sda(int val) const
{
    gpio_bit_write(port_sda_, pin_sda_, static_cast<bit_status>(val));
}

inline void I2C::scl(int val) const
{
    gpio_bit_write(port_scl_, pin_scl_, static_cast<bit_status>(val));
}
