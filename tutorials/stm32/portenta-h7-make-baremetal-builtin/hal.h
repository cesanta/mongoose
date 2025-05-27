// Copyright (c) 2022-2023 Cesanta Software Limited
// All rights reserved
//
// RM0399
// https://www.st.com/resource/en/reference_manual/rm0399-stm32h745755-and-stm32h747757-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
// https://www.st.com/resource/en/datasheet/stm32h747xi.pdf
// https://docs.arduino.cc/hardware/portenta-h7/

#pragma once

#define LED1 PIN('K', 5)  // On-board LED pin (red)
#define LED2 PIN('K', 6)  // On-board LED pin (green)
#define LED3 PIN('K', 7)  // On-board LED pin (blue)
#define LED LED2          // Use green LED for blinking

#ifndef UART_DEBUG
#define UART_DEBUG USART1
#endif

#include <stm32h747xx.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BIT(x) (1UL << (x))
#define CLRSET(reg, clear, set) ((reg) = ((reg) & ~(clear)) | (set))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

void hal_init(void);
size_t hal_ram_free(void);
size_t hal_ram_used(void);

// System clock (2.1, Fig 1; 8.5, Fig 45; 8.5.5, Fig 47; 8.5.6, Fig 49)
// - This board defaults to work with an SMPS regulator, so:
//   - SYS_FREQUENCY <= 400 MHz; hclk = SYS_FREQUENCY / HPRE ; hclk <= 200 MHz;
//   APB clocks <= 100 MHz.
// - D1 domain bus matrix (and so flash) runs at hclk frequency. Configure flash
// latency (WS) in accordance to hclk freq (4.3.8, Table 17)
// - The Ethernet controller is in D2 domain and runs at hclk frequency
enum {
  D1CPRE = 1,  // actual divisor value
  HPRE = 2,    // actual divisor value
  D1PPRE = 4,  // register values, divisor value = BIT(value - 3) = / 2
  D2PPRE1 = 4,
  D2PPRE2 = 4,
  D3PPRE = 4
};
// PLL1_P: odd division factors are not allowed (8.7.13) (according to Cube, '1'
// is also an "odd division factor").
// Make sure your chip is revision 'V', otherwise set PLL1_N = 400
enum { PLL1_HSI = 64, PLL1_M = 32, PLL1_N = 400, PLL1_P = 2 , PLL1_Q = 4 };
#define SYS_FREQUENCY ((PLL1_HSI * PLL1_N / PLL1_M / PLL1_P / D1CPRE) * 1000000)
// #define SYS_FREQUENCY ((PLL1_HSI / D1CPRE) * 1000000)
// #define SYS_FREQUENCY 64000000
#define AHB_FREQUENCY (SYS_FREQUENCY / HPRE)
#define APB2_FREQUENCY (AHB_FREQUENCY / (BIT(D2PPRE2 - 3)))
#define APB1_FREQUENCY (AHB_FREQUENCY / (BIT(D2PPRE1 - 3)))
#define PLL1_Q_FREQUENCY ((PLL1_HSI * PLL1_N / PLL1_M / PLL1_Q) * 1000000)

static inline void spin(volatile uint32_t n) {
  while (n--) (void) 0;
}

enum { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG };
enum { GPIO_OTYPE_PUSH_PULL, GPIO_OTYPE_OPEN_DRAIN };
enum { GPIO_SPEED_LOW, GPIO_SPEED_MEDIUM, GPIO_SPEED_HIGH, GPIO_SPEED_INSANE };
enum { GPIO_PULL_NONE, GPIO_PULL_UP, GPIO_PULL_DOWN };

#define GPIO(N) ((GPIO_TypeDef *) (0x40000000 + 0x18020000UL + 0x400 * (N)))

