#pragma once

#include "gd32f4xx_rcu.h"
#include "gd32f4xx_usart.h"

#include <sstream>
#include <string>
#include <string_view>

class Serial {
public:
    Serial(rcu_periph_enum rcu_usart, uint32_t usart, rcu_periph_enum rcu_port, uint32_t port, uint32_t pin_tx, uint32_t pin_rx);

    void init(uint32_t baudrate = 115200U, uint32_t parity = USART_PM_NONE, uint32_t word_len = 8, uint32_t stopbit = 1);

    void print(char ch) const;
    void print(float v) const;
    void print(double v) const;
    void print(int v) const;
    void print(unsigned int v) const;
    void print(long long v) const;
    void print(unsigned long long v) const;
    void print(const char* fmt, ...) const;

    void println(char ch) const;
    void println(float v) const;
    void println(double v) const;
    void println(int v) const;
    void println(unsigned int v) const;
    void println(long long v) const;
    void println(unsigned long long v) const;
    void println(const char* fmt, ...) const;

private:
    void periphInit();

private:
    rcu_periph_enum rcu_port_;
    rcu_periph_enum rcu_usart_;
    uint32_t usart_;
    uint32_t port_;
    uint32_t pin_rx_;
    uint32_t pin_tx_;
};