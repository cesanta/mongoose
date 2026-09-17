// https://www.nxp.com/docs/en/reference-manual/IMXRT1180RM.pdf
// https://www.nxp.com/docs/en/data-sheet/IMXRT1180EC.pdf 
// https://www.mouser.com/catalog/specsheets/NXP_0-28-2026_FRDM-IMXRT1186.pdf

#pragma once

#include "MIMXRT1186_cm33.h"
#include "fsl_clock.h"
#include "fsl_ele_base_api.h"
#include "fsl_trdc.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PIN(bank, num) ((((bank) - '0') << 8) | (num))
#define PINNO(pin) ((pin) & 255)
#define PINBANK(pin) ((pin) >> 8)

#define HAL_DEBUG_UART_CLK_ROOT kCLOCK_Root_Lpuart0102
#define HAL_DEBUG_UART_CLK 24000000UL
#define HAL_DEBUG_UART_TX_PIN PIN('1', 8)  // GPIO_AON_08, ball A7
#define HAL_DEBUG_UART_RX_PIN PIN('1', 9)  // GPIO_AON_09, ball B12

#define SYS_FREQUENCY 240000000UL

void hal_init(void);
size_t hal_ram_free(void);
size_t hal_ram_used(void);

bool hal_timer_expired(volatile uint64_t *t, uint64_t period, uint64_t now);
uint64_t hal_get_tick(void);

static inline void spin(volatile uint32_t count) {
  while (count--) (void) 0;
}

enum { HAL_GPIO_MODE_INPUT, HAL_GPIO_MODE_OUTPUT };
enum { HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_OTYPE_OPEN_DRAIN };
enum { HAL_GPIO_SPEED_LOW, HAL_GPIO_SPEED_HIGH };
enum { HAL_GPIO_PULL_NONE, HAL_GPIO_PULL_UP, HAL_GPIO_PULL_DOWN };

static void hal_flexspi2_clock_config(uint8_t src, uint32_t divider)
    __attribute__((section(".iram")));