static GPIO_TypeDef *gpio_bank(uint16_t pin) {
  return GPIO(PINBANK(pin));
}
static inline void gpio_toggle(uint16_t pin) {
  GPIO_TypeDef *gpio = gpio_bank(pin);
  uint32_t mask = BIT(PINNO(pin));
  gpio->BSRR = mask << (gpio->ODR & mask ? 16 : 0);
}
static inline int gpio_read(uint16_t pin) {
  return gpio_bank(pin)->IDR & BIT(PINNO(pin)) ? 1 : 0;
}
static inline void gpio_write(uint16_t pin, bool val) {
  GPIO_TypeDef *gpio = gpio_bank(pin);
  gpio->BSRR = BIT(PINNO(pin)) << (val ? 0 : 16);
}
static inline void gpio_init(uint16_t pin, uint8_t mode, uint8_t type,
                             uint8_t speed, uint8_t pull, uint8_t af) {
  GPIO_TypeDef *gpio = gpio_bank(pin);
  uint8_t n = (uint8_t) (PINNO(pin));
  RCC->AHB4ENR |= BIT(PINBANK(pin));  // Enable GPIO clock
  CLRSET(gpio->OTYPER, 1UL << n, ((uint32_t) type) << n);
  CLRSET(gpio->OSPEEDR, 3UL << (n * 2), ((uint32_t) speed) << (n * 2));
  CLRSET(gpio->PUPDR, 3UL << (n * 2), ((uint32_t) pull) << (n * 2));
  CLRSET(gpio->AFR[n >> 3], 15UL << ((n & 7) * 4),
         ((uint32_t) af) << ((n & 7) * 4));
  CLRSET(gpio->MODER, 3UL << (n * 2), ((uint32_t) mode) << (n * 2));
}
static inline void gpio_input(uint16_t pin) {
  gpio_init(pin, GPIO_MODE_INPUT, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH,
            GPIO_PULL_NONE, 0);
}
static inline void gpio_output(uint16_t pin) {
  gpio_init(pin, GPIO_MODE_OUTPUT, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH,
            GPIO_PULL_NONE, 0);
}

static inline void leds_init(void) {
  gpio_output(LED1);       // Initialise LED1
  gpio_output(LED2);       // Initialise LED2
  gpio_output(LED3);       // Initialise LED3
  gpio_write(LED1, true);  // OFF
  gpio_write(LED2, true);
  gpio_write(LED3, true);
}

// D2 Kernel clock (8.7.21) USART1 defaults to pclk2 (APB2), while USART2,3
// default to pclk1 (APB1). Even if using other kernel clocks, the APBx clocks
// must be enabled for CPU access, as the kernel clock drives the BRR, not the
// APB bus interface
static inline void uart_init(USART_TypeDef *uart, unsigned long baud) {
  uint8_t af = 7;           // Alternate function
  uint16_t rx = 0, tx = 0;  // pins
  uint32_t freq = 0;        // Bus frequency. UART1 is on APB2, rest on APB1

  if (uart == USART1) freq = APB2_FREQUENCY, RCC->APB2ENR |= BIT(4);
  if (uart == USART2) freq = APB1_FREQUENCY, RCC->APB1LENR |= BIT(17);
  if (uart == USART3) freq = APB1_FREQUENCY, RCC->APB1LENR |= BIT(18);

  if (uart == USART1) tx = PIN('A', 9), rx = PIN('A', 10);
  if (uart == USART2) tx = PIN('A', 2), rx = PIN('A', 3);
  if (uart == USART3) tx = PIN('D', 8), rx = PIN('D', 9);

#if 1                                     // CONSTANT BAUD RATE FOR DEBUGGING
  CLRSET(RCC->D2CCIP2R, 7 << 3, 3 << 3);  // use HSI for UART1
  freq = 64000000;
#endif

  gpio_init(tx, GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH, 0, af);
  gpio_init(rx, GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH, 0, af);
  uart->CR1 = 0;                         // Disable this UART
  uart->BRR = freq / baud;               // Set baud rate
  uart->CR1 = BIT(0) | BIT(2) | BIT(3);  // Set UE, RE, TE
}
static inline void uart_write_byte(USART_TypeDef *uart, uint8_t byte) {
  uart->TDR = byte;
  while ((uart->ISR & BIT(7)) == 0) spin(1);
}
static inline void uart_write_buf(USART_TypeDef *uart, char *buf, size_t len) {
  while (len-- > 0) uart_write_byte(uart, *(uint8_t *) buf++);
}
static inline int uart_read_ready(USART_TypeDef *uart) {
  return uart->ISR & BIT(5);  // If RXNE bit is set, data is ready
}
static inline uint8_t uart_read_byte(USART_TypeDef *uart) {
  return (uint8_t) (uart->RDR & 255);
}

static inline void rng_init(void) {
  RCC->D2CCIP2R |= RCC_D2CCIP2R_RNGSEL_0;  // RNG clock source pll1_q_ck
  RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;       // Enable RNG clock
  RNG->CR = RNG_CR_RNGEN;                  // Enable RNG
}

