#include "RW612.h"

void Reset_Handler(void);    // Defined below
void Dummy_Handler(void);    // Defined below
void SysTick_Handler(void);  // Defined in main.c
void SystemInit(void);       // Defined in main.c, called by reset handler
void __StackTop(void);          // Defined in link.ld
void _image_size(void);

#define WEAK __attribute__((weak, alias("Default_Handler")))

void Default_Handler(void);
WEAK void NMI_Handler(void);
WEAK void HardFault_Handler(void);
WEAK void MemManage_Handler(void);
WEAK void BusFault_Handler(void);
WEAK void UsageFault_Handler(void);
WEAK void SecureFault_Handler(void);
WEAK void SVC_Handler(void);
WEAK void DebugMon_Handler(void);
WEAK void PendSV_Handler(void);
WEAK void SysTick_Handler(void);
WEAK void IntDefaultHandler(void);
WEAK void WDT0_IRQHandler(void);
WEAK void DMA0_IRQHandler(void);
WEAK void GPIO_INTA_IRQHandler(void);
WEAK void GPIO_INTB_IRQHandler(void);
WEAK void PIN_INT0_IRQHandler(void);
WEAK void PIN_INT1_IRQHandler(void);
WEAK void PIN_INT2_IRQHandler(void);
WEAK void PIN_INT3_IRQHandler(void);
WEAK void UTICK_IRQHandler(void);
WEAK void MRT_IRQHandler(void);
WEAK void CTIMER0_IRQHandler(void);
WEAK void CTIMER1_IRQHandler(void);
WEAK void SCT0_IRQHandler(void);
WEAK void CTIMER3_IRQHandler(void);
WEAK void FLEXCOMM0_IRQHandler(void);
WEAK void FLEXCOMM1_IRQHandler(void);
WEAK void FLEXCOMM2_IRQHandler(void);
WEAK void FLEXCOMM3_IRQHandler(void);
WEAK void Reserved34_IRQHandler(void);
WEAK void Reserved35_IRQHandler(void);
WEAK void FLEXCOMM14_IRQHandler(void);
WEAK void Reserved37_IRQHandler(void);
WEAK void Reserved38_IRQHandler(void);
WEAK void GFMRT_IRQHandler(void);
WEAK void Reserved40_IRQHandler(void);
WEAK void DMIC_IRQHandler(void);
WEAK void WKDEEPSLEEP_IRQHandler(void);
WEAK void HYPERVISOR_IRQHandler(void);
WEAK void SECUREVIOLATION_IRQHandler(void);
WEAK void HWVAD_IRQHandler(void);
WEAK void Reserved46_IRQHandler(void);
WEAK void Reserved47_IRQHandler(void);
WEAK void RTC_IRQHandler(void);
WEAK void Reserved49_IRQHandler(void);
WEAK void Reserved50_IRQHandler(void);
WEAK void PIN_INT4_IRQHandler(void);
WEAK void PIN_INT5_IRQHandler(void);
WEAK void PIN_INT6_IRQHandler(void);
WEAK void PIN_INT7_IRQHandler(void);
WEAK void CTIMER2_IRQHandler(void);
WEAK void CTIMER4_IRQHandler(void);
WEAK void OS_EVENT_TIMER_IRQHandler(void);
WEAK void FLEXSPI_IRQHandler(void);
WEAK void Reserved59_IRQHandler(void);
WEAK void Reserved60_IRQHandler(void);
WEAK void Reserved61_IRQHandler(void);
WEAK void SDIO_IRQHandler(void);
WEAK void SGPIO_INTA_IRQHandler(void);
WEAK void SGPIO_INTB_IRQHandler(void);
WEAK void Reserved65_IRQHandler(void);
WEAK void USB_IRQHandler(void);
WEAK void Reserved67_IRQHandler(void);
WEAK void Reserved68_IRQHandler(void);
WEAK void Reserved69_IRQHandler(void);
WEAK void DMA1_IRQHandler(void);
WEAK void PUF_IRQHandler(void);
WEAK void POWERQUAD_IRQHandler(void);
WEAK void Reserved73_IRQHandler(void);
WEAK void Reserved74_IRQHandler(void);
WEAK void Reserved75_IRQHandler(void);
WEAK void Reserved76_IRQHandler(void);
WEAK void LCD_IRQHandler(void);
WEAK void CAPTIMER_IRQHandler(void);
WEAK void Reserved79_IRQHandler(void);
WEAK void W2MWKUP_DONE0_IRQHandler(void);
WEAK void W2MWKUP_DONE1_IRQHandler(void);
WEAK void W2MWKUP_DONE2_IRQHandler(void);
WEAK void W2MWKUP_DONE3_IRQHandler(void);
WEAK void W2MWKUP_DONE4_IRQHandler(void);
WEAK void W2MWKUP_DONE5_IRQHandler(void);
WEAK void W2MWKUP_DONE6_IRQHandler(void);
WEAK void W2MWKUP_DONE7_IRQHandler(void);
WEAK void W2MWKUP0_IRQHandler(void);
WEAK void W2MWKUP1_IRQHandler(void);
WEAK void WL_MCI_INT0_IRQHandler(void);
WEAK void WL_MCI_INT1_IRQHandler(void);
WEAK void WL_MCI_INT2_IRQHandler(void);
WEAK void WL_MCI_INT3_IRQHandler(void);
WEAK void WL_MCI_INT4_IRQHandler(void);
WEAK void WL_MCI_INT5_IRQHandler(void);
WEAK void WL_MCI_INT6_IRQHandler(void);
WEAK void WL_MCI_INT7_IRQHandler(void);
WEAK void B2MWKUP_DONE0_IRQHandler(void);
WEAK void B2MWKUP_DONE1_IRQHandler(void);
WEAK void B2MWKUP_DONE2_IRQHandler(void);
WEAK void B2MWKUP_DONE3_IRQHandler(void);
WEAK void B2MWKUP_DONE4_IRQHandler(void);
WEAK void B2MWKUP_DONE5_IRQHandler(void);
WEAK void B2MWKUP_DONE6_IRQHandler(void);
WEAK void B2MWKUP_DONE7_IRQHandler(void);
WEAK void B2MWKUP0_IRQHandler(void);
WEAK void B2MWKUP1_IRQHandler(void);
WEAK void BLE_MCI_INT0_IRQHandler(void);
WEAK void BLE_MCI_INT1_IRQHandler(void);
WEAK void BLE_MCI_INT2_IRQHandler(void);
WEAK void BLE_MCI_INT3_IRQHandler(void);
WEAK void BLE_MCI_INT4_IRQHandler(void);
WEAK void BLE_MCI_INT5_IRQHandler(void);
WEAK void BLE_MCI_INT6_IRQHandler(void);
WEAK void BLE_MCI_INT7_IRQHandler(void);
WEAK void PIN0_INT_IRQHandler(void);
WEAK void PIN1_INT_IRQHandler(void);
WEAK void ELS_IRQHandler(void);
WEAK void ELS_GDET_IRQHandler(void);
WEAK void ELS_GDET_UM_IRQHandler(void);
WEAK void PKC_INT_IRQHandler(void);
WEAK void PKC_ERR_IRQHandler(void);
WEAK void CDOG_INT_IRQHandler(void);
WEAK void GAU_DAC_IRQHandler(void);
WEAK void GAU_ACOMP_WKUP_IRQHandler(void);
WEAK void GAU_ACOMP_IRQHandler(void);
WEAK void GAU_ADC1_IRQHandler(void);
WEAK void GAU_ADC0_IRQHandler(void);
WEAK void USIM_IRQHandler(void);
WEAK void OTP_IRQHandler(void);
WEAK void ENET_IRQHandler(void);
WEAK void ENET_TIMER_IRQHandler(void);
WEAK void PMIP_IRQHandler(void);
WEAK void PMIP_CHANGE_IRQHandler(void);
WEAK void ITRC_IRQHandler(void);
WEAK void Reserved136_IRQHandler(void);
WEAK void Reserved137_IRQHandler(void);
WEAK void Reserved138_IRQHandler(void);
WEAK void TRNG_IRQHandler(void);
WEAK void ACC_C_INT_IRQHandler(void);
WEAK void ACC_S_INT_IRQHandler(void);
WEAK void WACC_IRQHandler(void);
WEAK void BACC_IRQHandler(void);
WEAK void GDMA_IRQHandler(void);


