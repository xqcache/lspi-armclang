#include "bsp_led.h"
#include "bsp_serial.h"
#include "gd32f4xx_gpio.h"
#include "gd32f4xx_rcu.h"
#include "systick.h"

int main(void)
{
    Led led2(RCU_GPIOD, GPIOD, GPIO_PIN_7);
    Serial serial0(RCU_USART0, USART0, RCU_GPIOA, GPIOA, GPIO_PIN_9, GPIO_PIN_10);

    systick_config();

    led2.init();
    serial0.init();

    int count = 0;
    while (1) {
        delay_1ms(100);
        led2.toggle();
        serial0.println("count = %d", count++);
    }
}