static void hal_flexspi2_clock_config(uint8_t src, uint32_t divider) {
  const clock_root_t root = kCLOCK_Root_Flexspi2;
  const clock_lpcg_t lpcg = kCLOCK_Flexspi2;
  FLEXSPI_Type *base = FLEXSPI2;
  uint32_t status, last_status, retry;

  if ((CCM->CLOCK_ROOT[root].CONTROL & CCM_CLOCK_ROOT_CONTROL_MUX_MASK) ==
          CCM_CLOCK_ROOT_CONTROL_MUX(src) &&
      (CCM->CLOCK_ROOT[root].CONTROL & CCM_CLOCK_ROOT_CONTROL_DIV_MASK) ==
          CCM_CLOCK_ROOT_CONTROL_DIV(divider - 1U)) {
    return;
  }

  // BOARD_DeinitFlash(): flush/disable the CM33 cache, then stop FlexSPI2.
  if ((XCACHE_PC->CCR & XCACHE_CCR_ENCACHE_MASK) != 0U) {
    XCACHE_PC->CCR |=
        XCACHE_CCR_PUSHW0_MASK | XCACHE_CCR_PUSHW1_MASK | XCACHE_CCR_GO_MASK;
    while ((XCACHE_PC->CCR & XCACHE_CCR_GO_MASK) != 0U) (void) 0;
    XCACHE_PC->CCR &=
        ~(XCACHE_CCR_PUSHW0_MASK | XCACHE_CCR_PUSHW1_MASK |
          XCACHE_CCR_ENCACHE_MASK);
    __ISB();
    __DSB();
  }

  base->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK;
  while ((base->STS0 & (FLEXSPI_STS0_ARBIDLE_MASK |
                        FLEXSPI_STS0_SEQIDLE_MASK)) !=
         (FLEXSPI_STS0_ARBIDLE_MASK | FLEXSPI_STS0_SEQIDLE_MASK)) {
    (void) 0;
  }
  base->MCR0 |= FLEXSPI_MCR0_MDIS_MASK;

  CCM->LPCG[lpcg].DIRECT &= ~CCM_LPCG_DIRECT_ON_MASK;
  __DSB();
  __ISB();
  while ((CCM->LPCG[lpcg].STATUS0 & CCM_LPCG_STATUS0_ON_MASK) != 0U)
    (void) 0;

  CCM->CLOCK_ROOT[root].CONTROL =
      CCM_CLOCK_ROOT_CONTROL_MUX(src) |
      CCM_CLOCK_ROOT_CONTROL_DIV(divider - 1U);
  __DSB();
  __ISB();
  (void) CCM->CLOCK_ROOT[root].CONTROL;

  CCM->LPCG[lpcg].DIRECT |= CCM_LPCG_DIRECT_ON_MASK;
  __DSB();
  __ISB();
  while ((CCM->LPCG[lpcg].STATUS0 & CCM_LPCG_STATUS0_ON_MASK) == 0U)
    (void) 0;

  // BOARD_InitFlash(): restore the reference project's AHB buffers and DLL.
  if ((base->AHBCR & FLEXSPI_AHBCR_PREFETCHEN_MASK) != 0U) {
    base->AHBRXBUFCR0[0] =
        FLEXSPI_AHBRXBUFCR0_PREFETCHEN_MASK |
        FLEXSPI_AHBRXBUFCR0_PRIORITY(4U) |
        FLEXSPI_AHBRXBUFCR0_MSTRID(0U) |
        FLEXSPI_AHBRXBUFCR0_BUFSZ(512U / 8U);  // NETC
    base->AHBRXBUFCR0[1] =
        FLEXSPI_AHBRXBUFCR0_PREFETCHEN_MASK |
        FLEXSPI_AHBRXBUFCR0_PRIORITY(7U) |
        FLEXSPI_AHBRXBUFCR0_MSTRID(8U) |
        FLEXSPI_AHBRXBUFCR0_BUFSZ(512U / 8U);  // CM33
    base->AHBRXBUFCR0[2] =
        FLEXSPI_AHBRXBUFCR0_PREFETCHEN_MASK |
        FLEXSPI_AHBRXBUFCR0_PRIORITY(7U) |
        FLEXSPI_AHBRXBUFCR0_MSTRID(9U) |
        FLEXSPI_AHBRXBUFCR0_BUFSZ(512U / 8U);  // CM7
    for (uint32_t i = 3U; i < FLEXSPI_AHBRXBUFCR0_COUNT; i++) {
      base->AHBRXBUFCR0[i] =
          FLEXSPI_AHBRXBUFCR0_PREFETCHEN_MASK |
          FLEXSPI_AHBRXBUFCR0_BUFSZ(0U);
    }
  }

  base->DLLCR[0] = 1U;
  base->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK;
  base->MCR0 |= FLEXSPI_MCR0_SWRESET_MASK;
  while ((base->MCR0 & FLEXSPI_MCR0_SWRESET_MASK) != 0U) (void) 0;

  if ((base->DLLCR[0] & FLEXSPI_DLLCR_DLLEN_MASK) != 0U) {
    last_status = base->STS2;
    retry = 10U;
    do {
      status = base->STS2;
      if ((status &
           (FLEXSPI_STS2_AREFLOCK_MASK | FLEXSPI_STS2_ASLVLOCK_MASK)) ==
          (FLEXSPI_STS2_AREFLOCK_MASK | FLEXSPI_STS2_ASLVLOCK_MASK)) {
        retry = 100U;
        break;
      } else if (status == last_status) {
        retry--;
      } else {
        retry = 10U;
        last_status = status;
      }
    } while (retry > 0U);
    while (retry-- > 0U) __NOP();  // ERR011377
  }

  if ((XCACHE_PC->CCR & XCACHE_CCR_ENCACHE_MASK) == 0U) {
    XCACHE_PC->CCR = XCACHE_CCR_INVW1_MASK | XCACHE_CCR_INVW0_MASK;
    XCACHE_PC->CCR |= XCACHE_CCR_GO_MASK;
    while ((XCACHE_PC->CCR & XCACHE_CCR_GO_MASK) != 0U) (void) 0;
    XCACHE_PC->CCR = XCACHE_CCR_ENCACHE_MASK;
    __ISB();
    __DSB();
  }
}

static inline void hal_clock_root(clock_root_t root, uint8_t mux, uint8_t div) {
  const clock_root_config_t config = {
      .clockOff = false,
      .mux = mux,
      .div = div,
  };
  CLOCK_SetRootClock(root, &config);
}

