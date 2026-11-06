// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved
//
// Seeed Studio XIAO MG24 hardware abstraction layer. EFR32MG24B220F1536IM48
// reference: Simplicity SDK, https://github.com/SiliconLabs/simplicity_sdk,
// platform/Device/SiliconLabs/EFR32MG24. Board: Seeed XIAO MG24 schematic,
// pin names from Silicon Labs Arduino core, variants/xiao_mg24

#pragma once

#include "em_device.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

uint64_t hal_get_tick(void);
bool hal_timer_expired(volatile uint64_t *t, uint64_t period, uint64_t now);
size_t hal_ram_used(void);
size_t hal_ram_free(void);
uint32_t hal_rng_read(void);  // Random 32 bits, Secure Engine TRNG. See hal.c

#define BIT(x) (1UL << (x))
#define PIN(port, num) ((((port) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINPORT(pin) (pin >> 8)

#define RF_SW_PIN PIN('B', 4)      // Antenna switch VCTL: low onboard, high UFL
#define RF_SW_PWR_PIN PIN('B', 5)  // Antenna switch power

// HFXO crystal tuning, XIAO MG24 board value: Silicon Labs Arduino core,
// sl_device_init_hfxo_config.h. XO gets +40, see ctuneDelta in em_cmu.c
#define HFXO_CTUNE 95
#define HFXO_CTUNE_XO_DELTA 40

enum { GPIO_MODE_DISABLED = 0, GPIO_MODE_INPUT = 1, GPIO_MODE_PUSHPULL = 4 };

static inline void gpio_set_mode(uint16_t pin, uint32_t mode) {
  volatile uint32_t *r = PINNO(pin) < 8 ? &GPIO->P[PINPORT(pin)].MODEL
                                        : &GPIO->P[PINPORT(pin)].MODEH;
  uint32_t shift = (PINNO(pin) & 7) * 4;
  *r = (*r & ~(15UL << shift)) | (mode << shift);
}

static inline void gpio_write(uint16_t pin, bool val) {
  if (val) {
    GPIO->P_SET[PINPORT(pin)].DOUT = BIT(PINNO(pin));
  } else {
    GPIO->P_CLR[PINPORT(pin)].DOUT = BIT(PINNO(pin));
  }
}

static inline void gpio_output(uint16_t pin, bool val) {
  gpio_write(pin, val);
  gpio_set_mode(pin, GPIO_MODE_PUSHPULL);
}

// Start the HFXO crystal oscillator, 39 MHz, and wait until it is ready.
// Steps and default values as in CMU_HFXOInit() and CMU_HFXOINIT_DEFAULT,
// emlib em_cmu.c. The radio needs HFXO, and RAIL expects it configured
static inline void hal_hfxo_init(void) {
  CMU->CLKEN0_SET = CMU_CLKEN0_HFXO0;
  HFXO0->LOCK = HFXO_LOCK_LOCKKEY_UNLOCK;
  HFXO0->CTRL_SET = HFXO_CTRL_DISONDEMAND | HFXO_CTRL_DISONDEMANDBUFOUT;
  HFXO0->CTRL_CLR = HFXO_CTRL_FORCEEN;
  while (HFXO0->STATUS & HFXO_STATUS_ENS) (void) 0;  // Wait until disabled
  while (HFXO0->STATUS & HFXO_STATUS_SYNCBUSY) (void) 0;
  HFXO0->XTALCFG = HFXO_XTALCFG_TIMEOUTCBLSB_T416US |
                   HFXO_XTALCFG_TIMEOUTSTEADY_T833US |  // First lock
                   (32U << _HFXO_XTALCFG_COREBIASSTARTUP_SHIFT) |
                   (32U << _HFXO_XTALCFG_COREBIASSTARTUPI_SHIFT);
  HFXO0->XTALCTRL =
      (HFXO0->XTALCTRL & _HFXO_XTALCTRL_SKIPCOREBIASOPT_MASK) |
      HFXO_XTALCTRL_CTUNEFIXANA_XO |  // 20 dBm PA parts, see em_cmu.h
      ((uint32_t) (HFXO_CTUNE + HFXO_CTUNE_XO_DELTA)
       << _HFXO_XTALCTRL_CTUNEXOANA_SHIFT) |
      ((uint32_t) HFXO_CTUNE << _HFXO_XTALCTRL_CTUNEXIANA_SHIFT) |
      (60U << _HFXO_XTALCTRL_COREBIASANA_SHIFT);
  HFXO0->CFG &= ~(_HFXO_CFG_SQBUFSCHTRGANA_MASK | _HFXO_CFG_ENXIDCBIASANA_MASK |
                  _HFXO_CFG_MODE_MASK);  // Crystal mode
  HFXO0->CTRL = (HFXO0->CTRL & ~(_HFXO_CTRL_FORCEXO2GNDANA_MASK |
                                 _HFXO_CTRL_FORCEXI2GNDANA_MASK |
                                 _HFXO_CTRL_DISONDEMAND_MASK |
                                 _HFXO_CTRL_EM23ONDEMAND_MASK)) |
                HFXO_CTRL_FORCEEN;
  uint32_t ready = HFXO_STATUS_RDY | HFXO_STATUS_COREBIASOPTRDY |
                   HFXO_STATUS_ENS;
  while ((HFXO0->STATUS & ready) != ready) (void) 0;
  HFXO0->CTRL_SET = HFXO_CTRL_DISONDEMAND;
  HFXO0->XTALCFG = (HFXO0->XTALCFG & ~_HFXO_XTALCFG_TIMEOUTSTEADY_MASK) |
                   HFXO_XTALCFG_TIMEOUTSTEADY_T83US;  // Subsequent locks
  HFXO0->XTALCTRL_SET = HFXO_XTALCTRL_SKIPCOREBIASOPT;
  HFXO0->CTRL_CLR = HFXO_CTRL_DISONDEMAND;  // Radio can request it on demand
}

