#pragma once

#include "gd32f4xx_rcu.h"
#include <stdbool.h>

class I2C {
public:
    I2C(rcu_periph_enum rcu_sda, uint32_t port_sda, uint32_t pin_sda, rcu_periph_enum rcu_scl, uint32_t port_scl, uint32_t pin_scl);

    void init();

    void start() const;
    void stop() const;
    void ack() const;
    void nack() const;
    bool waitack() const;

    bool write(uint8_t value) const;
    uint8_t read() const;

private:
    static void delay();

    inline void sdaInput() const;
    inline void sdaOutput() const;
    inline uint8_t sdaGet() const;
    inline void sda(int val) const;
    inline void scl(int val) const;

private:
    rcu_periph_enum rcu_sda_;
    uint32_t port_sda_;
    uint32_t pin_sda_;

    rcu_periph_enum rcu_scl_;
    uint32_t port_scl_;
    uint32_t pin_scl_;
};