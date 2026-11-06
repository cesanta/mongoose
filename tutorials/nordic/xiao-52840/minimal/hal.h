#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "nrf.h"
uint64_t hal_get_tick(void);
bool hal_timer_expired(volatile uint64_t *t, uint64_t period, uint64_t now);
size_t hal_ram_used(void);
size_t hal_ram_free(void);
void usb_init(void);
void usb_poll(void);
void usb_log(char ch, void *param);

#define BIT(x) (1UL << (x))
#define PIN(port, num) (((port) << 5) | (num))
#define PINNO(pin) ((pin) & 31)
#define PINPORT(pin) ((pin) >> 5)

static inline NRF_GPIO_Type *hal_gpio_bank(uint16_t pin) {
  return PINPORT(pin) == 0 ? NRF_P0 : NRF_P1;
}

static inline void hal_gpio_write(uint16_t pin, bool val) {
  NRF_GPIO_Type *gpio = hal_gpio_bank(pin);
  if (val) {
    gpio->OUTSET = BIT(PINNO(pin));
  } else {
    gpio->OUTCLR = BIT(PINNO(pin));
  }
}

static inline void hal_gpio_toggle(uint16_t pin) {
  hal_gpio_write(pin, (hal_gpio_bank(pin)->OUT & BIT(PINNO(pin))) == 0);
}

static inline void hal_gpio_output(uint16_t pin) {
  hal_gpio_bank(pin)->PIN_CNF[PINNO(pin)] =
      (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos) |
      (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos);
}

static inline void hal_system_init(void) {
  extern uint32_t __isr_vector[];
  SCB->VTOR = (uint32_t) __isr_vector;  // Application vectors above the bootloader
  __DSB();
  __ISB();
  __enable_irq();
  NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
  NRF_CLOCK->TASKS_HFCLKSTART = 1;
  while (!NRF_CLOCK->EVENTS_HFCLKSTARTED) (void) 0;
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock / 1000);
}

static inline void hal_rng_init(void) {
  NRF_RNG->CONFIG = 1;
  NRF_RNG->TASKS_START = 1;
}

static inline uint32_t hal_rng_read(void) {
  uint32_t value = 0;
  for (unsigned i = 0; i < 4; i++) {
    while (!NRF_RNG->EVENTS_VALRDY) (void) 0;
    value |= (uint32_t) NRF_RNG->VALUE << (i * 8);
    NRF_RNG->EVENTS_VALRDY = 0;
  }
  return value;
}
