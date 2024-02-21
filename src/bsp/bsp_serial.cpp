#include "bsp_serial.h"
#include <algorithm>
#include <cstdarg>

Serial::Serial(rcu_periph_enum rcu_usart, uint32_t usart, rcu_periph_enum rcu_port, uint32_t port, uint32_t pin_tx, uint32_t pin_rx)
    : rcu_usart_(rcu_usart)
    , usart_(usart)
    , rcu_port_(rcu_port)
    , port_(port)
    , pin_tx_(pin_tx)
    , pin_rx_(pin_rx)
{
}

void Serial::init(uint32_t baudrate, uint32_t parity, uint32_t word_len, uint32_t stopbit)
{
    periphInit();

    usart_baudrate_set(usart_, baudrate);
    usart_parity_config(usart_, parity);
    usart_word_length_set(usart_, word_len);
    usart_stop_bit_set(usart_, stopbit);

    usart_enable(usart_);
}

void Serial::print(char ch) const
{
    usart_data_transmit(usart_, static_cast<uint16_t>(ch));
    while (!usart_flag_get(usart_, USART_FLAG_TBE)) { }
}

void Serial::print(float v) const
{
    print("%f", v);
}

void Serial::print(double v) const
{
    print("%lf", v);
}

void Serial::print(int v) const
{
    print("%d", v);
}

void Serial::print(unsigned int v) const
{
    print("%u", v);
}

void Serial::print(long long v) const
{
    print("%lld", v);
}

void Serial::print(unsigned long long v) const
{
    print("%llu", v);
}

void Serial::print(const char* fmt, ...) const
{
    char buffer[1024] = { 0 };
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, ap);
    va_end(ap);

    for (int i = 0; i < sizeof(buffer); ++i) {
        if (buffer[i] == 0) {
            break;
        }
        print(buffer[i]);
    }
}

void Serial::println(char ch) const
{
    print("%c\r\n", ch);
}

void Serial::println(float v) const
{
    print("%f\r\n", v);
}

void Serial::println(double v) const
{
    print("%lf\r\n", v);
}

void Serial::println(int v) const
{
    print("%d\r\n", v);
}

void Serial::println(unsigned int v) const
{
    print("%d\r\n", v);
}

void Serial::println(long long v) const
{
    print("%lld\r\n", v);
}

void Serial::println(unsigned long long v) const
{
    print("%llu\r\n", v);
}

void Serial::println(const char* fmt, ...) const
{
    char buffer[1024] = { 0 };
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, ap);
    va_end(ap);

    for (int i = 0; i < sizeof(buffer); ++i) {
        if (buffer[i] == 0) {
            break;
        }
        print(buffer[i]);
    }
    print('\r');
    print('\n');
}

void Serial::periphInit()
{
    rcu_periph_clock_enable(rcu_port_);
    gpio_af_set(port_, GPIO_AF_7, pin_tx_);
    gpio_af_set(port_, GPIO_AF_7, pin_rx_);

    gpio_mode_set(port_, GPIO_MODE_AF, GPIO_PUPD_PULLUP, pin_tx_);
    gpio_output_options_set(port_, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, pin_tx_);

    gpio_mode_set(port_, GPIO_MODE_AF, GPIO_PUPD_PULLUP, pin_rx_);
    gpio_output_options_set(port_, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, pin_rx_);

    rcu_periph_clock_enable(rcu_usart_);
    usart_deinit(usart_);
    usart_receive_config(usart_, USART_RECEIVE_ENABLE);
    usart_transmit_config(usart_, USART_TRANSMIT_ENABLE);
    usart_disable(usart_);
}