static inline uint32_t rng_read(void) {
  while ((RNG->SR & RNG_SR_DRDY) == 0) (void) 0;
  return RNG->DR;
}

// NO Hw pull-ups on PHY RXD0,1,DV; set them to enable autonegotiation
static inline void ethernet_init(void) {
  // Initialise Ethernet. Enable MAC GPIO pins, see schematics
  uint16_t pins1[] = {PIN('A', 7), PIN('C', 4), PIN('C', 5)};
  for (size_t i = 0; i < sizeof(pins1) / sizeof(pins1[0]); i++) {
    gpio_init(pins1[i], GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_INSANE,
              GPIO_PULL_UP, 11);  // 11 is the Ethernet function
  }
  uint16_t pins2[] = {PIN('A', 1),  PIN('A', 2),  PIN('C', 1),
                      PIN('G', 11), PIN('G', 12), PIN('G', 13)};
  for (size_t i = 0; i < sizeof(pins2) / sizeof(pins2[0]); i++) {
    gpio_init(pins2[i], GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_INSANE,
              GPIO_PULL_NONE, 11);  // 11 is the Ethernet function
  }
  NVIC_EnableIRQ(ETH_IRQn);                     // Setup Ethernet IRQ handler
  CLRSET(SYSCFG->PMCR, 7 << 21, 4 << 21);       // Use RMII (13.3.1)
  RCC->AHB1ENR |= BIT(15) | BIT(16) | BIT(17);  // Enable Ethernet clocks
}

static inline char chiprev(void) {
  uint16_t rev = (uint16_t) (((uint32_t) DBGMCU->IDCODE) >> 16);
  if (rev == 0x1003) return 'Y';
  if (rev == 0x2003) return 'V';
  return '?';
}

static inline unsigned int div2prescval(unsigned int div) {
  // 0 --> /1; 8 --> /2 ... 11 --> /16;  12 --> /64 ... 15 --> /512
  if (div == 1) return 0;
  if (div > 16) div /= 2;
  unsigned int val = 7;
  while (div >>= 1) ++val;
  return val;
}

static inline unsigned int pllrge(unsigned int f) {
  unsigned int val = 0;
  while (f >>= 1) ++val;
  return val - 1;
}

// HSE: 25MHz in PH0; LSE: in PC14; both unused
static inline void clock_init(void) {
  // Set flash latency. RM section 4.9.1, section 4.3.8 table 16
  CLRSET(FLASH->ACR, (FLASH_ACR_WRHIGHFREQ_Msk | FLASH_ACR_LATENCY_Msk),
         FLASH_ACR_LATENCY_2WS | FLASH_ACR_WRHIGHFREQ_1);
  CLRSET(PWR->CR3, BIT(1), BIT(2));            // select SMPS
  while ((PWR->CSR1 & BIT(13)) == 0) spin(1);  // ACTVOSRDY
  PWR->D3CR |= BIT(15) | BIT(14);              // Select VOS1
  uint32_t f = PWR->D3CR;  // fake read to wait for bus clocking
  while ((PWR->CSR1 & BIT(13)) == 0) spin(1);  // ACTVOSRDY
#if 0
  SYSCFG->PWRCR |= BIT(0);                     // ODEN for LDO to go 480MHz
  f = SYSCFG->PWRCR;
  while ((PWR->CSR1 & BIT(13)) == 0) spin(1);  // ACTVOSRDY
#endif
  (void) f;
  RCC->CFGR &= ~(3 << 0); // Set clock source to HSI (bootloader...)
  RCC->CR &= ~BIT(24);    // Disable PLL1
  CLRSET(
      RCC->D1CFGR, (0x0F << 8) | (7 << 4) | (0x0F << 0),
      (div2prescval(D1CPRE) << 8) | (D1PPRE << 4) | (div2prescval(HPRE) << 0));
  RCC->D2CFGR = (D2PPRE2 << 8) | (D2PPRE1 << 4);
  RCC->D3CFGR = (D3PPRE << 4);
  // DIVQ1EN DIVP1EN PLL1RGE !PLL1VCOSEL !PLL1FRACEN
  CLRSET(RCC->PLLCFGR, (3 << 2) | BIT(1) | BIT(0),
         BIT(17) | BIT(16) | pllrge(PLL1_HSI / PLL1_M) << 2);
  CLRSET(RCC->PLL1DIVR, (0x7F << 16) | (0x7F << 9) | (0x1FF << 0),
         ((PLL1_Q - 1) << 16) | ((PLL1_P - 1) << 9) | ((PLL1_N - 1) << 0));
  CLRSET(RCC->PLLCKSELR, (0x3F << 4) | (3 << 0), PLL1_M << 4);  // PLL1: HSI/M
  RCC->CR |= BIT(24);                                           // Enable PLL1
  while ((RCC->CR & BIT(25)) == 0) spin(1);  // Wait until done
  RCC->CFGR |= (3 << 0);                     // Set clock source to PLL1
  while ((RCC->CFGR & (7 << 3)) != (3 << 3)) spin(1);  // Wait until done
  RCC->APB4ENR |= RCC_APB4ENR_SYSCFGEN;                // Enable SYSCFG
}