static inline void hal_edgelock_clock_config(uint8_t mux, uint8_t div) {
  if (CLOCK_GetRootClockDiv(kCLOCK_Root_Edgelock) != div ||
      CLOCK_GetRootClockMux(kCLOCK_Root_Edgelock) != mux) {
    status_t status;
    do {
      status = ELE_BaseAPI_ClockChangeStart(MU_RT_S3MUA);
    } while (status != kStatus_Success);

    hal_clock_root(kCLOCK_Root_Edgelock, mux, div);

    uint8_t mhz =
        (uint8_t) (CLOCK_GetRootClockFreq(kCLOCK_Root_Edgelock) / 1000000UL);
    do {
      status = ELE_BaseAPI_ClockChangeFinish(MU_RT_S3MUA, mhz, 0U);
    } while (status != kStatus_Success);
  }
}

static inline void hal_clock_init(void) {
  const clock_sys_pll1_config_t sys_pll1 = {
      .pllDiv2En = true,
      .pllDiv5En = true,
      .ss = NULL,
      .ssEnable = false,
  };
  const clock_sys_pll2_config_t sys_pll2 = {
      .mfd = 268435455U,
      .ss = NULL,
      .ssEnable = false,
  };

  CLOCK_OSC_EnableOscRc400M();
  hal_clock_root(kCLOCK_Root_M33, kCLOCK_M33_ClockRoot_MuxOscRc400M, 2U);

  CLOCK_OSC_Set1MHzOutputBehavior(kCLOCK_1MHzOutEnableFreeRunning1Mhz);
  CLOCK_OSC_EnableOscRc24M(true);
  ANADIG_OSC->OSC_24M_CTRL |=
      ANADIG_OSC_OSC_24M_CTRL_OSC_EN_MASK |
      ANADIG_OSC_OSC_24M_CTRL_LP_EN_MASK;
  ANADIG_OSC->OSC_24M_CTRL &=
      ~(ANADIG_OSC_OSC_24M_CTRL_BYPASS_EN_MASK |
        ANADIG_OSC_OSC_24M_CTRL_OSC_24M_GATE_MASK);
  while ((ANADIG_OSC->OSC_24M_CTRL &
          ANADIG_OSC_OSC_24M_CTRL_OSC_24M_STABLE_MASK) == 0U) {
    (void) 0;
  }

  // The image is linked at 0x04000000 and executes through FlexSPI2.
  hal_flexspi2_clock_config(0U, 1U);  // OSC_RC_24M while PLLs change

  CLOCK_InitSysPll1(&sys_pll1);
  CLOCK_InitSysPll2(&sys_pll2);
  CLOCK_InitSysPll3();
  CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd2, 22U);
  CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd3, 18U);

  hal_clock_root(kCLOCK_Root_M33, kCLOCK_M33_ClockRoot_MuxSysPll3Out, 2U);
  hal_edgelock_clock_config(kCLOCK_EDGELOCK_ClockRoot_MuxOscRc400M, 2U);
  hal_clock_root(kCLOCK_Root_Bus_Aon,
                 kCLOCK_BUS_AON_ClockRoot_MuxSysPll2Out, 4U);
  hal_clock_root(kCLOCK_Root_Bus_Wakeup,
                 kCLOCK_BUS_WAKEUP_ClockRoot_MuxSysPll2Out, 4U);
  hal_clock_root(kCLOCK_Root_Wakeup_Axi,
                 kCLOCK_WAKEUP_AXI_ClockRoot_MuxSysPll3Out, 2U);
  hal_clock_root(kCLOCK_Root_M33_Systick,
                 kCLOCK_M33_SYSTICK_ClockRoot_MuxOsc24MOut, 240U);

  hal_flexspi2_clock_config(
      kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll3Pfd2, 4U);
  hal_clock_root(kCLOCK_Root_Lpuart0102,
                 kCLOCK_LPUART0102_ClockRoot_MuxSysPll3Div2, 10U);

  hal_clock_root(kCLOCK_Root_Enet,
                 kCLOCK_ENET_ClockRoot_MuxSysPll1Div2, 4U);
  hal_clock_root(kCLOCK_Root_Tmr_1588,
                 kCLOCK_TMR_1588_ClockRoot_MuxSysPll3Out, 2U);
  hal_clock_root(kCLOCK_Root_Netc,
                 kCLOCK_NETC_ClockRoot_MuxSysPll3Pfd3, 2U);
  hal_clock_root(kCLOCK_Root_Mac0,
                 kCLOCK_MAC0_ClockRoot_MuxSysPll1Div2, 4U);
  hal_clock_root(kCLOCK_Root_Mac1,
                 kCLOCK_MAC1_ClockRoot_MuxSysPll1Div2, 4U);
  hal_clock_root(kCLOCK_Root_Mac2,
                 kCLOCK_MAC2_ClockRoot_MuxSysPll1Div2, 4U);
  hal_clock_root(kCLOCK_Root_Mac3,
                 kCLOCK_MAC3_ClockRoot_MuxSysPll1Div2, 4U);
  hal_clock_root(kCLOCK_Root_Mac4,
                 kCLOCK_MAC4_ClockRoot_MuxSysPll1Div2, 10U);

  BLK_CTRL_WAKEUPMIX->NETC_PORT_MISC_CFG &=
      ~(BLK_CTRL_WAKEUPMIX_NETC_PORT_MISC_CFG_PORT0_RMII_REF_CLK_DIR_MASK |
        BLK_CTRL_WAKEUPMIX_NETC_PORT_MISC_CFG_PORT1_RMII_REF_CLK_DIR_MASK |
        BLK_CTRL_WAKEUPMIX_NETC_PORT_MISC_CFG_PORT2_RMII_REF_CLK_DIR_MASK |
        BLK_CTRL_WAKEUPMIX_NETC_PORT_MISC_CFG_PORT3_RMII_REF_CLK_DIR_MASK |
        BLK_CTRL_WAKEUPMIX_NETC_PORT_MISC_CFG_PORT4_RMII_REF_CLK_DIR_MASK);
  BLK_CTRL_WAKEUPMIX->NETC_PORT_MISC_CFG |=
      BLK_CTRL_WAKEUPMIX_NETC_PORT_MISC_CFG_TMR_EXT_CLK_SEL_MASK;

  SystemCoreClock = CLOCK_GetRootClockFreq(kCLOCK_Root_M33);
}

