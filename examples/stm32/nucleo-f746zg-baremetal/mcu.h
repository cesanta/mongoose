// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved
// https://www.st.com/resource/en/reference_manual/dm00124865-stm32f75xxx-and-stm32f74xxx-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BIT(x) (1UL << (x))
#define SETBITS(R, CLEARMASK, SETMASK) (R) = ((R) & ~(CLEARMASK)) | (SETMASK)
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

// System clock
enum { APB1_PRE = 5 /* AHB clock / 4*/, APB2_PRE = 4 /* AHB clock / 2 */ };
enum { PLL_HSI = 16, PLL_M = 8, PLL_N = 216, PLL_P = 2 };  // Run at 216 Mhz
//#define PLL_FREQ PLL_HSI
#define PLL_FREQ (PLL_HSI * PLL_N / PLL_M / PLL_P)
#define FREQ (PLL_FREQ * 1000000)

static inline void spin(volatile uint32_t count) {
  while (count--) asm("nop");
}

struct rcc {
  volatile uint32_t CR, PLLCFGR, CFGR, CIR, AHB1RSTR, AHB2RSTR, AHB3RSTR,
      RESERVED0, APB1RSTR, APB2RSTR, RESERVED1[2], AHB1ENR, AHB2ENR, AHB3ENR,
      RESERVED2, APB1ENR, APB2ENR, RESERVED3[2], AHB1LPENR, AHB2LPENR,
      AHB3LPENR, RESERVED4, APB1LPENR, APB2LPENR, RESERVED5[2], BDCR, CSR,
      RESERVED6[2], SSCGR, PLLI2SCFGR, PLLSAICFGR, DCKCFGR1, DCKCFGR2;
};
#define RCC ((struct rcc *) 0x40023800)

struct pwr {
  volatile uint32_t CR1, CSR1, CR2, CSR2;
};
#define PWR ((struct pwr *) 0x40007000)

struct nvic {
  volatile uint32_t ISER[8], RESERVED0[24], ICER[8], RSERVED1[24], ISPR[8],
      RESERVED2[24], ICPR[8], RESERVED3[24], IABR[8], RESERVED4[56], IP[240],
      RESERVED5[644], STIR;
};
#define NVIC ((struct nvic *) 0xe000e100)
static inline void nvic_set_prio(int irq, uint32_t prio) {
  NVIC->IP[irq] = prio << 4;
}
static inline void nvic_enable_irq(int irq) {
  NVIC->ISER[irq >> 5] = (uint32_t) (1 << (irq & 31));
}

struct systick {
  volatile uint32_t CTRL, LOAD, VAL, CALIB;
};
#define SYSTICK ((struct systick *) 0xe000e010)  // 2.2.2
static inline void systick_init(uint32_t ticks) {
  if ((ticks - 1) > 0xffffff) return;  // Systick timer is 24 bit
  SYSTICK->LOAD = ticks - 1;
  SYSTICK->VAL = 0;
  SYSTICK->CTRL = BIT(0) | BIT(1) | BIT(2);  // Enable systick
  RCC->APB2ENR |= BIT(14);                   // Enable SYSCFG
}

struct flash {
  volatile uint32_t ACR, KEYR, OPTKEYR, SR, CR, AR, RESERVED, OBR, WRPR;
};
#define FLASH ((struct flash *) 0x40023c00)

struct scb {
  volatile uint32_t CPUID, ICSR, VTOR, AIRCR, SCR, CCR, SHPR[3], SHCSR, CFSR,
      HFSR, DFSR, MMFAR, BFAR, AFSR, ID_PFR[2], ID_DFR, ID_AFR, ID_MFR[4],
      ID_ISAR[5], RESERVED0[1], CLIDR, CTR, CCSIDR, CSSELR, CPACR,
      RESERVED3[93], STIR, RESERVED4[15], MVFR0, MVFR1, MVFR2, RESERVED5[1],
      ICIALLU, RESERVED6[1], ICIMVAU, DCIMVAC, DCISW, DCCMVAU, DCCMVAC, DCCSW,
      DCCIMVAC, DCCISW, RESERVED7[6], ITCMCR, DTCMCR, AHBPCR, CACR, AHBSCR,
      RESERVED8[1], ABFSR;
};
#define SCB ((struct scb *) 0xe000e000)

