#include "xmc7200d_e272k8384.h"

void Reset_Handler(void);    // Defined below
void Dummy_Handler(void);    // Defined below
void SysTick_Handler(void);  // Defined in main.c
void SystemInit(void);       // Defined in main.c, called by reset handler
void _estack(void);          // Defined in link.ld

#define WEAK_ALIAS __attribute__((weak, alias("Default_Handler")))

WEAK_ALIAS void NMI_Handler(void);
WEAK_ALIAS void HardFault_Handler(void);
WEAK_ALIAS void SVCall_Handler(void);
WEAK_ALIAS void PendSV_Handler(void);
WEAK_ALIAS void SysTick_Handler(void);

WEAK_ALIAS void Default_Intr_Handler(void);
WEAK_ALIAS void CM0P_CpuIntr0_Handler(void);
WEAK_ALIAS void CM0P_CpuIntr1_Handler(void);
WEAK_ALIAS void CM0P_CpuIntr2_Handler(void);
WEAK_ALIAS void CM0P_CpuIntr4_Handler(void);
WEAK_ALIAS void CM0P_CpuIntr5_Handler(void);
WEAK_ALIAS void CM0P_CpuIntr6_Handler(void);
WEAK_ALIAS void CM0P_CpuIntr7_Handler(void);
WEAK_ALIAS void ETH_IRQHandler(void);

void CM0P_CpuIntr3_Handler(void) {
	ETH_IRQHandler();
}

__attribute__((section(".vectors"))) void (*const tab[16 + 16])(void) = {
	_estack,
	Reset_Handler,
	NMI_Handler,
	HardFault_Handler,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	SVCall_Handler,
	0,
	0,
	PendSV_Handler,
	SysTick_Handler,
	CM0P_CpuIntr0_Handler,
	CM0P_CpuIntr1_Handler,
	CM0P_CpuIntr2_Handler,
	CM0P_CpuIntr3_Handler,
	CM0P_CpuIntr4_Handler,
	CM0P_CpuIntr5_Handler,
	CM0P_CpuIntr6_Handler,
	CM0P_CpuIntr7_Handler,
	Default_Intr_Handler,
	Default_Intr_Handler,
	Default_Intr_Handler,
	Default_Intr_Handler,
	Default_Intr_Handler,
	Default_Intr_Handler,
	Default_Intr_Handler,
	Default_Intr_Handler
};

__attribute__((naked, noreturn)) void Reset_Handler(void) {
  // Clear BSS section, and copy data section from flash to RAM
  extern long _sbss, _ebss, _sdata, _edata, _sidata;
  for (long *src = &_sbss; src < &_ebss; src++) *src = 0;
  for (long *src = &_sdata, *dst = &_sidata; src < &_edata;) *src++ = *dst++;

  SCB->VTOR = (uint32_t) &tab;
  SystemInit();

  // Call main()
  extern void main(void);
  main();
  for (;;) (void) 0;  // Infinite loop
}

void Default_Handler(void) {
  for (;;) (void) 0;
}
