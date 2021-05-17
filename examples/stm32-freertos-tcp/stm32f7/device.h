// Copyright (c) 2018-2021 Cesanta Software Limited
// All rights reserved
// https://www.st.com/resource/en/reference_manual/dm00124865-stm32f75xxx-and-stm32f74xxx-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf
// Memory map: 2.2.2

#include "stm32f746xx.h"
//#include "stm32f7xx_hal.h"
#include "string.h"

#define BIT(x) ((uint32_t) 1 << (x))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))

static inline void spin(uint32_t count) {
  while (count--) asm("nop");
}
static inline GPIO_TypeDef *gpio_bank(uint16_t pin) {
  return (GPIO_TypeDef *) (0x40020000 + 0x400 * (pin >> 8));
}
static inline void gpio_on(uint16_t pin) {
  gpio_bank(pin)->ODR |= BIT(pin & 255);
}
static inline void gpio_off(uint16_t pin) {
  gpio_bank(pin)->ODR &= ~BIT(pin & 255);
}
static inline void gpio_toggle(uint16_t pin) {
  gpio_bank(pin)->ODR ^= BIT(pin & 255);
}
enum { GPIO_IN, GPIO_OUT, GPIO_AF, GPIO_ANALOG };
enum { GPIO_PP, GPIO_OD };
enum { GPIO_SPEED_LOW, GPIO_SPEED_MEDIUM, GPIO_SPEED_HIGH, GPIO_SPEED_INSANE };
enum { GPIO_PULL_NONE, GPIO_PULL_UP, GPIO_PULL_DOWN };
static inline void gpio_init(uint16_t pin, uint8_t mode, uint8_t type,
                             uint8_t speed, uint8_t pull, uint8_t af) {
  GPIO_TypeDef *gpio = gpio_bank(pin);
  uint8_t n = pin & 255;
  gpio->MODER &= ~(3 << (n * 2));
  gpio->MODER |= (mode << (n * 2));
  gpio->OTYPER &= ~(1 << n);
  gpio->OTYPER |= (type << n);
  gpio->OSPEEDR &= ~(3 << (n * 2));
  gpio->OSPEEDR |= (speed << (n * 2));
  gpio->PUPDR &= ~(3 << (n * 2));
  gpio->PUPDR |= (pull << (n * 2));
  if (n < 8) {
    gpio->AFR[0] &= 15 << (n * 4);
    gpio->AFR[0] |= af << (n * 4);
  } else {
    gpio->AFR[1] &= 15 << (n * 4);
    gpio->AFR[1] |= af << (n * 4);
  }
}

static inline void init_clock(void) {
  RCC->CR |= RCC_CR_HSEBYP | RCC_CR_HSEON;  // Start HSE
  while ((RCC->CR & RCC_CR_HSERDY) == 0) (void) 0;

#define PLL_M 8    // 8MHz HSE down-to 1MHz PLL input
#define PLL_N 432  // 432 MHz VCO output
#define PLL_P 2    // 216 MHz PLL output
#define PLL_Q 9    // 48  MHz (USB)
  RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) - 1) << 16) |
                 (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24);
  RCC->CR |= RCC_CR_PLLON;  // Enable the main PLL
  FLASH->ACR = FLASH_ACR_ARTEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_4WS;

  // Configure AHB/APB prescalers
  // AHB  Prescaler = /1	-> 216 MHz
  // APB1 Prescaler = /4  -> 54  MHz
  // APB2 Prescaler = /2  -> 108 MHz
  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
  while ((RCC->CR & RCC_CR_PLLRDY) == 0) (void) 0;

  RCC->CFGR &= ~RCC_CFGR_SW;     // Select the main PLL
  RCC->CFGR |= RCC_CFGR_SW_PLL;  // as system clock source
  while ((RCC->CFGR & RCC_CFGR_SWS) == 0) (void) 0;

  // Ethernet clock
  RCC->AHB1ENR |= RCC_AHB1ENR_ETHMACEN;
  RCC->AHB1ENR |= RCC_AHB1ENR_ETHMACTXEN;
  RCC->AHB1ENR |= RCC_AHB1ENR_ETHMACRXEN;
}