#define WEAK_ALIAS __attribute__((weak, alias("Default_Handler")))


__attribute__((section(".vectors"))) void (*const tab[16 + 156])(void) = {
    &__StackTop,                       // The initial stack pointer
    Reset_Handler,                          // The reset handler
    NMI_Handler,                       // NMI Handler
    HardFault_Handler,                 // Hard Fault Handler
    MemManage_Handler,                 // MPU Fault Handler
    BusFault_Handler,                  // Bus Fault Handler
    UsageFault_Handler,                // Usage Fault Handler
    SecureFault_Handler,               // Secure Fault Handler
    &_image_size,                      // Image size
    0,                                 // Image type
    0,                                 // Reserved
    SVC_Handler,                       // SVCall Handler
    DebugMon_Handler,                  // Debug Monitor Handler
    (void (*)())tab,                   // Image load address
    PendSV_Handler,                    // PendSV Handler
    SysTick_Handler,                   // SysTick Handler

    // Chip Level - RW612
    WDT0_IRQHandler,             // 16 : Windowed watchdog timer 0 (WDT0)(Cortex-M33 watchdog)
    DMA0_IRQHandler,             // 17 : Direct memory access (DMA) controller 0 (secure or Cortex-M33 DMA)
    GPIO_INTA_IRQHandler,        // 18 : GPIO interrupt A
    GPIO_INTB_IRQHandler,        // 19 : GPIO interrupt B
    PIN_INT0_IRQHandler,         // 20 : Pin interrupt 0 or pattern match engine slice 0
    PIN_INT1_IRQHandler,         // 21 : Pin interrupt 1 or pattern match engine slice 1
    PIN_INT2_IRQHandler,         // 22 : Pin interrupt 2 or pattern match engine slice 2
    PIN_INT3_IRQHandler,         // 23 : Pin interrupt 3 or pattern match engine slice 3
    UTICK_IRQHandler,            // 24 : Micro-tick Timer (UTICK)
    MRT_IRQHandler,              // 25 : Multi-Rate Timer (MRT). Global MRT interrupts
    CTIMER0_IRQHandler,          // 26 : Standard counter/timer CTIMER0
    CTIMER1_IRQHandler,          // 27 : Standard counter/timer CTIMER1
    SCT0_IRQHandler,             // 28 : SCTimer/PWM
    CTIMER3_IRQHandler,          // 29 : Standard counter/timer CTIMER3
    FLEXCOMM0_IRQHandler,        // 30 : Flexcomm Interface 0 (USART, SPI, I2C, I2S)
    FLEXCOMM1_IRQHandler,        // 31 : Flexcomm Interface 1 (USART, SPI, I2C, I2S)
    FLEXCOMM2_IRQHandler,        // 32 : Flexcomm Interface 2 (USART, SPI, I2C, I2S)
    FLEXCOMM3_IRQHandler,        // 33 : Flexcomm Interface 3 (USART, SPI, I2C, I2S)
    Reserved34_IRQHandler,       // 34 : Reserved interrupt
    Reserved35_IRQHandler,       // 35 : Reserved interrupt
    FLEXCOMM14_IRQHandler,       // 36 : Flexcomm Interface 14 (USART, SPI, I2C, I2S)
    Reserved37_IRQHandler,       // 37 : Reserved interrupt
    Reserved38_IRQHandler,       // 38 : Reserved interrupt
    GFMRT_IRQHandler,            // 39 : Free Multi-rate timer (GFMRT). Global MRT interrupts
    Reserved40_IRQHandler,       // 40 : Reserved interrupt
    DMIC_IRQHandler,             // 41 : Digital microphone (DMIC) and DMIC subsystem
    WKDEEPSLEEP_IRQHandler,      // 42 : Wake-up from deep sleep
    HYPERVISOR_IRQHandler,       // 43 : Hypervisor service software interrupt
    SECUREVIOLATION_IRQHandler,  // 44 : Secure violation
    HWVAD_IRQHandler,            // 45 : Hardware Voice Activity Detector
    Reserved46_IRQHandler,       // 46 : Reserved interrupt
    Reserved47_IRQHandler,       // 47 : Reserved interrupt
    RTC_IRQHandler,              // 48 : RTC alarm and wake-up
    Reserved49_IRQHandler,       // 49 : Reserved interrupt
    Reserved50_IRQHandler,       // 50 : Reserved interrupt
    PIN_INT4_IRQHandler,         // 51 : Pin interrupt 4 or pattern match engine slice 4
    PIN_INT5_IRQHandler,         // 52 : Pin interrupt 5 or pattern match engine slice 5
    PIN_INT6_IRQHandler,         // 53 : Pin interrupt 6 or pattern match engine slice 6
    PIN_INT7_IRQHandler,         // 54 : Pin interrupt 7 or pattern match engine slice 7
    CTIMER2_IRQHandler,          // 55 : Standard counter/timer CTIMER2
    CTIMER4_IRQHandler,          // 56 : Standard counter/timer CTIMER4
    OS_EVENT_TIMER_IRQHandler,   // 57 : OS event timer 0
    FLEXSPI_IRQHandler,          // 58 : FLEXSPI interface
    Reserved59_IRQHandler,       // 59 : Reserved interrupt
    Reserved60_IRQHandler,       // 60 : Reserved interrupt
    Reserved61_IRQHandler,       // 61 : Reserved interrupt
    SDIO_IRQHandler,             // 62 : The secure digital interface
    SGPIO_INTA_IRQHandler,       // 63 : Secure GPIO interrupt A
    SGPIO_INTB_IRQHandler,       // 64 : Secure GPIO interrupt B
    Reserved65_IRQHandler,       // 65 : Reserved interrupt
    USB_IRQHandler,              // 66 : High-speed USB device/host
    Reserved67_IRQHandler,       // 67 : Reserved interrupt
    Reserved68_IRQHandler,       // 68 : Reserved interrupt
    Reserved69_IRQHandler,       // 69 : Reserved interrupt
    DMA1_IRQHandler,             // 70 : DMA controller 1 (non-secure or HiFi 4 DMA)
    PUF_IRQHandler,              // 71 : Physical Unclonable Function
    POWERQUAD_IRQHandler,        // 72 : PowerQuad math coprocessor
    Reserved73_IRQHandler,       // 73 : Reserved interrupt
    Reserved74_IRQHandler,       // 74 : Reserved interrupt
    Reserved75_IRQHandler,       // 75 : Reserved interrupt
    Reserved76_IRQHandler,       // 76 : Reserved interrupt
    LCD_IRQHandler,              // 77 : LCDIC
    CAPTIMER_IRQHandler,         // 78 : Capture timer
    Reserved79_IRQHandler,       // 79 : Reserved interrupt
    W2MWKUP_DONE0_IRQHandler,    // 80 : Wi-Fi to MCU, wakeup done 0
    W2MWKUP_DONE1_IRQHandler,    // 81 : Wi-Fi to MCU, wakeup done 1
    W2MWKUP_DONE2_IRQHandler,    // 82 : Wi-Fi to MCU, wakeup done 2
    W2MWKUP_DONE3_IRQHandler,    // 83 : Wi-Fi to MCU, wakeup done 3
    W2MWKUP_DONE4_IRQHandler,    // 84 : Wi-Fi to MCU, wakeup done 4
    W2MWKUP_DONE5_IRQHandler,    // 85 : Wi-Fi to MCU, wakeup done 5
    W2MWKUP_DONE6_IRQHandler,    // 86 : Wi-Fi to MCU, wakeup done 6
    W2MWKUP_DONE7_IRQHandler,    // 87 : Wi-Fi to MCU, wakeup done 7
    W2MWKUP0_IRQHandler,         // 88 : Wi-Fi to MCU, wakeup signal 0
    W2MWKUP1_IRQHandler,         // 89 : Wi-Fi to MCU, wakueup signal 1
    WL_MCI_INT0_IRQHandler,      // 90 : Wi-Fi to MCU interrupt 0
    WL_MCI_INT1_IRQHandler,      // 91 : Reserved for Wi-Fi to MCU
    WL_MCI_INT2_IRQHandler,      // 92 : Reserved for Wi-Fi to MCU
    WL_MCI_INT3_IRQHandler,      // 93 : Reserved for Wi-Fi to MCU
    WL_MCI_INT4_IRQHandler,      // 94 : Reserved for Wi-Fi to MCU
    WL_MCI_INT5_IRQHandler,      // 95 : Reserved for Wi-Fi to MCU
    WL_MCI_INT6_IRQHandler,      // 96 : Reserved for Wi-Fi to MCU
    WL_MCI_INT7_IRQHandler,      // 97 : Reserved for Wi-Fi to MCU
    B2MWKUP_DONE0_IRQHandler,    // 98 : Bluetooth LE/802.15.4 radio to MCU, wakeup done 0
    B2MWKUP_DONE1_IRQHandler,    // 99 : Bluetooth LE/802.15.4 radio to MCU, wakeup done 1
    B2MWKUP_DONE2_IRQHandler,    // 100: Bluetooth LE/802.15.4 radio to MCU, wakeup done 2
    B2MWKUP_DONE3_IRQHandler,    // 101: Bluetooth LE/802.15.4 radio to MCU, wakeup done 3
    B2MWKUP_DONE4_IRQHandler,    // 102: Bluetooth LE/802.15.4 radio to MCU, wakeup done 4
    B2MWKUP_DONE5_IRQHandler,    // 103: Bluetooth LE/802.15.4 radio to MCU, wakeup done 5
    B2MWKUP_DONE6_IRQHandler,    // 104: Bluetooth LE/802.15.4 radio to MCU, wakeup done 6
    B2MWKUP_DONE7_IRQHandler,    // 105: Bluetooth LE/802.15.4 radio to MCU, wakeup done 7
    B2MWKUP0_IRQHandler,         // 106: Bluetooth LE/802.15.4 radio to MCU, wakeup signal 0
    B2MWKUP1_IRQHandler,         // 107: Bluetooth LE/802.15.4 radio to MCU, wakeup signal 1
    BLE_MCI_INT0_IRQHandler,     // 108: Bluetooth LE/802.15.4 radio to MCU interrupt 0
    BLE_MCI_INT1_IRQHandler,     // 109: Reserved for Bluetooth LE/802.15.4 radio to MCU
    BLE_MCI_INT2_IRQHandler,     // 110: Reserved for Bluetooth LE/802.15.4 radio to MCU
    BLE_MCI_INT3_IRQHandler,     // 111: Reserved for Bluetooth LE/802.15.4 radio to MCU
    BLE_MCI_INT4_IRQHandler,     // 112: Reserved for Bluetooth LE/802.15.4 radio to MCU
    BLE_MCI_INT5_IRQHandler,     // 113: Reserved for Bluetooth LE/802.15.4 radio to MCU
    BLE_MCI_INT6_IRQHandler,     // 114: Reserved for Bluetooth LE/802.15.4 radio to MCU
    BLE_MCI_INT7_IRQHandler,     // 115: Reserved for Bluetooth LE/802.15.4 radio to MCU
    PIN0_INT_IRQHandler,         // 116: From AON GPIO
    PIN1_INT_IRQHandler,         // 117: From AON GPIO
    ELS_IRQHandler,              // 118: EdgeLock subsystem (ELS)
    ELS_GDET_IRQHandler,         // 119: ELS IRQ line for GDET error
    ELS_GDET_UM_IRQHandler,      // 120: ELS un-gated latched error
    PKC_INT_IRQHandler,          // 121: Public key crypto-processor (PKC) interrupt
    PKC_ERR_IRQHandler,          // 122: PKC error
    CDOG_INT_IRQHandler,         // 123: Code watch dog timmer interrupt
    GAU_DAC_IRQHandler,          // 124: General analog unit (GAU) digital to analog converter (DAC)
    GAU_ACOMP_WKUP_IRQHandler,   // 125: GAU analog comparator (ACOMP) wake-up
    GAU_ACOMP_IRQHandler,        // 126: GAU analog comparator
    GAU_ADC1_IRQHandler,         // 127: GAU analog to digital converter 1 (ADC1)
    GAU_ADC0_IRQHandler,         // 128: GAU analog to digital converter 0 (ADC0)
    USIM_IRQHandler,             // 129: Universal subscriber identity module (USIM) interface
    OTP_IRQHandler,              // 130: One time programmable (OTP) memory interrupt
    ENET_IRQHandler,             // 131: Etheret interrupt
    ENET_TIMER_IRQHandler,       // 132: Ethernet timer interrupt
    PMIP_IRQHandler,             // 133: Power management IP (PMIP)
    PMIP_CHANGE_IRQHandler,      // 134: PMIP change from 1 to 0
    ITRC_IRQHandler,             // 135: Intrusion and tamper response controller (ITRC) interrupt request
    Reserved136_IRQHandler,      // 136: Reserved interrupt
    Reserved137_IRQHandler,      // 137: Reserved interrupt
    Reserved138_IRQHandler,      // 138: Reserved interrupt
    TRNG_IRQHandler,             // 139: TRNG interrupt request
    ACC_C_INT_IRQHandler,        // 140: AHB memory access checker - Cortex-M33 code bus
    ACC_S_INT_IRQHandler,        // 141: AHB memory access checker - Cortex-M33 sys bus
    WACC_IRQHandler,             // 142: Wi-Fi accessed during power off
    BACC_IRQHandler,             // 143: Bluetooth LE/802.15.4 radio accessed during power off
    GDMA_IRQHandler,             // 144: General purpose direct memory access (GDMA) interrupt
};

extern unsigned char _end[];  // End of data section, start of heap. See link.ld
__attribute__((naked, noreturn)) void Reset_Handler(void) {
  __asm("cpsid i");  // Disable interrupts
  // Config VTOR & MSPLIM register
  __asm("LDR R0, =0xE000ED08  \n"
                  "STR %0, [R0]         \n"
                  "LDR R1, [%0]         \n"
                  "MSR MSP, R1          \n"
                  "MSR MSPLIM, %1       \n"
                  :
                  : "r"(tab), "r"(_end)
                  : "r0", "r1");
  extern long _sbss, _ebss, _sdata, _edata, _sidata, _siram, _siiram, _eiram;
  for (long *dst = &_sbss; dst < &_ebss; dst++) *dst = 0;
  for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;) *dst++ = *src++;
  for (long *dst = &_siram, *src = &_siiram; dst < &_eiram;) *dst++ = *src++;
  SystemInit();
  __asm("cpsie i");  // Reenable interrupts
  // Call main()
  extern void main(void);
  main();
  for (;;) (void) 0;  // Infinite loop
}

void SystemCoreClockUpdate (void) {

}

void Default_Handler(void) {
  for (;;) (void) 0;
}
