#include "bsp_led.h"
#include "gd32f4xx_gpio.h"

Led::Led(rcu_periph_enum rcu_port, uint32_t port, uint32_t pin)
    : rcu_port_(rcu_port)
    , port_(port)
    , pin_(pin)
{
}

void Led::init()
{
    // 开启GPIOD的时钟
    rcu_periph_clock_enable(rcu_port_);

    // 配置GPIO模式
    gpio_mode_set(port_, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, pin_);

    // 配置PD7端口翻转速度
    gpio_output_options_set(port_, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, pin_);
}

void Led::on() const
{
    gpio_bit_set(port_, pin_);
}

void Led::off() const
{
    gpio_bit_reset(port_, pin_);
}

void Led::toggle() const
{
    gpio_bit_toggle(port_, pin_);
}

bool Led::isOn() const
{
    return gpio_output_bit_get(port_, pin_) == SET;
}
