// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#define LED PICO_DEFAULT_LED_PIN

static inline int gpio_read(uint16_t pin) {
  return gpio_get_out_level(pin);
}
static inline void gpio_write(uint16_t pin, bool val) {
  gpio_put(pin, val);
}