static inline void hal_rtwdog_disable(RTWDOG_Type *wdog) {
  if ((wdog->CS & RTWDOG_CS_CMD32EN_MASK) != 0U) {
    wdog->CNT = 0xD928C520U;
  } else {
    wdog->CNT = 0xC520U;
    wdog->CNT = 0xD928U;
  }
  wdog->TOVAL = 0xFFFFU;
  wdog->CS = (wdog->CS & ~RTWDOG_CS_EN_MASK) | RTWDOG_CS_UPDATE_MASK;
}

static inline void hal_system_init(void) {
  extern uint32_t _siram, _eiram, _siiram;
  volatile uint32_t *dst = &_siram;
  const uint32_t *src = &_siiram;

  while (dst < &_eiram) *dst++ = *src++;
  SCB->CPACR |= (3UL << 10U * 2U) | (3UL << 11U * 2U);

  hal_rtwdog_disable(RTWDOG1);
  hal_rtwdog_disable(RTWDOG2);
  hal_rtwdog_disable(RTWDOG3);
  hal_rtwdog_disable(RTWDOG4);
  hal_rtwdog_disable(RTWDOG5);

  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
  if ((XCACHE_PC->CCR & XCACHE_CCR_ENCACHE_MASK) == 0U) {
    XCACHE_PC->CCR = XCACHE_CCR_INVW1_MASK | XCACHE_CCR_INVW0_MASK;
    XCACHE_PC->CCR |= XCACHE_CCR_GO_MASK;
    while ((XCACHE_PC->CCR & XCACHE_CCR_GO_MASK) != 0U) (void) 0;
    XCACHE_PC->CCR = XCACHE_CCR_ENCACHE_MASK;
  }
  SCB->CCR |= SCB_CCR_DIV_0_TRP_Msk;
  __DSB();
  __ISB();
}

static inline RGPIO_Type *hal_gpio_bank(uint16_t pin) {
  static RGPIO_Type *const g[] = RGPIO_BASE_PTRS;
  return g[PINBANK(pin)];
}

static inline void hal_gpio_mux_config(uint16_t index, uint8_t af) {
  IOMUXC->SW_MUX_CTL_PAD[index] =
      IOMUXC_SW_MUX_CTL_PAD_MUX_MODE(af);
}

static inline void hal_periph_mux_config(uint16_t index, uint8_t input) {
  IOMUXC->SELECT_INPUT[index] = input;
}

static inline void hal_gpio_aon_mux_config(uint16_t index, uint8_t af) {
  IOMUXC_AON->SW_MUX_CTL_PAD[index] =
      IOMUXC_AON_SW_MUX_CTL_PAD_MUX_MODE(af);
}

