// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "RW612.h"
#include "fsl_clock.h"
#include "fsl_io_mux.h"
#include "fsl_reset.h"
#include "fsl_trng.h"

#define BIT(x) (1UL << (x))
#define CLRSET(reg, clear, set) ((reg) = ((reg) & ~(clear)) | (set))
#define PIN(bank, num) ((bank << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

void hal_init(void);
size_t hal_ram_free(void);
size_t hal_ram_used(void);
bool hal_timer_expired(volatile uint64_t *t, uint64_t period, uint64_t now);
uint64_t hal_get_tick(void);

static inline void spin(volatile uint32_t count) {
  while (count--)
    (void)0;
}

#define SYS_FREQUENCY 260000000UL
#define FRG_PLL_CLKDIV 13

enum { HAL_GPIO_MODE_INPUT, HAL_GPIO_MODE_OUTPUT };
enum {
  HAL_GPIO_TYPE_NOPULL_WEAKEST = 0x0,
  HAL_GPIO_TYPE_NOPULL_WEAK = 0x4,
  HAL_GPIO_TYPE_NOPULL_STRONG = 0x8,
  HAL_GPIO_TYPE_NOPULL_STRONGEST = 0xC,
  HAL_GPIO_TYPE_PULLUP_WEAKEST = 0x1,
  HAL_GPIO_TYPE_PULLUP_WEAK = 0x5,
  HAL_GPIO_TYPE_PULLUP_STRONG = 0x9,
  HAL_GPIO_TYPE_PULLUP_STRONGEST = 0xD,
  HAL_GPIO_TYPE_PULLDOWN_WEAKEST = 0x2,
  HAL_GPIO_TYPE_PULLDOWN_WEAK = 0x6,
  HAL_GPIO_TYPE_PULLDOWN_STRONG = 0xA,
  HAL_GPIO_TYPE_PULLDOWN_STRONGEST = 0xE,
  HAL_GPIO_TYPE_NOPULL = HAL_GPIO_TYPE_NOPULL_STRONGEST,
  HAL_GPIO_TYPE_PULLUP = HAL_GPIO_TYPE_PULLUP_STRONGEST,
  HAL_GPIO_TYPE_PULLDOWN = HAL_GPIO_TYPE_PULLDOWN_STRONGEST,
};

// Wrapper structure over the IO_MUX_* macros in fsl_io_mux.h
// To use those macros with our gpio functions,
// wrap them inside this structure as shown in the example from the
// ethernet_init function.
typedef struct
{
  uint32_t pinLowMask, pinHighMask, gpioFcSetMask,
  gpioFcClrMask, fselSetMask, fselClrMask, ctimerSetMask,
  ctimerClrMask, sctimerSetMask, sctimerClrMask;
} io_mux_config_t;

static inline void hal_gpio_init(uint16_t pin, uint8_t mode, uint8_t type,
                              io_mux_config_t *mux_config) {
  uint8_t bank = PINBANK(pin);
  uint8_t pin_no = PINNO(pin);
  if (mode == HAL_GPIO_MODE_INPUT) {
    GPIO->DIR[bank] &= ~(1 << pin_no);
  } else {
    GPIO->DIR[bank] |= (1 << pin_no);
  }

  // configure pull
  pin_no += bank * 32;
  uint32_t shift = (pin_no % 16) * 2;
  uint32_t pull_val = (uint32_t)type & 3;
  uint32_t drive_val = ((uint32_t)type >> 2) & 3;
  volatile uint32_t *pull_reg, *drive_reg;
  if (pin_no < 22 || pin_no > 27) {
    pull_reg = &SOCCTRL->PAD_PU_PD_EN0 + pin_no / 16;
    drive_reg = &SOCCTRL->SR_CONFIG0 + pin_no / 16;
  } else {
    pull_reg = &AON_SOC_CIU->PAD_PU_PD_EN1;
    drive_reg = &AON_SOC_CIU->SR_CONFIG1;
  }
  *pull_reg = (*pull_reg & ~(3 << shift)) | (pull_val << shift);
  *drive_reg = (*drive_reg & ~(3 << shift)) | (drive_val << shift);

  if (mux_config) {
    IO_MUX_SetPinMux(mux_config->pinLowMask, mux_config->pinHighMask,
      mux_config->gpioFcSetMask, mux_config->gpioFcClrMask,
      mux_config->fselSetMask, mux_config->fselClrMask,
      mux_config->ctimerSetMask, mux_config->ctimerClrMask,
      mux_config->sctimerSetMask, mux_config->sctimerClrMask
    );
  } else {
    // if no mux config is specified, assume default gpio settings and
    // no special function and just enable the IO
    IO_MUX_SetPinMux(bank ? 0 : BIT(PINNO(pin)), !bank ? 0 : BIT(PINNO(pin)),
                    1 << 16, 0, 0, 0, 0, 0, 0, 0);
  }
}