static inline void system_init(void) {  // Called automatically by startup code
  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));  // Enable FPU
  SCB_DisableDCache();  // The bootloader enables caches, disable them
  SCB_DisableICache();
  __DSB();
  __ISB();
}

#define PWRPIN PIN('J', 1)

// - 58.4 Table 473
// - 58.5.8 AHB bw >= 3x SDMMC bus bw; Table 492; SD HS => AHB >= ~20MHz
// - SDIO 2.0 section 3.1
static inline void sdmmc_set_ds(void) { // NO FLOW CONTROL ??? REALLY ??? *******************************
  CLRSET(SDMMC1->CLKCR, (1 << 17) | 0x3ff, (1 << 14) | ((PLL1_Q_FREQUENCY / (2 * 25000000)) & 0x3ff));  // 4-bit bus, 25MHz
} 
static inline void sdmmc_set_hs(void) { // mbed enables flow control... AIROC does not
  CLRSET(SDMMC1->CLKCR, 0x3ff, (1 << 17) | (1 << 14) | ((PLL1_Q_FREQUENCY / (2 * 50000000)) & 0x3ff));  // hw flow control, 4-bit bus, 50MHz
}
static inline void sdmmc_init(void) {
  RCC->AHB3RSTR |= BIT(16);  // Reset SDIO block
  RCC->AHB3RSTR &= ~BIT(16);
  RCC->D1CCIPR &= ~BIT(16); // 9.7.18, use PLL1Q
  RCC->AHB3ENR |= BIT(16);  // Enable SDIO clock
  // Initialise SDIO pins, see schematics
  uint16_t pins[] = {PIN('C', 8),  PIN('C', 9),  PIN('C', 10),
                     PIN('C', 11), PIN('C', 12), PIN('D', 2)};
  for (size_t i = 0; i < sizeof(pins) / sizeof(pins[0]); i++) {
    // Portenta H7 doesn't pull-up the SDIO bus
    gpio_init(pins[i], GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_INSANE,
              GPIO_PULL_UP, 12);  // 12 is the SDMMC1 (SDIO) function
  }
  gpio_input(PIN('J', 5));  // wake, ignored
  // 1-bit bus, rising edge, clock always on, no flow control, 400KHz
  SDMMC1->CLKCR = (PLL1_Q_FREQUENCY / (2 * 400000)) & 0x3ff;
  SDMMC1->POWER |= 3 << 0; // SDMMC_POWER_PWRCTRL
  SDMMC1->DCTRL = BIT(10); // Read Wait control by stopping SDMMCCLK
  // wait 74 clocks (~200us) before accessing the "card"
}