static inline void hal_gpio_pad_config(uint16_t index, uint8_t type,
                                       uint8_t speed, uint8_t pull) {
  if (index < kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_00 ||
      index >= kIOMUXC_SW_PAD_CTL_PAD_GPIO_SD_B1_00) {
    uint8_t pad_pull = 3U;  // No pull
    if (pull == HAL_GPIO_PULL_UP) pad_pull = 1U;
    if (pull == HAL_GPIO_PULL_DOWN) pad_pull = 2U;
    IOMUXC->SW_PAD_CTL_PAD[index] =
        IOMUXC_SW_PAD_CTL_PAD_PDRV(speed == HAL_GPIO_SPEED_LOW) |
        IOMUXC_SW_PAD_CTL_PAD_ODE(type == HAL_GPIO_OTYPE_OPEN_DRAIN) |
        IOMUXC_SW_PAD_CTL_PAD_PULL(pad_pull);
  } else {
    bool dopull = pull != HAL_GPIO_PULL_NONE;
    IOMUXC->SW_PAD_CTL_PAD[index] =
        IOMUXC_SW_PAD_CTL_PAD_SRE(speed == HAL_GPIO_SPEED_LOW) |
        IOMUXC_SW_PAD_CTL_PAD_DSE(speed == HAL_GPIO_SPEED_HIGH) |
        IOMUXC_SW_PAD_CTL_PAD_ODE(type == HAL_GPIO_OTYPE_OPEN_DRAIN) |
        IOMUXC_SW_PAD_CTL_PAD_PUE(dopull) |
        IOMUXC_SW_PAD_CTL_PAD_PUS(pull == HAL_GPIO_PULL_UP);
  }
}

static inline void hal_gpio_aon_pad_config(uint16_t index, uint8_t type,
                                           uint8_t speed, uint8_t pull) {
  bool dopull = pull != HAL_GPIO_PULL_NONE;
  IOMUXC_AON->SW_PAD_CTL_PAD[index] =
      IOMUXC_AON_SW_PAD_CTL_PAD_SRE(speed == HAL_GPIO_SPEED_LOW) |
      IOMUXC_AON_SW_PAD_CTL_PAD_DSE(speed == HAL_GPIO_SPEED_HIGH) |
      IOMUXC_AON_SW_PAD_CTL_PAD_ODE(type == HAL_GPIO_OTYPE_OPEN_DRAIN) |
      IOMUXC_AON_SW_PAD_CTL_PAD_PUE(dopull) |
      IOMUXC_AON_SW_PAD_CTL_PAD_PUS(pull == HAL_GPIO_PULL_UP);
}

static inline void hal_gpio_init(uint16_t pin, uint8_t mode, uint8_t type,
                                 uint8_t speed, uint8_t pull) {
  static const clock_ip_name_t clocks[] = GPIO_CLOCKS;
  uint8_t bank = (uint8_t) PINBANK(pin);
  uint8_t bit = (uint8_t) PINNO(pin);
  uint32_t mask = 1UL << bit;
  uint16_t index;

  switch (bank) {
    case 1:
      index = kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_00 + bit;
      break;
    case 2:
      index = kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_00 + bit;
      break;
    case 3:
      index = kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_32 + bit;
      break;
    case 4:
      index = kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_00 + bit;
      break;
    case 5:
      index = kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_32 + bit;
      break;
    case 6:
      index = kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_00 + bit;
      break;
    default:
      return;
  }

  RGPIO_Type *gpio = hal_gpio_bank(pin);
  CLOCK_EnableClock(clocks[bank]);
  CLOCK_EnableClock(bank == 1U ? kCLOCK_Iomuxc2 : kCLOCK_Iomuxc1);

  gpio->ICR[bit] = RGPIO_ICR_ISF_MASK;  // Disable and clear interrupts
  gpio->PIDR &= ~mask;                  // Keep the digital input path enabled
  if (mode == HAL_GPIO_MODE_INPUT) {
    gpio->PDDR &= ~mask;
  } else {
    gpio->PCOR = mask;  // Initial output level is low
    gpio->PDDR |= mask;
  }

  if (bank == 1U) {
    hal_gpio_aon_pad_config(index, type, speed, pull);
    hal_gpio_aon_mux_config(index, 5U);
  } else {
    hal_gpio_pad_config(index, type, speed, pull);
    hal_gpio_mux_config(index, 5U);
  }
}