static inline void hal_gpio_input(uint16_t pin) {
  hal_gpio_init(pin, HAL_GPIO_MODE_INPUT, HAL_GPIO_TYPE_PULLUP_STRONGEST, NULL);
}
static inline void hal_gpio_output(uint16_t pin) {
  hal_gpio_init(pin, HAL_GPIO_MODE_OUTPUT, HAL_GPIO_TYPE_NOPULL_STRONGEST, NULL);
}

static inline bool hal_gpio_read(uint16_t pin) {
  return GPIO->PIN[PINBANK(pin)] & (1 << PINNO(pin));
}

static inline void hal_gpio_write(uint16_t pin, bool value) {
  if (value) {
    GPIO->SET[PINBANK(pin)] |= (1 << PINNO(pin));
  } else {
    GPIO->CLR[PINBANK(pin)] |= (1 << PINNO(pin));
  }
}

static inline void hal_gpio_toggle(uint16_t pin) {
  hal_gpio_read(pin) ? hal_gpio_write(pin, false) : hal_gpio_write(pin, true);
}

#if defined(__ZEPHYR__)
#undef MG_HAL_DISABLE_SYSTEM_INIT
#define MG_HAL_DISABLE_SYSTEM_INIT 1
#undef MG_HAL_DISABLE_NEWLIB
#define MG_HAL_DISABLE_NEWLIB 1
#undef MG_HAL_DISABLE_CLOCK
#define MG_HAL_DISABLE_CLOCK 1
#undef MG_HAL_DISABLE_RANDOM
#define MG_HAL_DISABLE_RANDOM 1
#undef MG_HAL_DISABLE_MILLIS
#define MG_HAL_DISABLE_MILLIS 1
#undef MG_HAL_DISABLE_UART
#define MG_HAL_DISABLE_UART 1
#undef MG_HAL_DISABLE_ETHERNET
#define MG_HAL_DISABLE_ETHERNET 1
#undef MG_HAL_SYSTICK
#define MG_HAL_SYSTICK MG_HAL_SYSTICK_NONE
#endif

static inline void hal_uart_init(USART_Type *uart, unsigned long baud) {
  if (uart == USART0) {
    IO_MUX_SetPinMux(IO_MUX_FC0_USART_DATA);
  } else if (uart == USART1) {
    IO_MUX_SetPinMux(IO_MUX_FC1_USART_DATA);
  } else if (uart == USART2) {
    IO_MUX_SetPinMux(IO_MUX_FC2_USART_DATA);
  } else if (uart == USART3) {
    IO_MUX_SetPinMux(IO_MUX_FC3_USART_DATA);
  } else if (uart == USART14) {
    IO_MUX_SetPinMux(IO_MUX_FC14_USART_DATA);
  } else {
    return;
  }

  uint32_t uart_index = ((uint32_t)uart - USART0_BASE) / 0x1000;

  // set FRG clk
  CLKCTL1->FLEXCOMM[uart_index].FRGCLKSEL = kCLOCK_FrgPllDiv;
  CLKCTL1->FLEXCOMM[uart_index].FRGCTL =
      (CLKCTL1_FRGCTL_MULT(0) | CLKCTL1_FRGCTL_DIV(255));
  CLOCK_AttachClk(CLKCTL1_TUPLE_MUXA(0x508 + uart_index * 0x20, 4));

  // set FLEXCOMM to USART mode
  CLOCK_EnableClock(CLK_GATE_DEFINE(CLK_CTL1_PSCCTL0, uart_index + 8));
  uint32_t mask = BIT(uart_index + 8);
  RSTCTL1->PRSTCTL0_SET = mask;
  while ((RSTCTL1->PRSTCTL0 & mask) == 0)
    spin(1);
  RSTCTL1->PRSTCTL0_CLR = mask;
  while ((RSTCTL1->PRSTCTL0 & mask) == mask)
    spin(1);
  ((FLEXCOMM_Type *)uart)->PSELID = 1;

  uart->FIFOCFG |= USART_FIFOCFG_EMPTYTX_MASK |
                   USART_FIFOCFG_ENABLETX_MASK; // empty and enable txFIFO
  uart->CFG = BIT(2) | BIT(0);                  //  8 bit data length and enable

  // compute baud
  uint32_t uart_freq = SYS_FREQUENCY / FRG_PLL_CLKDIV;
  uint32_t best_diff = (uint32_t)-1, best_osr = 0xF, best_brg = (uint32_t)-1;
  uint32_t osr, brg, diff, current_baud;
  for (osr = 15; osr >= 4; osr--) {
    brg = (((uart_freq * 10) / ((osr + 1) * baud)) - 5) / 10;
    if (brg > 0xFFFF)
      continue;
    current_baud = uart_freq / ((osr + 1) * (brg + 1));
    diff =
        (baud < current_baud) ? (current_baud - baud) : (baud - current_baud);
    if (diff < best_diff) {
      best_diff = diff;
      best_osr = osr;
      best_brg = brg;
    }
  }
  uart->OSR = best_osr;
  uart->BRG = best_brg;
}

