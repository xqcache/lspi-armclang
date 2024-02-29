// 参考startup_gd32f450_470.s实现，其他cortex-mx系列的启动文件应该都类似，稍微修改下__isr_vectors就可以移植到stm32或者gd32等各系列芯片上

#include "gd32f4xx.h"
#include <stdint.h>
#include <string.h>

extern void SystemInit(void);
void Reset_Handler(void);
void Null_Handler(void);
void Loop_Handler(void);

void __attribute__((weak, alias("Loop_Handler"))) NMI_Handler(void); //  NMI Handler
void __attribute__((weak, alias("Loop_Handler"))) HardFault_Handler(void); //  Hard Fault Handler
void __attribute__((weak, alias("Loop_Handler"))) MemManage_Handler(void); //  MPU Fault Handler
void __attribute__((weak, alias("Loop_Handler"))) BusFault_Handler(void); //  Bus Fault Handler
void __attribute__((weak, alias("Loop_Handler"))) UsageFault_Handler(void); //  Usage Fault Handler
void __attribute__((weak, alias("Loop_Handler"))) SVC_Handler(void); //  SVCall Handler
void __attribute__((weak, alias("Loop_Handler"))) DebugMon_Handler(void); //  Debug Monitor Handler
void __attribute__((weak, alias("Loop_Handler"))) PendSV_Handler(void); //  PendSV Handler
void __attribute__((weak, alias("Loop_Handler"))) SysTick_Handler(void); //  SysTick Handler