static inline void hal_gpio_input(uint16_t pin) {
  hal_gpio_init(pin, HAL_GPIO_MODE_INPUT, HAL_GPIO_OTYPE_PUSH_PULL,
                HAL_GPIO_SPEED_LOW, HAL_GPIO_PULL_NONE);
}

static inline void hal_gpio_output(uint16_t pin) {
  hal_gpio_init(pin, HAL_GPIO_MODE_OUTPUT, HAL_GPIO_OTYPE_PUSH_PULL,
                HAL_GPIO_SPEED_LOW, HAL_GPIO_PULL_NONE);
}

static inline bool hal_gpio_read(uint16_t pin) {
  RGPIO_Type *gpio = hal_gpio_bank(pin);
  return (gpio->PDIR & (1UL << PINNO(pin))) != 0U;
}

static inline void hal_gpio_write(uint16_t pin, bool value) {
  RGPIO_Type *gpio = hal_gpio_bank(pin);
  uint32_t mask = 1UL << PINNO(pin);
  if (value) {
    gpio->PSOR = mask;
  } else {
    gpio->PCOR = mask;
  }
}

static inline void hal_gpio_toggle(uint16_t pin) {
  RGPIO_Type *gpio = hal_gpio_bank(pin);
  gpio->PTOR = 1UL << PINNO(pin);
}

static inline void hal_uart_init(LPUART_Type *uart, unsigned long baud) {
  if (uart != LPUART1 || baud == 0UL) return;

  uint16_t tx = kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_00 +
                PINNO(HAL_DEBUG_UART_TX_PIN);
  uint16_t rx = kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_00 +
                PINNO(HAL_DEBUG_UART_RX_PIN);
  uint32_t sbr = HAL_DEBUG_UART_CLK / (16UL * baud);
  if (sbr == 0U) sbr = 1U;

  hal_clock_root(HAL_DEBUG_UART_CLK_ROOT,
                 kCLOCK_LPUART0102_ClockRoot_MuxSysPll3Div2, 10U);
  CLOCK_EnableClock(kCLOCK_Iomuxc2);
  CLOCK_EnableClock(kCLOCK_Lpuart1);

  hal_gpio_aon_pad_config(tx, HAL_GPIO_OTYPE_PUSH_PULL,
                          HAL_GPIO_SPEED_HIGH, HAL_GPIO_PULL_NONE);
  hal_gpio_aon_mux_config(tx, 0U);
  hal_gpio_aon_pad_config(rx, HAL_GPIO_OTYPE_PUSH_PULL,
                          HAL_GPIO_SPEED_HIGH, HAL_GPIO_PULL_NONE);
  hal_gpio_aon_mux_config(rx, 0U);

  uart->GLOBAL |= LPUART_GLOBAL_RST_MASK;
  uart->GLOBAL &= ~LPUART_GLOBAL_RST_MASK;
  uart->FIFO = 0U;   // Disable TX/RX FIFOs and FIFO interrupts
  uart->WATER = 0U;
  uart->BAUD = LPUART_BAUD_OSR(16U - 1U) | LPUART_BAUD_SBR(sbr);
  uart->CTRL = LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK;
}

static inline void hal_uart_write_byte(LPUART_Type *uart, uint8_t byte) {
  uart->DATA = byte;
  while ((uart->STAT & LPUART_STAT_TDRE_MASK) == 0U) spin(1);
}

static inline void hal_uart_write_buf(LPUART_Type *uart, const char *buf,
                                      size_t len) {
  while (len-- > 0U) hal_uart_write_byte(uart, (uint8_t) *buf++);
}

static inline int hal_uart_read_ready(LPUART_Type *uart) {
  return (int) (uart->STAT & LPUART_STAT_RDRF_MASK);
}

static inline uint8_t hal_uart_read_byte(LPUART_Type *uart) {
  return (uint8_t) (uart->DATA & 0xFFU);
}

#define HAL_NETC_PHY0_ADDR 5U
#define HAL_NETC_PHY0_RESET PIN('1', 15)

static inline void hal_netc_trdc_region(uint8_t mrc, uint8_t domain,
                                        uint32_t start, uint32_t end) {
  const trdc_mrc_region_descriptor_config_t region = {
      .memoryAccessControlSelect = 7U,
      .startAddr = start,
      .valid = true,
      .nseEnable = false,
      .endAddr = end,
      .mrcIdx = mrc,
      .domainIdx = domain,
      .regionIdx = 0U,
  };
  TRDC_MrcSetRegionDescriptorConfig(TRDC2, &region);
}

