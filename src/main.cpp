#include "bsp_i2c.h"
#include "bsp_led.h"
#include "bsp_mpu6050.h"
#include "bsp_serial.h"
#include "gd32f4xx_gpio.h"
#include "gd32f4xx_rcu.h"
#include "systick.h"

int main(void)
{
    Led led2(RCU_GPIOD, GPIOD, GPIO_PIN_7);
    Serial serial0(RCU_USART0, USART0, RCU_GPIOA, GPIOA, GPIO_PIN_9, GPIO_PIN_10);
    I2C i2c(RCU_GPIOB, GPIOB, GPIO_PIN_6, RCU_GPIOB, GPIOB, GPIO_PIN_7);
    MPU6050 mpu(i2c);

    systick_config();

    serial0.init();
    i2c.init();
    led2.init();
    mpu.init(0x68);

    int count = 0;
    while (1) {
        delay_1ms(1000);
        led2.toggle();
        serial0.println("temp=%f,", mpu.getTemperature());
    }
}