enum { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG };
enum { GPIO_OTYPE_PUSH_PULL, GPIO_OTYPE_OPEN_DRAIN };
enum { GPIO_SPEED_LOW, GPIO_SPEED_MEDIUM, GPIO_SPEED_HIGH, GPIO_SPEED_INSANE };
enum { GPIO_PULL_NONE, GPIO_PULL_UP, GPIO_PULL_DOWN };

struct gpio {
  volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
};
#define GPIO(N) ((struct gpio *) (0x40020000 + 0x400 * (N)))

static struct gpio *gpio_bank(uint16_t pin) {
  return GPIO(PINBANK(pin));
}
static inline void gpio_toggle(uint16_t pin) {
  struct gpio *gpio = gpio_bank(pin);
  uint32_t mask = BIT(PINNO(pin));
  gpio->BSRR |= mask << (gpio->ODR & mask ? 16 : 0);
}
static inline int gpio_read(uint16_t pin) {
  return gpio_bank(pin)->IDR & BIT(PINNO(pin)) ? 1 : 0;
}
static inline void gpio_write(uint16_t pin, bool val) {
  struct gpio *gpio = gpio_bank(pin);
  gpio->BSRR |= BIT(PINNO(pin)) << (val ? 0 : 16);
}
static inline void gpio_init(uint16_t pin, uint8_t mode, uint8_t type,
                             uint8_t speed, uint8_t pull, uint8_t af) {
  struct gpio *gpio = gpio_bank(pin);
  uint8_t n = (uint8_t) (PINNO(pin));
  RCC->AHB1ENR |= BIT(PINBANK(pin));  // Enable GPIO clock
  SETBITS(gpio->OTYPER, 1UL << n, ((uint32_t) type) << n);
  SETBITS(gpio->OSPEEDR, 3UL << (n * 2), ((uint32_t) speed) << (n * 2));
  SETBITS(gpio->PUPDR, 3UL << (n * 2), ((uint32_t) pull) << (n * 2));
  SETBITS(gpio->AFR[n >> 3], 15UL << ((n & 7) * 4),
          ((uint32_t) af) << ((n & 7) * 4));
  SETBITS(gpio->MODER, 3UL << (n * 2), ((uint32_t) mode) << (n * 2));
}
static inline void gpio_input(uint16_t pin) {
  gpio_init(pin, GPIO_MODE_INPUT, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH,
            GPIO_PULL_NONE, 0);
}
static inline void gpio_output(uint16_t pin) {
  gpio_init(pin, GPIO_MODE_OUTPUT, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH,
            GPIO_PULL_NONE, 0);
}

struct syscfg {
  volatile uint32_t MEMRMP, PMC, EXTICR[4], RESERVED[2], CMPCR;
};
#define SYSCFG ((struct syscfg *) 0x40013800)

struct exti {
  volatile uint32_t IMR, EMR, RTSR, FTSR, SWIER, PR;
};
#define EXTI ((struct exti *) 0x40013c00)

static inline void irq_exti_attach(uint16_t pin) {
  uint8_t bank = (uint8_t) (PINBANK(pin)), n = (uint8_t) (PINNO(pin));
  RCC->APB2ENR |= BIT(14);  // Enable SYSCFG
  SYSCFG->EXTICR[n / 4] &= ~(15UL << ((n % 4) * 4));
  SYSCFG->EXTICR[n / 4] |= (uint32_t) (bank << ((n % 4) * 4));
  EXTI->IMR |= BIT(n);
  EXTI->RTSR |= BIT(n);
  EXTI->FTSR |= BIT(n);
  int irqvec = n < 5 ? 6 + n : n < 10 ? 23 : 40;  // IRQ vector index, 10.1.2
  nvic_set_prio(irqvec, 3);
  nvic_enable_irq(irqvec);
}

