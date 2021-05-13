// Copyright (c) 2018-2021 Cesanta Software Limited
// All rights reserved
// https://www.st.com/resource/en/reference_manual/dm00124865-stm32f75xxx-and-stm32f74xxx-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf
// Memory map: 2.2.2

#include "stm32f746xx.h"
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
enum { GPIO_MODE_IN, GPIO_MODE_OUT, GPIO_MODE_AF, GPIO_MODE_ANALOG };
enum { GPIO_TYPE_PP, GPIO_TYPE_OD };
enum { GPIO_SPEED_LOW, GPIO_SPEED_MEDIUM, GPIO_SPEED_HIGH, GPIO_SPEED_INSANE };
enum { GPIO_PULL_NONE, GPIO_PULL_UP, GPIO_PULL_DOWN };
enum {
  OUTPUT = GPIO_MODE_OUT | (GPIO_TYPE_PP << 2) | (GPIO_SPEED_MEDIUM << 3) |
           (GPIO_PULL_NONE << 5),
  INPUT = GPIO_MODE_IN | (GPIO_TYPE_PP << 2) | (GPIO_SPEED_MEDIUM << 3) |
          (GPIO_PULL_NONE << 5),
  INPUT_PULLUP = GPIO_MODE_IN | (GPIO_TYPE_PP << 2) | (GPIO_SPEED_MEDIUM << 3) |
                 (GPIO_PULL_UP << 5),
};
static inline void gpio_init(uint16_t pin, uint8_t state) {
  GPIO_TypeDef *gpio = gpio_bank(pin);
  uint8_t n = pin & 255, mode = state & 3, type = (state >> 2) & 1,
          speed = (state >> 3) & 3, pupdr = (state >> 5) & 3;
  gpio->MODER &= ~(3 << (n * 2));
  gpio->MODER |= (mode << (n * 2));
  gpio->OTYPER &= ~(1 << n);
  gpio->OTYPER |= (type << n);
  gpio->OSPEEDR &= ~(3 << (n * 2));
  gpio->OSPEEDR |= (speed << (n * 2));
  gpio->PUPDR &= ~(3 << (n * 2));
  gpio->PUPDR |= (pupdr << (n * 2));
}

static inline void init_ram(void) {
  extern uint32_t _bss_start, _bss_end;
  extern uint32_t _data_start, _data_end, _data_flash_start;
  memset(&_bss_start, 0, ((char *) &_bss_end - (char *) &_bss_start));
  memcpy(&_data_start, &_data_flash_start,
         ((char *) &_data_end - (char *) &_data_start));
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
}

#define delay(ms) vTaskDelay(pdMS_TO_TICKS(ms))

#define LED1 PIN('B', 0)  // On-board LED pin
#define LED2 PIN('B', 7)  // On-board LED pin
#define LED3 PIN('B', 14)  // On-board LED pin

static inline void led_toggle(void) {
  gpio_toggle(LED2);
}

static inline void init_hardware(void) {
  init_ram();
  init_clock();
  RCC->AHB1ENR |= BIT(0) | BIT(1) | BIT(2);  // Init GPIO banks A,B,C
  gpio_init(LED1, OUTPUT);
  gpio_init(LED2, OUTPUT);
  gpio_init(LED3, OUTPUT);
}