#define delay(ms) vTaskDelay(pdMS_TO_TICKS(ms))

#define LED1 PIN('B', 0)  // On-board LED pin
#define LED2 PIN('B', 7)  // On-board LED pin
#define LED3 PIN('B', 14)  // On-board LED pin

static inline void led_toggle(void) {
  gpio_toggle(LED2);
}

static inline void init_ram(void) {
  extern uint32_t _sbss, _ebss;
  extern uint32_t _sdata, _edata, _sidata;
  memset(&_sbss, 0, ((char *) &_ebss - (char *) &_sbss));
  memcpy(&_sdata, &_sidata, ((char *) &_edata - (char *) &_sdata));
}

static inline void init_hardware(void) {
  init_ram();
  init_clock();
  RCC->AHB1ENR |= BIT(0) | BIT(1) | BIT(2) | BIT(6);  // Init GPIO banks A,B,C,G
  gpio_init(LED1, GPIO_OUT, GPIO_PP, GPIO_SPEED_LOW, GPIO_PULL_NONE, 0);
  gpio_init(LED2, GPIO_OUT, GPIO_PP, GPIO_SPEED_LOW, GPIO_PULL_NONE, 0);
  gpio_init(LED3, GPIO_OUT, GPIO_PP, GPIO_SPEED_LOW, GPIO_PULL_NONE, 0);
  gpio_on(LED2);
#if 0
  uint16_t a1 = PIN('A', 1), a2 = PIN('A', 2), a7 = PIN('A', 7);
  gpio_init(a1, GPIO_AF, GPIO_PP, GPIO_SPEED_INSANE, GPIO_PULL_NONE, 11);
  gpio_init(a2, GPIO_AF, GPIO_PP, GPIO_SPEED_INSANE, GPIO_PULL_NONE, 11);
  gpio_init(a7, GPIO_AF, GPIO_PP, GPIO_SPEED_INSANE, GPIO_PULL_NONE, 11);

  uint16_t b13 = PIN('B', 13);
  gpio_init(b13, GPIO_AF, GPIO_PP, GPIO_SPEED_INSANE, GPIO_PULL_NONE, 11);

  uint16_t c1 = PIN('C', 1), c4 = PIN('C', 4), c5 = PIN('C', 5);
  gpio_init(c1, GPIO_AF, GPIO_PP, GPIO_SPEED_INSANE, GPIO_PULL_NONE, 11);
  gpio_init(c4, GPIO_AF, GPIO_PP, GPIO_SPEED_INSANE, GPIO_PULL_NONE, 11);
  gpio_init(c5, GPIO_AF, GPIO_PP, GPIO_SPEED_INSANE, GPIO_PULL_NONE, 11);

  uint16_t g2 = PIN('G', 2), g11 = PIN('G', 11), g13 = PIN('G', 13),
           g14 = PIN('G', 14);
  gpio_init(g2, GPIO_AF, GPIO_PP, GPIO_SPEED_INSANE, GPIO_PULL_NONE, 11);
  gpio_init(g11, GPIO_AF, GPIO_PP, GPIO_SPEED_INSANE, GPIO_PULL_NONE, 11);
  gpio_init(g13, GPIO_AF, GPIO_PP, GPIO_SPEED_INSANE, GPIO_PULL_NONE, 11);
  gpio_init(g14, GPIO_AF, GPIO_PP, GPIO_SPEED_INSANE, GPIO_PULL_NONE, 11);

  HAL_Init();
  HAL_NVIC_SetPriority(ETH_IRQn, 0x7, 0);
  HAL_NVIC_EnableIRQ(ETH_IRQn);
#endif
}