// Open the security gates needed by the CPU and NETC
static inline void hal_netc_trdc_init(void) {
  const trdc_processor_domain_assignment_t cm33 = {
      .domainId = 2U,
      .domainIdSelect = kTRDC_DidInput,
      .pidDomainHitConfig = kTRDC_pidDomainHitNone0,
      .pidMask = 0U,
      .secureAttr = kTRDC_ForceSecure,
      .pid = 0U,
      .lock = false,
  };
  const trdc_processor_domain_assignment_t netc = {
      .domainId = 10U,
      .domainIdSelect = kTRDC_DidInput,
      .pidDomainHitConfig = kTRDC_pidDomainHitNone0,
      .pidMask = 0U,
      .secureAttr = kTRDC_ForceSecure,
      .pid = 0U,
      .lock = false,
  };
  const trdc_memory_access_control_config_t netc_access = {
      .secureUsrW = 1U,
      .secureUsrR = 1U,
      .securePrivW = 1U,
      .securePrivR = 1U,
  };
  const trdc_memory_access_control_config_t cpu_access = {
      .securePrivW = 1U,
      .securePrivR = 1U,
  };

  ELE_BaseAPI_ReleaseRDC(MU_RT_S3MUA, 0x74U, 1U);  // TRDC AON
  ELE_BaseAPI_ReleaseRDC(MU_RT_S3MUA, 0x82U, 1U);  // TRDC MEGA
  ELE_BaseAPI_ReleaseRDC(MU_RT_S3MUA, 0x78U, 1U);  // TRDC WAKEUP

  TRDC_SetProcessorDomainAssignment(
      TRDC1, (uint8_t) kTRDC1_MasterCM33, 0U, &cm33);
  TRDC_SetProcessorDomainAssignment(
      TRDC2, (uint8_t) kTRDC2_MasterNETC, 0U, &netc);

  TRDC_MrcSetMemoryAccessConfig(TRDC2, &netc_access, 3U, 7U);
  TRDC_MrcSetMemoryAccessConfig(TRDC2, &netc_access, 4U, 7U);
  TRDC_MrcSetMemoryAccessConfig(TRDC2, &cpu_access, 6U, 7U);
  hal_netc_trdc_region(3U, 10U, 0x20480000UL, 0x204FFFFFUL);
  hal_netc_trdc_region(4U, 10U, 0x20500000UL, 0x2053FFFFUL);
  hal_netc_trdc_region(6U, 2U, 0x60000000UL, 0x60FFFFFFUL);
}

static inline void hal_netc_pins_init(void) {
  static const struct {
    uint16_t index;
    uint8_t af;
  } muxes[] = {
      {kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B2_00, 4U}, // RX_CLK
      {kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B2_01, 4U}, // RX_DATA_2
      {kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B2_02, 4U}, // RX_DATA_3
      {kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B2_03, 4U}, // TX_DATA_2
      {kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B2_04, 4U}, // TX_DATA_3
      {kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B2_05, 3U}, // TX_DATA_0
      {kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B2_06, 3U}, // TX_DATA_1
      {kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B2_07, 3U}, // TX_EN
      {kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B2_08, 3U}, // TX_CLK
      {kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B2_09, 3U}, // RX_DATA_0
      {kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B2_10, 3U}, // RX_DATA_1
      {kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B2_11, 3U}, // RX_EN
  };
  static const struct {
    uint16_t index;
    uint8_t input;
  } inputs[] = {
      {kIOMUXC_NETC_PINMUX_IPP_IND_ETH0_RX_CLK_SELECT_INPUT, 0U},
      {kIOMUXC_NETC_PINMUX_IPP_IND_ETH0_RX_DV_SELECT_INPUT, 1U},
      {kIOMUXC_NETC_PINMUX_IPP_IND_ETH0_RXD_SELECT_INPUT_0, 1U},
      {kIOMUXC_NETC_PINMUX_IPP_IND_ETH0_RXD_SELECT_INPUT_1, 1U},
      {kIOMUXC_NETC_PINMUX_IPP_IND_ETH0_RXD_SELECT_INPUT_2, 0U},
      {kIOMUXC_NETC_PINMUX_IPP_IND_ETH0_RXD_SELECT_INPUT_3, 0U},
      {kIOMUXC_NETC_PINMUX_IPP_IND_ETH0_TX_CLK_SELECT_INPUT, 1U},
  };

  CLOCK_EnableClock(kCLOCK_Iomuxc1);
  CLOCK_EnableClock(kCLOCK_Iomuxc2);

  hal_gpio_output(HAL_NETC_PHY0_RESET);
  hal_gpio_write(HAL_NETC_PHY0_RESET, true);

  hal_gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_30, 7U); // EMDC
  hal_gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_30,
                      HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_HIGH,
                      HAL_GPIO_PULL_DOWN);
  hal_gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_31, 7U); // EMDIO
  hal_gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_31,
                      HAL_GPIO_OTYPE_OPEN_DRAIN, HAL_GPIO_SPEED_HIGH,
                      HAL_GPIO_PULL_DOWN);
  hal_periph_mux_config(kIOMUXC_NETC_EMDIO_IN_SELECT_INPUT, 4U);

  for (size_t i = 0; i < sizeof(muxes) / sizeof(muxes[0]); i++) {
    hal_gpio_mux_config(muxes[i].index, muxes[i].af);
  }
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); i++) {
    hal_periph_mux_config(inputs[i].index, inputs[i].input);
  }
}