static inline void hal_uart_write_byte(USART_Type *uart, uint8_t byte) {
  while ((uart->FIFOSTAT & USART_FIFOSTAT_TXNOTFULL_MASK) == 0)
    spin(1);
  uart->FIFOWR = byte;
}
static inline void hal_uart_write_buf(USART_Type *uart, char *buf, size_t len) {
  while (len-- > 0)
    hal_uart_write_byte(uart, *(uint8_t *)buf++);
}

static inline void hal_rng_init(void) {
  trng_config_t config;
  TRNG_GetDefaultConfig(&config);
  TRNG_Init(TRNG, &config);
}

static inline uint32_t hal_rng_read(void) {
  uint32_t value;
  TRNG_GetRandomData(TRNG, &value, sizeof(value));
  return value;
}

static inline void hal_ethernet_init(void) {
  CLOCK_EnableClock(kCLOCK_TddrMciEnetClk);
  hal_gpio_init(PIN(0, 25), HAL_GPIO_MODE_OUTPUT, IO_MUX_PinConfigPullUpDriveWeak,
            &(io_mux_config_t){ IO_MUX_ENET_CLK }); // ENET_CLK on GPIO_25
  IO_MUX_SetPinMux(
      IO_MUX_ENET_RX); // ENET_RX on pin GPIO_62, GPIO_63, GPIO_22, GPIO_23
  IO_MUX_SetPinMux(IO_MUX_ENET_TX); // ENET_TX on pin GPIO_58, GPIO_59, GPIO_60
  IO_MUX_SetPinMux(IO_MUX_ENET_MDIO); // ENET_MDIO on pin GPIO_56, GPIO_57

  // reset ENET peripheral
  RSTCTL0->PRSTCTL1_SET = BIT(25);
  while ((RSTCTL0->PRSTCTL1 & BIT(25)) == 0)
    spin(1);
  RSTCTL0->PRSTCTL1_CLR = BIT(25);
  while ((RSTCTL0->PRSTCTL1 & BIT(25)) == BIT(25))
    spin(1);
  RSTCTL0->PRSTCTL1_SET = BIT(26);
  while ((RSTCTL0->PRSTCTL1 & BIT(26)) == 0)
    spin(1);
  RSTCTL0->PRSTCTL1_CLR = BIT(26);
  while ((RSTCTL0->PRSTCTL1 & BIT(26)) == BIT(26))
    spin(1);

  // Enable clocks
  CLOCK_EnableClock(kCLOCK_EnetIpg);
  CLOCK_EnableClock(kCLOCK_EnetIpgS);

  NVIC_EnableIRQ(ENET_IRQn);
}
extern void set_flexspi_clock(FLEXSPI_Type *base, uint32_t src,
                              uint32_t divider);