/* external interrupts handler */
void __attribute__((weak, alias("Null_Handler"))) WWDGT_IRQHandler(void); //  16:Window Watchdog Timer
void __attribute__((weak, alias("Null_Handler"))) LVD_IRQHandler(void); //  17:LVD through EXTI Line detect
void __attribute__((weak, alias("Null_Handler"))) TAMPER_STAMP_IRQHandler(void); //  18:Tamper and TimeStamp through EXTI Line detect
void __attribute__((weak, alias("Null_Handler"))) RTC_WKUP_IRQHandler(void); //  19:RTC Wakeup through EXTI Line
void __attribute__((weak, alias("Null_Handler"))) FMC_IRQHandler(void); //  20:FMC
void __attribute__((weak, alias("Null_Handler"))) RCU_CTC_IRQHandler(void); //  21:RCU and CTC
void __attribute__((weak, alias("Null_Handler"))) EXTI0_IRQHandler(void); //  22:EXTI Line 0
void __attribute__((weak, alias("Null_Handler"))) EXTI1_IRQHandler(void); //  23:EXTI Line 1
void __attribute__((weak, alias("Null_Handler"))) EXTI2_IRQHandler(void); //  24:EXTI Line 2
void __attribute__((weak, alias("Null_Handler"))) EXTI3_IRQHandler(void); //  25:EXTI Line 3
void __attribute__((weak, alias("Null_Handler"))) EXTI4_IRQHandler(void); //  26:EXTI Line 4
void __attribute__((weak, alias("Null_Handler"))) DMA0_Channel0_IRQHandler(void); //  27:DMA0 Channel0
void __attribute__((weak, alias("Null_Handler"))) DMA0_Channel1_IRQHandler(void); //  28:DMA0 Channel1
void __attribute__((weak, alias("Null_Handler"))) DMA0_Channel2_IRQHandler(void); //  29:DMA0 Channel2
void __attribute__((weak, alias("Null_Handler"))) DMA0_Channel3_IRQHandler(void); //  30:DMA0 Channel3
void __attribute__((weak, alias("Null_Handler"))) DMA0_Channel4_IRQHandler(void); //  31:DMA0 Channel4
void __attribute__((weak, alias("Null_Handler"))) DMA0_Channel5_IRQHandler(void); //  32:DMA0 Channel5
void __attribute__((weak, alias("Null_Handler"))) DMA0_Channel6_IRQHandler(void); //  33:DMA0 Channel6
void __attribute__((weak, alias("Null_Handler"))) ADC_IRQHandler(void); //  34:ADC
void __attribute__((weak, alias("Null_Handler"))) CAN0_TX_IRQHandler(void); //  35:CAN0 TX
void __attribute__((weak, alias("Null_Handler"))) CAN0_RX0_IRQHandler(void); //  36:CAN0 RX0
void __attribute__((weak, alias("Null_Handler"))) CAN0_RX1_IRQHandler(void); //  37:CAN0 RX1
void __attribute__((weak, alias("Null_Handler"))) CAN0_EWMC_IRQHandler(void); //  38:CAN0 EWMC
void __attribute__((weak, alias("Null_Handler"))) EXTI5_9_IRQHandler(void); //  39:EXTI5 to EXTI9
void __attribute__((weak, alias("Null_Handler"))) TIMER0_BRK_TIMER8_IRQHandler(void); //  40:TIMER0 Break and TIMER8
void __attribute__((weak, alias("Null_Handler"))) TIMER0_UP_TIMER9_IRQHandler(void); //  41:TIMER0 Update and TIMER9
void __attribute__((weak, alias("Null_Handler"))) TIMER0_TRG_CMT_TIMER10_IRQHandler(void); //  42:TIMER0 Trigger and Commutation and TIMER10
void __attribute__((weak, alias("Null_Handler"))) TIMER0_Channel_IRQHandler(void); //  43:TIMER0 Capture Compare
void __attribute__((weak, alias("Null_Handler"))) TIMER1_IRQHandler(void); //  44:TIMER1
void __attribute__((weak, alias("Null_Handler"))) TIMER2_IRQHandler(void); //  45:TIMER2
void __attribute__((weak, alias("Null_Handler"))) TIMER3_IRQHandler(void); //  46:TIMER3
void __attribute__((weak, alias("Null_Handler"))) I2C0_EV_IRQHandler(void); //  47:I2C0 Event
void __attribute__((weak, alias("Null_Handler"))) I2C0_ER_IRQHandler(void); //  48:I2C0 Error
void __attribute__((weak, alias("Null_Handler"))) I2C1_EV_IRQHandler(void); //  49:I2C1 Event
void __attribute__((weak, alias("Null_Handler"))) I2C1_ER_IRQHandler(void); //  50:I2C1 Error
void __attribute__((weak, alias("Null_Handler"))) SPI0_IRQHandler(void); //  51:SPI0
void __attribute__((weak, alias("Null_Handler"))) SPI1_IRQHandler(void); //  52:SPI1
void __attribute__((weak, alias("Null_Handler"))) USART0_IRQHandler(void); //  53:USART0
void __attribute__((weak, alias("Null_Handler"))) USART1_IRQHandler(void); //  54:USART1
void __attribute__((weak, alias("Null_Handler"))) USART2_IRQHandler(void); //  55:USART2
void __attribute__((weak, alias("Null_Handler"))) EXTI10_15_IRQHandler(void); //  56:EXTI10 to EXTI15
void __attribute__((weak, alias("Null_Handler"))) RTC_Alarm_IRQHandler(void); //  57:RTC Alarm
void __attribute__((weak, alias("Null_Handler"))) USBFS_WKUP_IRQHandler(void); //  58:USBFS Wakeup
void __attribute__((weak, alias("Null_Handler"))) TIMER7_BRK_TIMER11_IRQHandler(void); //  59:TIMER7 Break and TIMER11
void __attribute__((weak, alias("Null_Handler"))) TIMER7_UP_TIMER12_IRQHandler(void); //  60:TIMER7 Update and TIMER12
void __attribute__((weak, alias("Null_Handler"))) TIMER7_TRG_CMT_TIMER13_IRQHandler(void); //  61:TIMER7 Trigger and Commutation and TIMER13
void __attribute__((weak, alias("Null_Handler"))) TIMER7_Channel_IRQHandler(void); //  62:TIMER7 Channel Capture Compare
void __attribute__((weak, alias("Null_Handler"))) DMA0_Channel7_IRQHandler(void); //  63:DMA0 Channel7
void __attribute__((weak, alias("Null_Handler"))) EXMC_IRQHandler(void); //  64:EXMC
void __attribute__((weak, alias("Null_Handler"))) SDIO_IRQHandler(void); //  65:SDIO
void __attribute__((weak, alias("Null_Handler"))) TIMER4_IRQHandler(void); //  66:TIMER4
void __attribute__((weak, alias("Null_Handler"))) SPI2_IRQHandler(void); //  67:SPI2
void __attribute__((weak, alias("Null_Handler"))) UART3_IRQHandler(void); //  68:UART3
void __attribute__((weak, alias("Null_Handler"))) UART4_IRQHandler(void); //  69:UART4
void __attribute__((weak, alias("Null_Handler"))) TIMER5_DAC_IRQHandler(void); //  70:TIMER5 and DAC0 DAC1 Underrun error
void __attribute__((weak, alias("Null_Handler"))) TIMER6_IRQHandler(void); //  71:TIMER6
void __attribute__((weak, alias("Null_Handler"))) DMA1_Channel0_IRQHandler(void); //  72:DMA1 Channel0
void __attribute__((weak, alias("Null_Handler"))) DMA1_Channel1_IRQHandler(void); //  73:DMA1 Channel1
void __attribute__((weak, alias("Null_Handler"))) DMA1_Channel2_IRQHandler(void); //  74:DMA1 Channel2
void __attribute__((weak, alias("Null_Handler"))) DMA1_Channel3_IRQHandler(void); //  75:DMA1 Channel3
void __attribute__((weak, alias("Null_Handler"))) DMA1_Channel4_IRQHandler(void); //  76:DMA1 Channel4
void __attribute__((weak, alias("Null_Handler"))) ENET_IRQHandler(void); //  77:Ethernet
void __attribute__((weak, alias("Null_Handler"))) ENET_WKUP_IRQHandler(void); //  78:Ethernet Wakeup through EXTI Line
void __attribute__((weak, alias("Null_Handler"))) CAN1_TX_IRQHandler(void); //  79:CAN1 TX
void __attribute__((weak, alias("Null_Handler"))) CAN1_RX0_IRQHandler(void); //  80:CAN1 RX0
void __attribute__((weak, alias("Null_Handler"))) CAN1_RX1_IRQHandler(void); //  81:CAN1 RX1
void __attribute__((weak, alias("Null_Handler"))) CAN1_EWMC_IRQHandler(void); //  82:CAN1 EWMC
void __attribute__((weak, alias("Null_Handler"))) USBFS_IRQHandler(void); //  83:USBFS
void __attribute__((weak, alias("Null_Handler"))) DMA1_Channel5_IRQHandler(void); //  84:DMA1 Channel5
void __attribute__((weak, alias("Null_Handler"))) DMA1_Channel6_IRQHandler(void); //  85:DMA1 Channel6
void __attribute__((weak, alias("Null_Handler"))) DMA1_Channel7_IRQHandler(void); //  86:DMA1 Channel7
void __attribute__((weak, alias("Null_Handler"))) USART5_IRQHandler(void); //  87:USART5
void __attribute__((weak, alias("Null_Handler"))) I2C2_EV_IRQHandler(void); //  88:I2C2 Event
void __attribute__((weak, alias("Null_Handler"))) I2C2_ER_IRQHandler(void); //  89:I2C2 Error
void __attribute__((weak, alias("Null_Handler"))) USBHS_EP1_Out_IRQHandler(void); //  90:USBHS Endpoint 1 Out
void __attribute__((weak, alias("Null_Handler"))) USBHS_EP1_In_IRQHandler(void); //  91:USBHS Endpoint 1 in
void __attribute__((weak, alias("Null_Handler"))) USBHS_WKUP_IRQHandler(void); //  92:USBHS Wakeup through EXTI Line
void __attribute__((weak, alias("Null_Handler"))) USBHS_IRQHandler(void); //  93:USBHS
void __attribute__((weak, alias("Null_Handler"))) DCI_IRQHandler(void); //  94:DCI
void __attribute__((weak, alias("Null_Handler"))) TRNG_IRQHandler(void); //  96:TRNG
void __attribute__((weak, alias("Null_Handler"))) FPU_IRQHandler(void); //  97:FPU
void __attribute__((weak, alias("Null_Handler"))) UART6_IRQHandler(void); //  98:UART6
void __attribute__((weak, alias("Null_Handler"))) UART7_IRQHandler(void); //  99:UART7
void __attribute__((weak, alias("Null_Handler"))) SPI3_IRQHandler(void); //  100:SPI3
void __attribute__((weak, alias("Null_Handler"))) SPI4_IRQHandler(void); //  101:SPI4
void __attribute__((weak, alias("Null_Handler"))) SPI5_IRQHandler(void); //  102:SPI5
void __attribute__((weak, alias("Null_Handler"))) TLI_IRQHandler(void); //  104:TLI
void __attribute__((weak, alias("Null_Handler"))) TLI_ER_IRQHandler(void); //  105:TLI Error
void __attribute__((weak, alias("Null_Handler"))) IPA_IRQHandler(void); //  106:IPA