static inline bool sdmmc_transact(uint8_t cmd, uint32_t arg, uint32_t *resp) {
  unsigned int wresp = (resp != NULL) ? 1 : 0; // 58.10.4: 2-bit; cmds used
  uint32_t cmdr = BIT(12) | (wresp << 8) | ((cmd & 0x3f) << 0); // CPSMEN
  SDMMC1->ARG = arg;
  CLRSET(SDMMC1->CMD, 0x00011f3f /* CMDSUSPEND CPSMEN WAITPEND WAITINT WAITRESP CMD */, cmdr);
  if (wresp == 0) {
    while ((SDMMC1->STA & SDMMC_STA_CMDSENT) == 0) spin(1);
    SDMMC1->ICR = 0x002000C5; // BUSYD0END CMDSENT CMDREND CTIMEOUT CCRCFAIL
    return true;
  }
  //while ((SDMMC1->STA & 0x00200045 /*BUSYD0END CMDREND CTIMEOUT CCRCFAIL*/) == 0 || SDMMC1->STA & SDMMC_STA_CPSMACT) spin(1);
  while (SDMMC1->STA & SDMMC_STA_CPSMACT) spin(1);
  bool res = (SDMMC1->STA & 0x00200005 /*BUSYD0END CTIMEOUT CCRCFAIL*/) == 0;
  SDMMC1->ICR = 0x002000C5; // BUSYD0END CMDSENT CMDREND CTIMEOUT CCRCFAIL
  *resp = SDMMC1->RESP1;
  if (!res || SDMMC1->RESPCMD != cmd) return false;
  return true;
}

static inline unsigned int _log2(uint16_t x) {
  unsigned int i = 0;
  while (x != 0) x = ((uint16_t)x) >> 1, ++i;
  return i - 1;
}

static inline bool sdmmc_transfer(bool write, uint32_t arg, uint16_t blksz, uint32_t *data, uint32_t length, uint32_t *resp) {
  uint32_t cmdr = BIT(12) | (1 << 8) | (53 << 0); // CPSMEN WRESP CMD=53
  bool bmode = (blksz != 0);
  uint8_t dblksz = bmode ? (uint8_t)_log2(blksz) : 0;
  uint32_t dctrlr = (dblksz << 4) | (bmode ? (0 << 2) : (1 << 2)) | (write ? 0 : BIT(1)); // DBLOCKSIZE=f(blksz) DTMODE=f(bmode) DTDIR=!write !DTEN
  SDMMC1->DCTRL = SDMMC_DCTRL_RWMOD;
  SDMMC1->IDMACTRL = SDMMC_IDMA_IDMAEN; // single buffer
  SDMMC1->IDMABASE0 = (uint32_t)data;
  SDMMC1->DTIMER = (uint32_t) 50000000; // 58.10.7, set for 1 sec @50MHz (2 secs @25MHz)
  SDMMC1->DLEN = length;  // 58.10.8 call with a multiple of blksz (we do)
  CLRSET(SDMMC1->DCTRL, 0xff /* DBLOCKSIZE DTMODE DTDIR DTEN */, dctrlr);
  SDMMC1->CMD = SDMMC_CMD_CMDTRANS;  // CPSM will perform a data transfer
  SDMMC1->ARG = arg;
  SDMMC1->CMD |= cmdr; // other flags cleared above
  while ((SDMMC1->STA & 0x0020003f) == 0 && SDMMC1->STA & SDMMC_STA_DPSMACT)
    spin(1); // incorrect "card" usage will stall here
  bool res = (SDMMC1->STA & 0x0020003f /* BUSYD0END RXOVERR TXUNDERR DTIMEOUT CTIMEOUT DCRCFAIL CCRCFAIL */) == 0;
  SDMMC1->CMD = 0;
  SDMMC1->DLEN = 0;
  SDMMC1->IDMACTRL = 0;
  SDMMC1->ICR = (uint32_t) ~0; // clear all flags
  *resp = SDMMC1->RESP1;
  SDMMC1->DCTRL = SDMMC_DCTRL_RWMOD;
  if (!res || SDMMC1->RESPCMD != 53) return false;
  return true;
}

 



// Bit-banged SPI, pin wiring does not allow using a hw SPI peripheral
#define CYWSPI NULL
#define BBSPI_SPIN 1

static inline void spi_init(void *spi, uint8_t div) {
  // Initialise bit-banged SPI, see: schematics; CYW DS 4.1.1 Table 5, 13.7
  // Table 22
  gpio_input(PIN('C', 8));      // DO -> MISO
  gpio_input(PIN('C', 9));      // IRQ -> ignore
  gpio_output(PIN('C', 10));    // D2 <-- strap for SPI mode
  gpio_output(PIN('C', 11));    // CS
  gpio_output(PIN('C', 12));    // SCLK
  gpio_output(PIN('D', 2));     // DI -> MOSI
  // select SPI mode at power-on/reset time (not WL_REG_ON, actual POR)
  // "Sampling occurs a few milliseconds after an internal POR
  // or deassertion of the external POR
  gpio_write(PIN('C', 10), 0);
  gpio_write(PIN('D', 2), 0);   // idle values
  gpio_write(PIN('C', 12), 0);
  gpio_write(PIN('C', 11), 0);  // no power at startup
  (void) spi;
  (void) div;
}