// Lock HFRCODPLL to HFXO at 78 MHz: 39 MHz * (N + 1) / (M + 1), N 3839,
// M 1919, phase lock, falling edge, auto recover. Values and steps as in
// Simplicity SDK CMU_DPLLLock() and the xG24 clock manager configuration.
// HFRCO band 72.6..80 MHz, tuned with its factory calibration from DEVINFO
static inline bool hal_dpll_78mhz(void) {
  uint32_t lock = DPLL_IF_LOCK | DPLL_IF_LOCKFAILLOW | DPLL_IF_LOCKFAILHIGH;
  CMU->CLKEN0_SET = CMU_CLKEN0_DPLL0 | CMU_CLKEN0_HFRCO0;
  DPLL0->EN_CLR = DPLL_EN_EN;
  while (DPLL0->EN & DPLL_EN_DISABLING) (void) 0;
  while (HFRCO0->STATUS & (HFRCO_STATUS_SYNCBUSY | HFRCO_STATUS_FREQBSY))
    (void) 0;
  HFRCO0->CAL = 0xCCF09F00UL | (DEVINFO->HFRCODPLLCAL[16].HFRCODPLLCAL &
                                _HFRCO_CAL_TUNING_MASK);
  DPLL0->CFG1 = (3839UL << _DPLL_CFG1_N_SHIFT) | (1919UL << _DPLL_CFG1_M_SHIFT);
  CMU->DPLLREFCLKCTRL = CMU_DPLLREFCLKCTRL_CLKSEL_HFXO;
  DPLL0->CFG = (1UL << _DPLL_CFG_AUTORECOVER_SHIFT) |  // Edge: falling, 0
               (_DPLL_CFG_MODE_PLL << _DPLL_CFG_MODE_SHIFT);
  DPLL0->IF_CLR = lock;
  DPLL0->EN_SET = DPLL_EN_EN;
  while ((DPLL0->IF & lock) == 0) (void) 0;
  return (DPLL0->IF & lock) == DPLL_IF_LOCK;
}

// Run the CPU at its maximum, 78 MHz, from HFRCODPLL locked to HFXO. First
// set the limits for 78 MHz, datasheet table 4.2 and em_cmu.c: flash 1 wait
// state above 40 MHz; PCLK max 50 MHz, so SYSCLK / 2; radio HCLK max 40 MHz,
// so SYSCLK / 2. Voltage scaling is VSCALE2 after reset, fine for 78 MHz.
// If the DPLL does not lock, stay on HFXO, 39 MHz
static inline void hal_clock_init(void) {
  hal_hfxo_init();
  CMU->CLKEN1_SET = CMU_CLKEN1_MSC;
  MSC->LOCK = MSC_LOCK_LOCKKEY_UNLOCK;
  MSC->READCTRL = (MSC->READCTRL & ~_MSC_READCTRL_MODE_MASK) |
                  MSC_READCTRL_MODE_WS1;
  CMU->SYSCLKCTRL_SET = CMU_SYSCLKCTRL_PCLKPRESC | CMU_SYSCLKCTRL_RHCLKPRESC;
  if (hal_dpll_78mhz()) {
    SystemHFRCODPLLClockSet(78000000);
    CMU->SYSCLKCTRL = (CMU->SYSCLKCTRL & ~_CMU_SYSCLKCTRL_CLKSEL_MASK) |
                      CMU_SYSCLKCTRL_CLKSEL_HFRCODPLL;
  } else {
    CMU->SYSCLKCTRL = (CMU->SYSCLKCTRL & ~_CMU_SYSCLKCTRL_CLKSEL_MASK) |
                      CMU_SYSCLKCTRL_CLKSEL_HFXO;
  }
  SystemCoreClock = SystemSYSCLKGet();     // HCLK = SYSCLK, no prescaler
  SysTick_Config(SystemCoreClock / 1000);  // Sys tick every 1ms
}