static inline bool hal_netc_ierb_init(void) {
  uint32_t timeout = 1000000UL;

  BLK_CTRL_WAKEUPMIX->NETC_LINK_CFG[0] =
      (BLK_CTRL_WAKEUPMIX->NETC_LINK_CFG[0] &
       ~BLK_CTRL_WAKEUPMIX_NETC_LINK_CFG_MII_PROT_MASK) |
      BLK_CTRL_WAKEUPMIX_NETC_LINK_CFG_MII_PROT(2U);
  if ((NETC_PRIV->NETCRR & NETC_PRIV_NETCRR_LOCK_MASK) != 0U) {
    NETC_PRIV->NETCRR &= ~NETC_PRIV_NETCRR_LOCK_MASK;
    while ((NETC_PRIV->NETCRR & NETC_PRIV_NETCRR_LOCK_MASK) != 0U &&
           timeout > 0U) {
      timeout--;
    }
    if ((NETC_PRIV->NETCRR & NETC_PRIV_NETCRR_LOCK_MASK) != 0U) return false;
  }

  // Attribute 1 permits MSI-X writes through the root-complex window.
  NETC_IERB->ARRAY_NUM_RC[0].RCMSIAMQR =
      (NETC_IERB->ARRAY_NUM_RC[0].RCMSIAMQR & ~(7UL << 27)) | (1UL << 27);
  // RT1180's SoC-specific topology fixes link 0 to switch port 0, link 4 to
  // ENETC0, and link 5 between switch port 4 and ENETC1.
  NETC_IERB->L0BCR =
      NETC_IERB_L0BCR_MDIO_PHYAD_PRTAD(HAL_NETC_PHY0_ADDR);
  __DSB();

  if ((NETC_PRIV->NETCSR & NETC_PRIV_NETCSR_ERROR_MASK) != 0U) return false;
  NETC_PRIV->NETCRR |= NETC_PRIV_NETCRR_LOCK_MASK;
  timeout = 1000000UL;
  while ((NETC_PRIV->NETCSR & NETC_PRIV_NETCSR_STATE_MASK) != 0U &&
         timeout > 0U) {
    timeout--;
  }
  return (NETC_PRIV->NETCSR &
          (NETC_PRIV_NETCSR_STATE_MASK | NETC_PRIV_NETCSR_ERROR_MASK)) == 0U;
}

static inline void hal_ethernet_init(void) {
  hal_netc_trdc_init();
  CLOCK_EnableClock(kCLOCK_Netc);
  hal_netc_pins_init();
  if (!hal_netc_ierb_init()) return;

  hal_gpio_write(HAL_NETC_PHY0_RESET, false);
  SDK_DelayAtLeastUs(10000U, SYS_FREQUENCY);
  hal_gpio_write(HAL_NETC_PHY0_RESET, true);
  SDK_DelayAtLeastUs(150000U, SYS_FREQUENCY);
  NVIC_EnableIRQ(MSGINTR1_IRQn);
}

static inline void hal_rng_init(void) {
}

static inline uint32_t hal_rng_read(void) {
  return 42;
}