struct uart {
  volatile uint32_t CR1, CR2, CR3, BRR, GTPR, RTOR, RQR, ISR, ICR, RDR, TDR;
};
#define UART1 ((struct uart *) 0x40011000)
#define UART2 ((struct uart *) 0x40004400)
#define UART3 ((struct uart *) 0x40004800)

static inline void uart_init(struct uart *uart, unsigned long baud) {
  // https://www.st.com/resource/en/datasheet/stm32f746zg.pdf
  uint8_t af = 0;           // Alternate function
  uint16_t rx = 0, tx = 0;  // pins

  if (uart == UART1) RCC->APB2ENR |= BIT(4);
  if (uart == UART2) RCC->APB1ENR |= BIT(17);
  if (uart == UART3) RCC->APB1ENR |= BIT(18);

  if (uart == UART1) af = 4, tx = PIN('A', 9), rx = PIN('A', 10);
  if (uart == UART2) af = 4, tx = PIN('A', 2), rx = PIN('A', 3);
  if (uart == UART3) af = 7, tx = PIN('D', 8), rx = PIN('D', 9);

  gpio_init(tx, GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH, 0, af);
  gpio_init(rx, GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH, 0, af);
  uart->CR1 = 0;                          // Disable this UART
  uart->BRR = FREQ / APB2_PRE / baud;     // Baud rate. /4 is a PLL prescaler
  uart->CR1 |= BIT(0) | BIT(2) | BIT(3);  // Set UE, RE, TE
}
static inline void uart_write_byte(struct uart *uart, uint8_t byte) {
  uart->TDR = byte;
  while ((uart->ISR & BIT(7)) == 0) spin(1);
}
static inline void uart_write_buf(struct uart *uart, char *buf, size_t len) {
  while (len-- > 0) uart_write_byte(uart, *(uint8_t *) buf++);
}
static inline int uart_read_ready(struct uart *uart) {
  return uart->ISR & BIT(5);  // If RXNE bit is set, data is ready
}
static inline uint8_t uart_read_byte(struct uart *uart) {
  return (uint8_t) (uart->RDR & 255);
}

static inline void clock_init(void) {  // Set clock to 216Mhz
#if 0
  RCC->APB1ENR |= BIT(28);                     // Power enable
  PWR->CR1 |= 3UL << 14;                       // Voltage regulator scale 3
  PWR->CR1 |= BIT(16);                         // Enable overdrive
  while ((PWR->CSR1 & BIT(16)) == 0) spin(1);  // Wait until done
  PWR->CR1 |= BIT(17);                         // Enable overdrive switching
  while ((PWR->CSR1 & BIT(17)) == 0) spin(1);  // Wait until done
  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));  // Enable FPU
#endif
  FLASH->ACR |= 7 | BIT(8) | BIT(9);                // Flash latency 7, prefetch
  RCC->PLLCFGR &= ~((BIT(17) - 1));                 // Clear PLL multipliers
  RCC->PLLCFGR |= (((PLL_P - 2) / 2) & 3) << 16;    // Set PLL_P
  RCC->PLLCFGR |= PLL_M | (PLL_N << 6);             // Set PLL_M and PLL_N
  RCC->CR |= BIT(24);                               // Enable PLL
  while ((RCC->CR & BIT(25)) == 0) spin(1);         // Wait until done
  RCC->CFGR = (APB1_PRE << 10) | (APB2_PRE << 13);  // Set prescalers
  RCC->CFGR |= 2;                                   // Set clock source to PLL
  while ((RCC->CFGR & 12) == 0) spin(1);            // Wait until done
}