static inline void spi_write_byte(void *spi, uint8_t byte) {
  unsigned int n = 8;
  while (n--) {
    gpio_write(PIN('D', 2), byte & 0x80);
    spin(BBSPI_SPIN);
    gpio_write(PIN('C', 12), 1);
    byte <<= 1;
    spin(BBSPI_SPIN);
    gpio_write(PIN('C', 12), 0);
  }
  gpio_write(PIN('D', 2), 0);
  (void) spi;
}

static inline uint8_t spi_read_byte(void *spi) {
  uint8_t byte = 0;
  unsigned int n = 8;
  while (n--) {
    byte <<= 1;
    spin(BBSPI_SPIN);
    byte |= gpio_read(PIN('C', 8)) ? 1 : 0;
    gpio_write(PIN('C', 12), 1);
    spin(BBSPI_SPIN);
    gpio_write(PIN('C', 12), 0);
  }
  (void) spi;
  return byte;
}

static inline void spi_select(void *spi, bool s) {
  gpio_write(PIN('C', 11), !s);
  (void) spi;
}

// Current bootloader sets all outputs in the PMIC, no need to use I2C.
// The bootloader inits I2C1 and chips in there, but we change MCU clock
// DS 4 Table 12, schematic PB6,7
static inline void i2c_init(I2C_TypeDef *i2c) {
  CLRSET(RCC->D2CCIP2R, 3 << 12, 2 << 12);  // use HSI for I2C1 (64MHz)
  RCC->APB1LENR |= BIT(21);                 // enable I2C clock
  gpio_init(PIN('B', 6), GPIO_MODE_AF, GPIO_OTYPE_OPEN_DRAIN, GPIO_SPEED_INSANE,
            GPIO_PULL_NONE, 4);
  gpio_init(PIN('B', 7), GPIO_MODE_AF, GPIO_OTYPE_OPEN_DRAIN, GPIO_SPEED_INSANE,
            GPIO_PULL_NONE, 4);
  i2c->CR1 = 0;        // disable before any config change
  i2c->CR1 = BIT(12);  // 50.7 disable filters
  i2c->TIMINGR = 10U << 28 | 3 << 20 | 2 << 16 | 3 << 8 | 9 << 0;  // < 400KHz
  i2c->CR1 |= BIT(0);  // enable after fully configured
}

static inline void i2c_write_reg(I2C_TypeDef *i2c, uint8_t addr, uint8_t reg,
                                 uint8_t byte) {
  i2c->CR2 = BIT(25) | 2 << 16 |
             addr << 1;  // write byte to reg in chip at addr, AUTOEND
  i2c->CR2 |= BIT(13);   // START
  while ((i2c->ISR & BIT(0)) == 0) spin(1);  // TXE
  i2c->TXDR = reg;
  while ((i2c->ISR & BIT(0)) == 0) spin(1);  // TXE
  i2c->TXDR = byte;
  while ((i2c->ISR & BIT(5)) == 0) spin(1);  // STOPF
  i2c->ICR |= BIT(5);                        // STOPCF, clear STOPF
}

static inline uint8_t i2c_read_reg(I2C_TypeDef *i2c, uint8_t addr,
                                   uint8_t reg) {
  i2c->CR2 = 1 << 16 | addr << 1;            // write reg addr to chip at addr
  i2c->CR2 |= BIT(13);                       // START
  while ((i2c->ISR & BIT(0)) == 0) spin(1);  // TXE
  i2c->TXDR = reg;
  while ((i2c->ISR & BIT(6)) == 0) spin(1);            // TC
  i2c->CR2 = BIT(13) | BIT(10) | 1 << 16 | addr << 1;  // read 1 byte, AUTOEND
  i2c->CR2 |= BIT(25);                                 // START
  while ((i2c->ISR & BIT(2)) == 0) spin(1);            // RXNE
  uint8_t byte = (uint8_t) i2c->RXDR;
  while ((i2c->ISR & BIT(5)) == 0) spin(1);  // STOPF
  i2c->ICR |= BIT(5);                        // STOPCF, clear STOPF
  return byte;
}