static inline void hal_clock_init(void) {
  const clock_avpll_config_t avpllConfig_BOARD_BootClockRUN = {
      .ch1Freq =
          kCLOCK_AvPllChFreq12p288m,    // AVPLL channel frequency 12.288 MHz
      .ch2Freq = kCLOCK_AvPllChFreq64m, // AVPLL channel frequency 64 MHz
      .enableCali = true,               // AVPLL calibration is enabled
  };
  if ((PMU->CAU_SLP_CTRL & PMU_CAU_SLP_CTRL_SOC_SLP_RDY_MASK) == 0) {
    CLOCK_EnableClock(
        kCLOCK_RefClkCauSlp); // Enable the CAU sleep clock for PMU
  }
  if ((SYSCTL2->SOURCE_CLK_GATE & SYSCTL2_SOURCE_CLK_GATE_REFCLK_SYS_CG_MASK) !=
      0) {
    CLOCK_EnableClock(kCLOCK_RefClkSys); // Enable the REFCLK_SYS clock
  }
  CLOCK_InitT3RefClk(kCLOCK_T3MciIrc48m); // Initialize T3 PLL and enable
                                          // outputs that are not clock gated
  CLOCK_EnableClock(
      kCLOCK_T3PllMciIrcClk); // Enable FFRO - T3 PLL 48/60 MHz IRC clock output
  CLOCK_EnableClock(
      kCLOCK_T3PllMci256mClk); // Enable T3 PLL 256 MHz clock output

  // Move FLEXSPI clock source to T3 256M / 4 to avoid instruction/data fetch
  // issue in XIP when updating PLL and main clock.
  set_flexspi_clock(FLEXSPI, 6, 4);
  CLOCK_AttachClk(kSYSOSC_to_MAIN_CLK); // Set core clock to safe system
                                        // oscillator for initialization
  CLOCK_SetClkDiv(kCLOCK_DivSysCpuAhbClk,
                  1); // Set system CPU AHB clock divider to 1
  CLOCK_InitTcpuRefClk(
      3120000000UL,
      kCLOCK_TcpuFlexspiDiv10); // Initialize TCPU PLL and enable outputs
  CLOCK_EnableClock(kCLOCK_TcpuMciClk); // Enable TCPU PLL MCI clock output
  CLOCK_InitTddrRefClk(
      kCLOCK_TddrFlexspiDiv10); // Initialize TDDR PLL and enable outputs
  CLOCK_EnableClock(
      kCLOCK_TddrMciFlexspiClk); // Enable TDDR PLL FlexSPI clock output
  CLOCK_InitAvPll(&avpllConfig_BOARD_BootClockRUN); // Initialize AVPLL and
                                                    // enable both channels
  CLOCK_AttachClk(kRC32K_to_CLK32K);                // Switch CLK32K to RC32K
  CLOCK_AttachClk(
      kSYSTICK_DIV_to_SYSTICK_CLK); // Switch SYSTICK_CLK to SYSTICK_DIV
  CLOCK_SetClkDiv(kCLOCK_DivAudioPllClk, 1U); // Set AUDIOPLLCLKDIV divider to 1
  CLOCK_SetClkDiv(kCLOCK_DivPllFrgClk,
                  FRG_PLL_CLKDIV);           // Set FRGPLLCLKDIV divider
  CLOCK_SetClkDiv(kCLOCK_DivMainPllClk, 1U); // Set MAINPLLCLKDIV divider to 1
  CLOCK_SetClkDiv(kCLOCK_DivAux0PllClk, 1U); // Set AUX0PLLCLKDIV divider to 1
  CLOCK_SetClkDiv(kCLOCK_DivSystickClk, 1U); // Set SYSTICKFCLKDIV divider to 1
  CLOCK_SetClkDiv(kCLOCK_DivPmuFclk, 5U);    // Set PMUFCLKDIV divider to 5
  CLOCK_AttachClk(
      kMAIN_PLL_to_MAIN_CLK); // Select the main clock source for system clock
  set_flexspi_clock(FLEXSPI, 2U, 2U); // set_flexspi_clock

  CLOCK_EnableClock(kCLOCK_HsGpio0);
  RSTCTL1->PRSTCTL1_SET = BIT(0); // Reset GPIO_0
  while ((RSTCTL1->PRSTCTL1 & BIT(0)) == 0)
    spin(1);
  RSTCTL1->PRSTCTL1_CLR = BIT(0);
  while ((RSTCTL1->PRSTCTL1 & BIT(0)) == BIT(0))
    spin(1);

  CLOCK_EnableClock(kCLOCK_HsGpio1);
  RSTCTL1->PRSTCTL1_SET = BIT(1); // Reset GPIO_1
  while ((RSTCTL1->PRSTCTL1 & BIT(1)) == 0)
    spin(1);
  RSTCTL1->PRSTCTL1_CLR = BIT(1);
  while ((RSTCTL1->PRSTCTL1 & BIT(1)) == BIT(1))
    spin(1);
}

#if defined(__MCUXPRESSO)
#undef MG_HAL_DISABLE_SYSTEM_INIT
#define MG_HAL_DISABLE_SYSTEM_INIT 1
#undef MG_HAL_SBRK_NONE
#define MG_HAL_SBRK_NONE 1
#endif

static inline void hal_system_init(void) {
  SCB->CPACR |=
    (3UL << 0 * 2) | (3UL << 1 * 2) |   // Enable PowerQuad (CPO/CP1)
    (3UL << 10 * 2) | (3UL << 11 * 2); // Enable FPU
  SCB->NSACR |= ((3UL << 0) | (3UL << 10));
  CACHE64_CTRL0->CCR = CACHE64_CTRL_CCR_INVW1_MASK | CACHE64_CTRL_CCR_INVW0_MASK;
  CACHE64_CTRL0->CCR |= CACHE64_CTRL_CCR_GO_MASK;
  while ((CACHE64_CTRL0->CCR & CACHE64_CTRL_CCR_GO_MASK) != 0) spin(1);
  // Enable cache, enable write buffer
  CACHE64_CTRL0->CCR = (CACHE64_CTRL_CCR_ENWRBUF_MASK | CACHE64_CTRL_CCR_ENCACHE_MASK);

  // Set whole FlexSPI0 space to write through
  CACHE64_POLSEL0->REG0_TOP = 0x07FFFC00U;
  CACHE64_POLSEL0->REG1_TOP = 0x0U;
  CACHE64_POLSEL0->POLSEL   = 0x1U;
  
  __DSB();
  __ISB();
}
