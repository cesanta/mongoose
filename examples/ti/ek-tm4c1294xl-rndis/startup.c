// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

// Startup code
__attribute__((naked, noreturn)) void Reset_Handler(void) {
  // Initialise memory
  extern long _sbss, _ebss, _sdata, _edata, _sidata;
  for (long *src = &_sbss; src < &_ebss; src++) *src = 0;
  for (long *src = &_sdata, *dst = &_sidata; src < &_edata;) *src++ = *dst++;

  // Call the clock system initialization function
  extern void SystemInit(void);
  SystemInit();
  // Call static constructors
  extern void __libc_init_array(void);
  __libc_init_array();
  // Call the application entry point
  extern void main(void);
  main();
  for (;;) (void) 0;  // Infinite loop
}

void __attribute__((weak)) DefaultIRQHandler(void) {
  for (;;) (void) 0;
}
#define WEAK_ALIAS __attribute__((weak, alias("DefaultIRQHandler")))

WEAK_ALIAS void NMI_Handler(void);
WEAK_ALIAS void HardFault_Handler(void);
WEAK_ALIAS void MemManage_Handler(void);
WEAK_ALIAS void BusFault_Handler(void);
WEAK_ALIAS void UsageFault_Handler(void);
WEAK_ALIAS void SVC_Handler(void);
WEAK_ALIAS void DebugMon_Handler(void);
WEAK_ALIAS void PendSV_Handler(void);
WEAK_ALIAS void SysTick_Handler(void);

WEAK_ALIAS void GPIOA_Handler(void);
WEAK_ALIAS void GPIOB_Handler(void);
WEAK_ALIAS void GPIOC_Handler(void);
WEAK_ALIAS void GPIOD_Handler(void);
WEAK_ALIAS void GPIOE_Handler(void);
WEAK_ALIAS void UART0_Handler(void);
WEAK_ALIAS void UART1_Handler(void);
WEAK_ALIAS void SSI0_Handler(void);
WEAK_ALIAS void I2C0_Handler(void);
WEAK_ALIAS void PMW0_FAULT_Handler(void);
WEAK_ALIAS void PWM0_0_Handler(void);
WEAK_ALIAS void PWM0_1_Handler(void);
WEAK_ALIAS void PWM0_2_Handler(void);
WEAK_ALIAS void QEI0_Handler(void);
WEAK_ALIAS void ADC0SS0_Handler(void);
WEAK_ALIAS void ADC0SS1_Handler(void);
WEAK_ALIAS void ADC0SS2_Handler(void);
WEAK_ALIAS void ADC0SS3_Handler(void);
WEAK_ALIAS void WDT0_Handler(void);
WEAK_ALIAS void TIMER0A_Handler(void);
WEAK_ALIAS void TIMER0B_Handler(void);
WEAK_ALIAS void TIMER1A_Handler(void);
WEAK_ALIAS void TIMER1B_Handler(void);
WEAK_ALIAS void TIMER2A_Handler(void);
WEAK_ALIAS void TIMER2B_Handler(void);
WEAK_ALIAS void COMP0_Handler(void);
WEAK_ALIAS void COMP1_Handler(void);
WEAK_ALIAS void COMP2_Handler(void);
WEAK_ALIAS void SYSCTL_Handler(void);
WEAK_ALIAS void FLASH_Handler(void);
WEAK_ALIAS void GPIOF_Handler(void);
WEAK_ALIAS void GPIOG_Handler(void);
WEAK_ALIAS void GPIOH_Handler(void);
WEAK_ALIAS void UART2_Handler(void);
WEAK_ALIAS void SSI1_Handler(void);
WEAK_ALIAS void TIMER3A_Handler(void);
WEAK_ALIAS void TIMER3B_Handler(void);
WEAK_ALIAS void I2C1_Handler(void);
WEAK_ALIAS void CAN0_Handler(void);
WEAK_ALIAS void CAN1_Handler(void);
WEAK_ALIAS void EMAC0_IRQHandler(void);
WEAK_ALIAS void HIB_Handler(void);
WEAK_ALIAS void USB0_Handler(void);
WEAK_ALIAS void PWM0_3_Handler(void);
WEAK_ALIAS void UDMA_Handler(void);
WEAK_ALIAS void UDMAERR_Handler(void);
WEAK_ALIAS void ADC1SS0_Handler(void);
WEAK_ALIAS void ADC1SS1_Handler(void);
WEAK_ALIAS void ADC1SS2_Handler(void);
WEAK_ALIAS void ADC1SS3_Handler(void);
WEAK_ALIAS void EPI0_Handler(void);
WEAK_ALIAS void GPIOJ_Handler(void);
WEAK_ALIAS void GPIOK_Handler(void);
WEAK_ALIAS void GPIOL_Handler(void);
WEAK_ALIAS void SSI2_Handler(void);
WEAK_ALIAS void SSI3_Handler(void);
WEAK_ALIAS void UART3_Handler(void);
WEAK_ALIAS void UART4_Handler(void);
WEAK_ALIAS void UART5_Handler(void);
WEAK_ALIAS void UART6_Handler(void);
WEAK_ALIAS void UART7_Handler(void);
WEAK_ALIAS void I2C2_Handler(void);
WEAK_ALIAS void I2C3_Handler(void);
WEAK_ALIAS void TIMER4A_Handler(void);
WEAK_ALIAS void TIMER4B_Handler(void);
WEAK_ALIAS void TIMER5A_Handler(void);
WEAK_ALIAS void TIMER5B_Handler(void);
WEAK_ALIAS void FPU_Handler(void);
WEAK_ALIAS void I2C4_Handler(void);
WEAK_ALIAS void I2C5_Handler(void);
WEAK_ALIAS void GPIOM_Handler(void);
WEAK_ALIAS void GPION_Handler(void);
WEAK_ALIAS void TAMPER_Handler(void);
WEAK_ALIAS void GPIOP0_Handler(void);
WEAK_ALIAS void GPIOP1_Handler(void);
WEAK_ALIAS void GPIOP2_Handler(void);
WEAK_ALIAS void GPIOP3_Handler(void);
WEAK_ALIAS void GPIOP4_Handler(void);
WEAK_ALIAS void GPIOP5_Handler(void);
WEAK_ALIAS void GPIOP6_Handler(void);
WEAK_ALIAS void GPIOP7_Handler(void);
WEAK_ALIAS void GPIOQ0_Handler(void);
WEAK_ALIAS void GPIOQ1_Handler(void);
WEAK_ALIAS void GPIOQ2_Handler(void);
WEAK_ALIAS void GPIOQ3_Handler(void);
WEAK_ALIAS void GPIOQ4_Handler(void);
WEAK_ALIAS void GPIOQ5_Handler(void);
WEAK_ALIAS void GPIOQ6_Handler(void);
WEAK_ALIAS void GPIOQ7_Handler(void);
WEAK_ALIAS void TIMER6A_Handler(void);
WEAK_ALIAS void TIMER6B_Handler(void);
WEAK_ALIAS void TIMER7A_Handler(void);
WEAK_ALIAS void TIMER7B_Handler(void);
WEAK_ALIAS void I2C6_Handler(void);
WEAK_ALIAS void I2C7_Handler(void);
WEAK_ALIAS void I2C8_Handler(void);
WEAK_ALIAS void I2C9_Handler(void);