extern uint32_t __initial_sp;
extern uint32_t __heap_base;
extern uint32_t __heap_limit;

__attribute__((used)) unsigned __user_heap_extend(int var0, void** base, unsigned requested_size)
{
    if ((uint32_t)&__heap_limit - (uint32_t)&__heap_base > requested_size) {
        *base = (uint32_t*)(&__heap_base);
        return (uint32_t)&__heap_limit - (uint32_t)&__heap_base;
    }
    return 0;
}

__attribute__((noinline)) void Reset_Handler(void)
{
    extern void __main(void);
    __asm(".global __initial_sp\n\t"
          ".equ __initial_sp, 0x2006F800\n\t");

    __asm(".global __heap_base\n\t"
          ".equ __heap_base, 0x2006F800\n\t");

    __asm(".global __heap_limit\n\t"
          ".equ __heap_limit, 0x2007000\n\t");

    SystemInit();
    __main();
    while (1) { };
}

void Null_Handler(void)
{
}

void Loop_Handler(void)
{
    while (1) { }
}

const uint32_t __isr_vectors[] __attribute__((used, section(".isr_vectors"))) = {
    (uint32_t)(&__initial_sp),
    (uint32_t)Reset_Handler, //  Reset Handler
    (uint32_t)NMI_Handler, //  NMI Handler
    (uint32_t)HardFault_Handler, //  Hard Fault Handler
    (uint32_t)MemManage_Handler, //  MPU Fault Handler
    (uint32_t)BusFault_Handler, //  Bus Fault Handler
    (uint32_t)UsageFault_Handler, //  Usage Fault Handler
    (uint32_t)0, //  Reserved
    (uint32_t)0, //  Reserved
    (uint32_t)0, //  Reserved
    (uint32_t)0, //  Reserved
    (uint32_t)SVC_Handler, //  SVCall Handler
    (uint32_t)DebugMon_Handler, //  Debug Monitor Handler
    (uint32_t)0, //  Reserved
    (uint32_t)PendSV_Handler, //  PendSV Handler
    (uint32_t)SysTick_Handler, //  SysTick Handler

    /* external interrupts handler */
    (uint32_t)WWDGT_IRQHandler, //  16:Window Watchdog Timer
    (uint32_t)LVD_IRQHandler, //  17:LVD through EXTI Line detect
    (uint32_t)TAMPER_STAMP_IRQHandler, //  18:Tamper and TimeStamp through EXTI Line detect
    (uint32_t)RTC_WKUP_IRQHandler, //  19:RTC Wakeup through EXTI Line
    (uint32_t)FMC_IRQHandler, //  20:FMC
    (uint32_t)RCU_CTC_IRQHandler, //  21:RCU and CTC
    (uint32_t)EXTI0_IRQHandler, //  22:EXTI Line 0
    (uint32_t)EXTI1_IRQHandler, //  23:EXTI Line 1
    (uint32_t)EXTI2_IRQHandler, //  24:EXTI Line 2
    (uint32_t)EXTI3_IRQHandler, //  25:EXTI Line 3
    (uint32_t)EXTI4_IRQHandler, //  26:EXTI Line 4
    (uint32_t)DMA0_Channel0_IRQHandler, //  27:DMA0 Channel0
    (uint32_t)DMA0_Channel1_IRQHandler, //  28:DMA0 Channel1
    (uint32_t)DMA0_Channel2_IRQHandler, //  29:DMA0 Channel2
    (uint32_t)DMA0_Channel3_IRQHandler, //  30:DMA0 Channel3
    (uint32_t)DMA0_Channel4_IRQHandler, //  31:DMA0 Channel4
    (uint32_t)DMA0_Channel5_IRQHandler, //  32:DMA0 Channel5
    (uint32_t)DMA0_Channel6_IRQHandler, //  33:DMA0 Channel6
    (uint32_t)ADC_IRQHandler, //  34:ADC
    (uint32_t)CAN0_TX_IRQHandler, //  35:CAN0 TX
    (uint32_t)CAN0_RX0_IRQHandler, //  36:CAN0 RX0
    (uint32_t)CAN0_RX1_IRQHandler, //  37:CAN0 RX1
    (uint32_t)CAN0_EWMC_IRQHandler, //  38:CAN0 EWMC
    (uint32_t)EXTI5_9_IRQHandler, //  39:EXTI5 to EXTI9
    (uint32_t)TIMER0_BRK_TIMER8_IRQHandler, //  40:TIMER0 Break and TIMER8
    (uint32_t)TIMER0_UP_TIMER9_IRQHandler, //  41:TIMER0 Update and TIMER9
    (uint32_t)TIMER0_TRG_CMT_TIMER10_IRQHandler, //  42:TIMER0 Trigger and Commutation and TIMER10
    (uint32_t)TIMER0_Channel_IRQHandler, //  43:TIMER0 Capture Compare
    (uint32_t)TIMER1_IRQHandler, //  44:TIMER1
    (uint32_t)TIMER2_IRQHandler, //  45:TIMER2
    (uint32_t)TIMER3_IRQHandler, //  46:TIMER3
    (uint32_t)I2C0_EV_IRQHandler, //  47:I2C0 Event
    (uint32_t)I2C0_ER_IRQHandler, //  48:I2C0 Error
    (uint32_t)I2C1_EV_IRQHandler, //  49:I2C1 Event
    (uint32_t)I2C1_ER_IRQHandler, //  50:I2C1 Error
    (uint32_t)SPI0_IRQHandler, //  51:SPI0
    (uint32_t)SPI1_IRQHandler, //  52:SPI1
    (uint32_t)USART0_IRQHandler, //  53:USART0
    (uint32_t)USART1_IRQHandler, //  54:USART1
    (uint32_t)USART2_IRQHandler, //  55:USART2
    (uint32_t)EXTI10_15_IRQHandler, //  56:EXTI10 to EXTI15
    (uint32_t)RTC_Alarm_IRQHandler, //  57:RTC Alarm
    (uint32_t)USBFS_WKUP_IRQHandler, //  58:USBFS Wakeup
    (uint32_t)TIMER7_BRK_TIMER11_IRQHandler, //  59:TIMER7 Break and TIMER11
    (uint32_t)TIMER7_UP_TIMER12_IRQHandler, //  60:TIMER7 Update and TIMER12
    (uint32_t)TIMER7_TRG_CMT_TIMER13_IRQHandler, //  61:TIMER7 Trigger and Commutation and TIMER13
    (uint32_t)TIMER7_Channel_IRQHandler, //  62:TIMER7 Channel Capture Compare
    (uint32_t)DMA0_Channel7_IRQHandler, //  63:DMA0 Channel7
    (uint32_t)EXMC_IRQHandler, //  64:EXMC
    (uint32_t)SDIO_IRQHandler, //  65:SDIO
    (uint32_t)TIMER4_IRQHandler, //  66:TIMER4
    (uint32_t)SPI2_IRQHandler, //  67:SPI2
    (uint32_t)UART3_IRQHandler, //  68:UART3
    (uint32_t)UART4_IRQHandler, //  69:UART4
    (uint32_t)TIMER5_DAC_IRQHandler, //  70:TIMER5 and DAC0 DAC1 Underrun error
    (uint32_t)TIMER6_IRQHandler, //  71:TIMER6
    (uint32_t)DMA1_Channel0_IRQHandler, //  72:DMA1 Channel0
    (uint32_t)DMA1_Channel1_IRQHandler, //  73:DMA1 Channel1
    (uint32_t)DMA1_Channel2_IRQHandler, //  74:DMA1 Channel2
    (uint32_t)DMA1_Channel3_IRQHandler, //  75:DMA1 Channel3
    (uint32_t)DMA1_Channel4_IRQHandler, //  76:DMA1 Channel4
    (uint32_t)ENET_IRQHandler, //  77:Ethernet
    (uint32_t)ENET_WKUP_IRQHandler, //  78:Ethernet Wakeup through EXTI Line
    (uint32_t)CAN1_TX_IRQHandler, //  79:CAN1 TX
    (uint32_t)CAN1_RX0_IRQHandler, //  80:CAN1 RX0
    (uint32_t)CAN1_RX1_IRQHandler, //  81:CAN1 RX1
    (uint32_t)CAN1_EWMC_IRQHandler, //  82:CAN1 EWMC
    (uint32_t)USBFS_IRQHandler, //  83:USBFS
    (uint32_t)DMA1_Channel5_IRQHandler, //  84:DMA1 Channel5
    (uint32_t)DMA1_Channel6_IRQHandler, //  85:DMA1 Channel6
    (uint32_t)DMA1_Channel7_IRQHandler, //  86:DMA1 Channel7
    (uint32_t)USART5_IRQHandler, //  87:USART5
    (uint32_t)I2C2_EV_IRQHandler, //  88:I2C2 Event
    (uint32_t)I2C2_ER_IRQHandler, //  89:I2C2 Error
    (uint32_t)USBHS_EP1_Out_IRQHandler, //  90:USBHS Endpoint 1 Out
    (uint32_t)USBHS_EP1_In_IRQHandler, //  91:USBHS Endpoint 1 in
    (uint32_t)USBHS_WKUP_IRQHandler, //  92:USBHS Wakeup through EXTI Line
    (uint32_t)USBHS_IRQHandler, //  93:USBHS
    (uint32_t)DCI_IRQHandler, //  94:DCI
    (uint32_t)0, //  95:Reserved
    (uint32_t)TRNG_IRQHandler, //  96:TRNG
    (uint32_t)FPU_IRQHandler, //  97:FPU
    (uint32_t)UART6_IRQHandler, //  98:UART6
    (uint32_t)UART7_IRQHandler, //  99:UART7
    (uint32_t)SPI3_IRQHandler, //  100:SPI3
    (uint32_t)SPI4_IRQHandler, //  101:SPI4
    (uint32_t)SPI5_IRQHandler, //  102:SPI5
    (uint32_t)0, //  103:Reserved
    (uint32_t)TLI_IRQHandler, //  104:TLI
    (uint32_t)TLI_ER_IRQHandler, //  105:TLI Error
    (uint32_t)IPA_IRQHandler, //  106:IPA
};