// PCLK, the peripheral clock: SYSCLK, divided by 2 if PCLKPRESC is set
static inline uint32_t hal_pclk(void) {
  bool half = (CMU->SYSCLKCTRL & CMU_SYSCLKCTRL_PCLKPRESC) != 0;
  return half ? SystemCoreClock / 2 : SystemCoreClock;
}

static inline void hal_gpio_output(uint16_t pin) {
  CMU->CLKEN0_SET = CMU_CLKEN0_GPIO;
  gpio_set_mode(pin, GPIO_MODE_PUSHPULL);
}

static inline void hal_gpio_toggle(uint16_t pin) {
  GPIO->P_TGL[PINPORT(pin)].DOUT = BIT(PINNO(pin));
}

// USART0, 8N1, on tx and rx pins: any pin, through GPIO routing. On XIAO
// MG24, PA8 TX and PA9 RX go to the SAMD11 USB serial bridge. USART0 is
// clocked by PCLK
static inline void hal_uart_init(USART_TypeDef *uart, uint16_t tx, uint16_t rx,
                                 unsigned long baud) {
  uint32_t ovs = 16, div, pclk = hal_pclk();
  CMU->CLKEN0_SET = CMU_CLKEN0_GPIO | CMU_CLKEN0_USART0;
  gpio_output(tx, 1);
  gpio_set_mode(rx, GPIO_MODE_INPUT);
  GPIO->USARTROUTE[0].TXROUTE =
      ((uint32_t) PINPORT(tx) << _GPIO_USART_TXROUTE_PORT_SHIFT) |
      ((uint32_t) PINNO(tx) << _GPIO_USART_TXROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[0].RXROUTE =
      ((uint32_t) PINPORT(rx) << _GPIO_USART_RXROUTE_PORT_SHIFT) |
      ((uint32_t) PINNO(rx) << _GPIO_USART_RXROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[0].ROUTEEN =
      GPIO_USART_ROUTEEN_TXPEN | GPIO_USART_ROUTEEN_RXPEN;
  uart->EN = USART_EN_EN;
  uart->CTRL = 0;  // Async, 16x oversampling
  uart->FRAME = USART_FRAME_DATABITS_EIGHT | USART_FRAME_STOPBITS_ONE |
                USART_FRAME_PARITY_NONE;
  // CLKDIV = 256 * (fPCLK / (ovs * baud) - 1), see USART_BaudrateAsyncSet()
  div = (32 * pclk + (ovs * baud) / 2) / (ovs * baud) - 32;
  uart->CLKDIV = (div * 8) & _USART_CLKDIV_DIV_MASK;
  uart->CMD = USART_CMD_TXEN | USART_CMD_RXEN;
}

static inline void hal_uart_write_byte(USART_TypeDef *uart, uint8_t byte) {
  while ((uart->STATUS & USART_STATUS_TXBL) == 0) (void) 0;
  uart->TXDATA = byte;
}

static inline void hal_uart_write_buf(USART_TypeDef *uart, const char *buf,
                                      size_t len) {
  while (len-- > 0) hal_uart_write_byte(uart, *(const uint8_t *) buf++);
}

// Random numbers come from the Secure Engine TRNG, see hal_rng_read(): clock
// its mailbox
static inline void hal_rng_init(void) {
  CMU->CLKEN1_SET = CMU_CLKEN1_SEMAILBOXHOST;
}

// Prepare for RAIL. Set all IRQs to priority 5, as Silicon Labs
// sl_interrupt_manager_init() does: RAIL atomic sections mask priority
// numbers 3 and above. Route the radio to the onboard ceramic antenna.
// FM8625H switch: VCTL low connects ANT to RF1, the chip antenna; high RF2,
// the U.FL socket
static inline void hal_radio_init(void) {
  for (int i = 0; i < EXT_IRQ_COUNT; i++) NVIC_SetPriority((IRQn_Type) i, 5);
  CMU->CLKEN0_SET = CMU_CLKEN0_GPIO;
  gpio_output(RF_SW_PWR_PIN, 1);
  gpio_output(RF_SW_PIN, 0);
}
