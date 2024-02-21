#pragma once

#include "gd32f4xx_rcu.h"

class Led {
public:
    Led(rcu_periph_enum rcu_port, uint32_t port, uint32_t pin);

    void init();

    void on() const;
    void off() const;
    void toggle() const;
    bool isOn() const;

private:
    rcu_periph_enum rcu_port_;
    uint32_t port_;
    uint32_t pin_;
};