extern void _estack();

// IRQ table
__attribute__((section(".vectors"))) void (*tab[16 + 114])(void) = {
    // Cortex interrupts
    _estack, Reset_Handler, NMI_Handler, HardFault_Handler, MemManage_Handler,
    BusFault_Handler, UsageFault_Handler, 0, 0, 0, 0, SVC_Handler,
    DebugMon_Handler, 0, PendSV_Handler, SysTick_Handler,

    // Interrupts from peripherals
    GPIOA_Handler, GPIOB_Handler, GPIOC_Handler, GPIOD_Handler, GPIOE_Handler,
    UART0_Handler, UART1_Handler, SSI0_Handler, I2C0_Handler,
    PMW0_FAULT_Handler, PWM0_0_Handler, PWM0_1_Handler, PWM0_2_Handler,
    QEI0_Handler, ADC0SS0_Handler, ADC0SS1_Handler, ADC0SS2_Handler,
    ADC0SS3_Handler, WDT0_Handler, TIMER0A_Handler, TIMER0B_Handler,
    TIMER1A_Handler, TIMER1B_Handler, TIMER2A_Handler, TIMER2B_Handler,
    COMP0_Handler, COMP1_Handler, COMP2_Handler, SYSCTL_Handler, FLASH_Handler,
    GPIOF_Handler, GPIOG_Handler, GPIOH_Handler, UART2_Handler, SSI1_Handler,
    TIMER3A_Handler, TIMER3B_Handler, I2C1_Handler, CAN0_Handler, CAN1_Handler,
    EMAC0_IRQHandler, HIB_Handler, USB0_Handler, PWM0_3_Handler, UDMA_Handler,
    UDMAERR_Handler, ADC1SS0_Handler, ADC1SS1_Handler, ADC1SS2_Handler,
    ADC1SS3_Handler, EPI0_Handler, GPIOJ_Handler, GPIOK_Handler, GPIOL_Handler,
    SSI2_Handler, SSI3_Handler, UART3_Handler, UART4_Handler, UART5_Handler,
    UART6_Handler, UART7_Handler, I2C2_Handler, I2C3_Handler, TIMER4A_Handler,
    TIMER4B_Handler, TIMER5A_Handler, TIMER5B_Handler, FPU_Handler, 0, 0,
    I2C4_Handler, I2C5_Handler, GPIOM_Handler, GPION_Handler, 0, TAMPER_Handler,
    GPIOP0_Handler, GPIOP1_Handler, GPIOP2_Handler, GPIOP3_Handler,
    GPIOP4_Handler, GPIOP5_Handler, GPIOP6_Handler, GPIOP7_Handler,
    GPIOQ0_Handler, GPIOQ1_Handler, GPIOQ2_Handler, GPIOQ3_Handler,
    GPIOQ4_Handler, GPIOQ5_Handler, GPIOQ6_Handler, GPIOQ7_Handler, 0, 0, 0, 0,
    0, 0, TIMER6A_Handler, TIMER6B_Handler, TIMER7A_Handler, TIMER7B_Handler,
    I2C6_Handler, I2C7_Handler, 0, 0, 0, 0, 0, I2C8_Handler, I2C9_Handler, 0, 0,
    0};
