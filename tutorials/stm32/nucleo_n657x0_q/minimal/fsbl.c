// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved
#include "hal.h"

#define APP_BASE 0x70100000U  // Should match link.ld

uint32_t SystemCoreClock;
void SystemInit(void) {
  hal_system_init();
  if (!hal_clock_init(HAL_SYS_FREQUENCY)) {
    for (;;) (void) 0;
  }
}

static void xip_init(void) {
  enum {
    XSPI_AF = 9U,
    XSPI_FLASH_SIZE_BITS = 26U,          // 64 MiB MX25UM51245G
    XSPI_KERNEL_DIV = HAL_XSPI_IC3_DIV,  // PLL1 / div, capped at 50 MHz
    XSPI_CSHT_CYCLES = 3U,
    XSPI_REQ2ACK_CYCLES = 1U,
    XSPI_READ_OPCODE = 0x0CU,  // JEDEC 4-byte fast read
    XSPI_READ_DUMMY_CYCLES = 8U,
  };
  static const uint16_t xspi_pins[] = {
      PIN('N', 4), PIN('N', 6),  PIN('N', 8),  PIN('N', 0),
      PIN('N', 3), PIN('N', 5),  PIN('N', 1),  PIN('N', 9),
      PIN('N', 2), PIN('N', 10), PIN('N', 11),
  };
  const uint32_t clear_flags =
      XSPI_FCR_CTCF | XSPI_FCR_CTEF | XSPI_FCR_CSMF | XSPI_FCR_CTOF;

  // Explicitly enable every clock this XIP path depends on.
  RCC->AHB4ENSR = RCC_AHB4ENSR_PWRENS | RCC_AHB4ENSR_GPIONENS;
  RCC->APB4ENSR2 = RCC_APB4ENSR2_SYSCFGENS;
  (void) RCC->AHB4ENR;
  (void) RCC->APB4ENR2;

  RCC->AHB5RSTSR = RCC_AHB5RSTSR_XSPIMRSTS | RCC_AHB5RSTSR_XSPI2RSTS;
  RCC->AHB5RSTCR = RCC_AHB5RSTCR_XSPIMRSTC | RCC_AHB5RSTCR_XSPI2RSTC;
  RCC->AHB4RSTSR = RCC_AHB4RSTSR_GPIONRSTS;
  RCC->AHB4RSTCR = RCC_AHB4RSTCR_GPIONRSTC;

  PWR->SVMCR1 |= PWR_SVMCR1_VDDIO4SV;
  PWR->SVMCR2 |= PWR_SVMCR2_VDDIO5SV;
  PWR->SVMCR3 |= PWR_SVMCR3_VDDIO2SV | PWR_SVMCR3_VDDIO3SV;

  SYSCFG->VDDIO2CCCR = SYSCFG_VDDIO2CCCR_CS | SYSCFG_VDDIO2CCCR_RAPSRC_3 |
                       SYSCFG_VDDIO2CCCR_RANSRC_0 | SYSCFG_VDDIO2CCCR_RANSRC_1 |
                       SYSCFG_VDDIO2CCCR_RANSRC_2;
  SYSCFG->VDDIO3CCCR = SYSCFG_VDDIO3CCCR_CS | SYSCFG_VDDIO3CCCR_RAPSRC_3 |
                       SYSCFG_VDDIO3CCCR_RANSRC_0 | SYSCFG_VDDIO3CCCR_RANSRC_1 |
                       SYSCFG_VDDIO3CCCR_RANSRC_2;
  SYSCFG->VDDIO4CCCR = SYSCFG_VDDIO4CCCR_CS | SYSCFG_VDDIO4CCCR_RAPSRC_3 |
                       SYSCFG_VDDIO4CCCR_RANSRC_0 | SYSCFG_VDDIO4CCCR_RANSRC_1 |
                       SYSCFG_VDDIO4CCCR_RANSRC_2;
  SYSCFG->VDDIO5CCCR = SYSCFG_VDDIO5CCCR_CS | SYSCFG_VDDIO5CCCR_RAPSRC_3 |
                       SYSCFG_VDDIO5CCCR_RANSRC_0 | SYSCFG_VDDIO5CCCR_RANSRC_1 |
                       SYSCFG_VDDIO5CCCR_RANSRC_2;
  SYSCFG->VDDCCCR = SYSCFG_VDDCCCR_CS | SYSCFG_VDDCCCR_RAPSRC_3 |
                    SYSCFG_VDDCCCR_RANSRC_0 | SYSCFG_VDDCCCR_RANSRC_1 |
                    SYSCFG_VDDCCCR_RANSRC_2;

  // Mirror the Cube FSBL topology: XSPI2 kernel clock is IC3 fed from PLL1.
  CLRSET(RCC->IC3CFGR, RCC_IC3CFGR_IC3SEL | RCC_IC3CFGR_IC3INT,
         (((uint32_t) (XSPI_KERNEL_DIV - 1U) << RCC_IC3CFGR_IC3INT_Pos) &
          RCC_IC3CFGR_IC3INT));
  RCC->DIVENSR = RCC_DIVENSR_IC3ENS;
  CLRSET(RCC->CCIPR6, RCC_CCIPR6_XSPI2SEL, RCC_CCIPR6_XSPI2SEL_1);

  RCC->AHB5ENSR = RCC_AHB5ENSR_XSPIMENS | RCC_AHB5ENSR_XSPI2ENS;
  (void) RCC->AHB5ENR;

  for (size_t i = 0; i < sizeof(xspi_pins) / sizeof(xspi_pins[0]); i++) {
    hal_gpio_init(xspi_pins[i], HAL_GPIO_MODE_AF, HAL_GPIO_OTYPE_PUSH_PULL,
                  HAL_GPIO_SPEED_VERY_HIGH, HAL_GPIO_PULL_NONE, XSPI_AF);
  }

  // Keep XIP bring-up small and predictable: fixed 1-1-1 JEDEC 4-byte read.
  XSPI2->CR = (uint32_t) ((4U - 1U) << XSPI_CR_FTHRES_Pos);
  XSPI2->DCR1 = ((uint32_t) XSPI_CSHT_CYCLES << XSPI_DCR1_CSHT_Pos) |
                XSPI_DCR1_MTYP_0 |
                ((uint32_t) XSPI_FLASH_SIZE_BITS << XSPI_DCR1_DEVSIZE_Pos);
  XSPI2->DCR2 = 0U;
  XSPI2->DCR3 = 0U;
  XSPI2->DCR4 = 0U;
  XSPIM->CR = XSPIM_CR_CSSEL_OVR_EN |
              ((uint32_t) XSPI_REQ2ACK_CYCLES << XSPIM_CR_REQ2ACK_TIME_Pos);
  XSPI2->CR |= XSPI_CR_EN;
  XSPI2->TCR = XSPI_READ_DUMMY_CYCLES | XSPI_TCR_SSHIFT;
  XSPI2->DLR = 0U;
  XSPI2->CCR =
      XSPI_CCR_IMODE_0 | XSPI_CCR_ADMODE_0 | XSPI_CCR_ADSIZE | XSPI_CCR_DMODE_0;
  XSPI2->IR = XSPI_READ_OPCODE;
  XSPI2->AR = 0U;
  XSPI2->FCR = clear_flags;

  // In reset topology XSPI2 is wired to XSPIM port 2 already.
  CLRSET(XSPI2->CR, XSPI_CR_FMODE, XSPI_CR_FMODE);
}

static void jump_to_app(void) {
  // Lit blue pin
  hal_gpio_output(PIN('G', 8));
  hal_gpio_write(PIN('G', 8), false);

  // Read stack and app entry point from the app vector table
  uint32_t sp = *(volatile uint32_t *) (APP_BASE + 0U);
  uint32_t pc = *(volatile uint32_t *) (APP_BASE + 4U);

  // Switch to the app's vector table only after the first XIP reads succeed.
  SCB->VTOR = APP_BASE;
  __DSB();
  __ISB();

  // Lit green pin
  hal_gpio_output(PIN('G', 0));
  hal_gpio_write(PIN('G', 0), false);

  __set_MSP(sp);
  ((void (*)(void)) pc)();  // Jump to the application entry point
  for (;;) (void) 0;        // Hang
}

int main(void) {
  xip_init();
  jump_to_app();
  return 0;
}

void _init(void) {
}
